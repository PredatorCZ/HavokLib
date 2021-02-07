/*  Havok Format Library
    Copyright(C) 2016-2020 Lukas Cone

    This program is free software : you can redistribute it and / or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see <https://www.gnu.org/licenses/>.
*/

#include "format_new.hpp"
#include "datas/binreader.hpp"
#include "datas/except.hpp"
#include "datas/master_printer.hpp"
#include "internal/hk_internal_api.hpp"
#include "pugixml.hpp"
#include <map>

template <class C> void PtrGuard(const C *val) {
  if (!val) {
    throw std::runtime_error("nullptr detected");
  }
}

template <uint32 tag> struct Read {
  static auto e(BinReaderRef, hkChunk *, hkxNewHeader *){};
};

template <> struct Read<1> {
  static auto e(BinReaderRef rd, hkChunk *holder, hkxNewHeader *) {
    PtrGuard(holder);

    rd.Skip(holder->Size());
  };
};

template <> struct Read<CompileFourCC("SDKV")> {
  static auto e(BinReaderRef rd, hkChunk *, hkxNewHeader *root) {
    PtrGuard(root);
    char buff[8];

    rd.Read(buff);
    buff[4] = 0;

    uint32 version = atoi(buff);

    auto convert = [&] {
      switch (version) {
      case 2015:
        return HK2015;
      case 2016:
        return HK2016;
      case 2017:
        return HK2017;
      default:
        return HKUNKVER;
      }
    };

    root->toolset = convert();

    if (root->toolset == HKUNKVER) {
      throw es::InvalidVersionError(version);
    }
  };
};

template <> struct Read<CompileFourCC("DATA")> {
  static auto e(BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
    PtrGuard(holder);
    PtrGuard(root);

    const uint32 dataSize = holder->Size();
    rd.ReadContainer(root->dataBuffer, dataSize);
  };
};

void ReadTypeNames(BinReaderRef rd, hkChunk *holder, hkxNewHeader *root,
                   std::string &outBuffer, hkxNewHeader::StrVec &outVec) {
  PtrGuard(holder);
  PtrGuard(root);

  const uint32 bufferSize = holder->Size();
  rd.ReadContainer(outBuffer, bufferSize);
  char *buffer = &outBuffer[0];
  es::string_view lastName = buffer;

  for (uint32 t = 0; t < bufferSize; t++) {
    if (!buffer[t]) {
      outVec.push_back(lastName);
      lastName = buffer + t + 1;

      if (lastName.empty()) {
        break;
      }
    }
  }
}

template <> struct Read<CompileFourCC("TSTR")> {
  static auto e(BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
    ReadTypeNames(rd, holder, root, root->classNamesBuffer, root->classNames);
  };
};

template <> struct Read<CompileFourCC("FSTR")> {
  static auto e(BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
    ReadTypeNames(rd, holder, root, root->memberNamesBuffer, root->memberNames);
  };
};

int32 ReadCompressedInt(BinReaderRef rd) {
  uint8 firstInt;
  int32 resultInt = 0;

  rd.Read(firstInt);

  const bool flag1 = (firstInt & 0x80) == 0x80;
  const bool flag2 = (firstInt & 0xC0) == 0xC0;
  const bool flag3 = (firstInt & 0xE0) == 0xE0;

  if (flag3) {
    rd.Read(resultInt);
    resultInt |= firstInt << 4;
  } else if (flag2) {
    uint16 secondInt;
    rd.Read(secondInt);
    resultInt = firstInt << 3;
    resultInt <<= 16;
    resultInt |= secondInt;
  } else if (flag1) {
    uint8 secondInt;
    rd.Read(secondInt);
    resultInt = firstInt & 0x3f;
    resultInt <<= 8;
    resultInt |= secondInt;
  } else {
    resultInt = firstInt;
  }

  return resultInt;
}

template <> struct Read<CompileFourCC("TNAM")> {
  static auto e(BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
    PtrGuard(holder);
    PtrGuard(root);

    const size_t savepos = rd.Tell();
    const uint32 numClasses = ReadCompressedInt(rd) - 1;

    root->typeDescriptors.resize(numClasses);

    for (auto &c : root->typeDescriptors) {
      const uint32 classNameIndex = ReadCompressedInt(rd);
      c.className = root->classNames[classNameIndex];

      uint8 numTemplateArgs;
      rd.Read(numTemplateArgs);
      c.templateArguments.resize(numTemplateArgs);

      for (auto &t : c.templateArguments) {
        int32 argNameIndex = ReadCompressedInt(rd);
        t.argName = root->classNames[argNameIndex];

        argNameIndex = ReadCompressedInt(rd);

        if (argNameIndex) {
          t.argType = &root->typeDescriptors[argNameIndex - 1];
        }
      }
    }

    const int32 diff = static_cast<int32>(holder->Size()) -
                       static_cast<int32>(rd.Tell() - savepos);

    if (diff < 0) {
      printwarning("[Havok] TNAM chunk read too much data, possible incorrect "
                   "class map, proceeding to next chunk.");
    }

    rd.Skip(diff);
  };
};

template <> struct Read<CompileFourCC("TNA1")> : Read<CompileFourCC("TNAM")> {};

template <> struct Read<CompileFourCC("ITEM")> {
  static auto e(BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
    PtrGuard(holder);
    PtrGuard(root);

    const uint32 numFixups = holder->Size() / sizeof(classEntryFixup);

    rd.ReadContainer(root->classEntries, numFixups);
  };
};

template <> struct Read<CompileFourCC("PTCH")> {
  static auto e(BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
    PtrGuard(holder);
    PtrGuard(root);

    if (!root->typeDescriptors.size()) {
      throw std::runtime_error("File is missing type infos.");
    }

    const size_t endPos = rd.Tell() + holder->Size();

    while (rd.Tell() < endPos) {
      uint32 classNameIndex;
      rd.Read(classNameIndex);

      es::string_view toclassname =
          root->typeDescriptors[classNameIndex - 1].className;

      bool isHKArray = toclassname == "hkArray";
      bool isHKRelArray = toclassname == "hkRelArray";

      uint32 numPointers;
      rd.Read(numPointers);

      for (uint32 t = 0; t < numPointers; t++) {
        uint32 cPointer;
        rd.Read(cPointer);

        if (isHKRelArray) {
          uint32 *retarget =
              reinterpret_cast<uint32 *>(&root->dataBuffer[0] + cPointer);
          const classEntryFixup &xfix = root->classEntries[*retarget];

          uint16 *relRetarget = reinterpret_cast<uint16 *>(retarget);
          *relRetarget = xfix.tag - cPointer;
          *(relRetarget + 1) = xfix.count;
        } else {
          uint64 *retarget =
              reinterpret_cast<uint64 *>(&root->dataBuffer[0] + cPointer);
          const classEntryFixup &xfix = root->classEntries[*retarget];

          *retarget =
              reinterpret_cast<uint64>(xfix.tag + root->dataBuffer.data());

          if (isHKArray) {
            *(retarget + 1) = xfix.count;
          }
        }
      }
    }

    for (auto &f : root->classEntries) {
      const int32 clsID = f.Size() - 1;

      if (clsID < 0) {
        continue;
      }

      es::string_view clName = root->typeDescriptors[clsID].className;
      const JenHash chash(clName);
      CRule rule(root->toolset, false, 8); // No way to detect so far
      hkVirtualClass *cls = hkVirtualClass::Create(chash, rule);

      if (cls) {
        cls->SetDataPointer(&root->dataBuffer[0] + f.tag);
        cls->className = clName;
        cls->AddHash(clName);
        cls->header = root;
        root->virtualClasses.emplace_back(cls);
        cls->Process();
      }
    }

    // es::Dispose(root->classEntries);
  };
};

// clang-format off
// With help from 
// github.com/blueskythlikesclouds/TagTools/blob/70d05f892fe84083dc4cc360f5ca37bacb612327/TagTools.py#L222
// clang-format on
template <> struct Read<CompileFourCC("TBOD")> {
  static auto e(BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
    PtrGuard(holder);
    PtrGuard(root);

    if (!root->typeDescriptors.size()) {
      throw std::runtime_error("File is missing type infos.");
    }

    const size_t endPos = rd.Tell() + holder->Size();

    while (rd.Tell() < endPos) {
      const uint32 typeIndex = ReadCompressedInt(rd);

      if (!typeIndex) {
        continue;
      }

      ClassDesc &cDesc = root->typeDescriptors.at(typeIndex - 1);
      const uint32 parentIndex = ReadCompressedInt(rd);

      if (parentIndex) {
        cDesc.base = &root->typeDescriptors.at(parentIndex - 1);
      }

      cDesc.layout = ReadCompressedInt(rd);

      if ((uint16)cDesc.layout > 0x7f) {
        throw std::runtime_error("unk layout flags at: " +
                                 std::to_string(rd.Tell()));
      }

      if (cDesc.layout[BodyLayoutFlags::Flags]) {
        cDesc.flags = ReadCompressedInt(rd);
      }

      if (cDesc.layout[BodyLayoutFlags::IsPointer]) {
        const uint32 index = ReadCompressedInt(rd);
        if (index) {
          cDesc.pointer = &root->typeDescriptors.at(index - 1);
        }
      }

      if (cDesc.layout[BodyLayoutFlags::Version]) {
        cDesc.version = ReadCompressedInt(rd);
      }

      if (cDesc.layout[BodyLayoutFlags::ObjectSize]) {
        cDesc.size = ReadCompressedInt(rd);
        cDesc.alignment = ReadCompressedInt(rd);
      }

      if (cDesc.layout[BodyLayoutFlags::AbstractValue]) {
        cDesc.absstrVal = ReadCompressedInt(rd);
      }

      if (cDesc.layout[BodyLayoutFlags::IsClass]) {
        const uint32 numMembers = ReadCompressedInt(rd);
        cDesc.members.resize(numMembers);

        for (auto &m : cDesc.members) {
          BodyMember cMem;
          cMem.name = root->memberNames[ReadCompressedInt(rd)];
          cMem.flags = ReadCompressedInt(rd);
          cMem.offset = ReadCompressedInt(rd);
          const uint32 index = ReadCompressedInt(rd);
          if (index) {
            cMem.type = &root->typeDescriptors.at(index - 1);
          }

          m = std::move(cMem);
        }
      }

      if (cDesc.layout[BodyLayoutFlags::Interfaces]) {
        const uint32 numInterfaces = ReadCompressedInt(rd);

        cDesc.interfaces.resize(numInterfaces);

        for (auto &i : cDesc.interfaces) {
          BodyInterface cint;
          const uint32 index = ReadCompressedInt(rd);
          cint.type = &root->typeDescriptors.at(index - 1);
          cint.flags = ReadCompressedInt(rd);

          i = std::move(cint);
        }
      }
    }
  }
};

template <> struct Read<CompileFourCC("TBDY")> : Read<CompileFourCC("TBOD")> {};

template <uint32 fourcc> constexpr auto make() {
  return std::make_pair(fourcc, Read<fourcc>::e);
}

template <uint32 fourcc> constexpr auto makeSkip() {
  return std::make_pair(fourcc, Read<1>::e);
}

static const std::map<uint32, decltype(&Read<0>::e)> hkChunkRegistry = {
    makeSkip<CompileFourCC("TAG0")>(), //
    makeSkip<CompileFourCC("TPTR")>(), //
    makeSkip<CompileFourCC("TPAD")>(), //
    makeSkip<CompileFourCC("THSH")>(), //
    make<CompileFourCC("TBOD")>(),     //
    make<CompileFourCC("TBDY")>(),     //
    make<CompileFourCC("TYPE")>(),     //
    make<CompileFourCC("INDX")>(),     //
    make<CompileFourCC("SDKV")>(),     //
    make<CompileFourCC("DATA")>(),     //
    make<CompileFourCC("TSTR")>(),     //
    make<CompileFourCC("FSTR")>(),     //
    make<CompileFourCC("TNAM")>(),     //
    make<CompileFourCC("ITEM")>(),     //
    make<CompileFourCC("PTCH")>(),     //
    make<CompileFourCC("TNA1")>(),     //
};

void hkChunk::Reorder() {
  FByteswapper(sizeAndFlags);
  sizeAndFlags = ((sizeAndFlags & 0xffffff) - 8) | (sizeAndFlags & 0xff000000);
}

void hkChunk::Read(BinReaderRef rd, hkxNewHeader *root) {
  rd.Read(*this);
  Reorder();

  if (!hkChunkRegistry.count(tag)) {
    auto dec = es::InvalidHeaderError::DecompileFourCC(tag, 4);
    printwarning("[Havok] Unhandled tag chunk: " << dec);
    rd.Skip(Size());
    return;
  }

  hkChunkRegistry.at(tag)(rd, this, root);
}

void hkxNewHeader::Load(BinReaderRef rd) {
  rd.Read(static_cast<hkChunk &>(*this));
  Reorder();

  if (tag != CompileFourCC("TAG0")) {
    throw es::InvalidHeaderError(tag);
  }

  const uint32 fileSize = Size();

  while (rd.Tell() < fileSize) {
    hkChunk curChunk;
    curChunk.Read(rd, this);
  }
}

std::ostream &operator<<(std::ostream &str, ClassDesc &desc) {
  if (desc.layout[BodyLayoutFlags::IsPointer] &&
      !desc.layout[BodyLayoutFlags::IsClass]) {
    str << *desc.pointer;

    if (!desc.flags[BodyFlags::InlineArray]) {
      str << '*';
    }
    return str;
  }

  str << desc.className;

  if (desc.templateArguments.size()) {
    str << '<';

    for (auto &t : desc.templateArguments) {
      if (t.argType) {
        str << *t.argType << ", ";
      } else {
        str << "void, ";
      }
    }

    str.seekp(-2, std::ios_base::cur);
    str << ">";
  }

  return str;
}

void hkxNewHeader::DumpClassDeclarationsAsCPP(std::ostream &str) const {
  // forward declaration
  for (auto &cl : typeDescriptors) {
    if (!cl.layout[BodyLayoutFlags::IsClass] || cl.templateArguments.empty()) {
      continue;
    }

    str << "template<";

    for (auto &t : cl.templateArguments) {
      str << "class " << t.argName << ", ";
    }

    str.seekp(-2, std::ios_base::cur);
    str << "> class " << cl.className << ";\n";
  }

  // type declaration
  size_t id = 0;
  for (auto &cl : typeDescriptors) {
    if (!cl.layout[BodyLayoutFlags::IsClass]) {
      // str << cl.className << ";\n";
      id++;
      continue;
    }

    str << "class " << cl.className;

    if (cl.templateArguments.size()) {
      str << '<';

      for (auto &t : cl.templateArguments) {
        if (t.argType) {
          str << *t.argType << ", ";
        } else {
          str << "void, ";
        }
      }

      str.seekp(-2, std::ios_base::cur);
      str << ">";
    }

    if (cl.base) {
      str << " : public " << *cl.base;
    }

    if (cl.interfaces.size()) {
      if (!cl.base) {
        str << " : ";
      } else {
        str << ", ";
      }

      for (auto &i : cl.interfaces) {
        str << "public " << *i.type << ", ";
      }

      str.seekp(-2, std::ios_base::cur);
    }

    str << " {";

    if (cl.members.size()) {
      str << "\n  ";

      for (auto &m : cl.members) {
        str << *m.type << ' ' << m.name;

        if (m.type->flags[BodyFlags::InlineArray]) {
          uint16 numItems = static_cast<uint32>(m.type->flags) >> 8;
          str << '[' << numItems << ']';
        }

        str << ";\n  ";
      }

      str.seekp(-2, std::ios_base::cur);
    }
    id++;
    str << "};\n\n";
  }
}

#include "datas/float.hpp"
#include <cinttypes>

static void XMLClass(const ClassDesc &type, const char *ptr,
                     pugi::xml_node node);
static void XMLSingleType(const ClassDesc &type, const char *data,
                          pugi::xml_node node);

static void XMLArray(const ClassDesc &type, const char *data, size_t numItems,
                     pugi::xml_node node) {
  auto aTypeName = type.className;
  std::string buffer;
  auto toString = [&](auto value) {
    for (size_t p = 0; p < numItems; p++, value++) {
      buffer += std::to_string(*value) + ' ';
    }
    node.append_buffer(buffer.data(), buffer.size());
  };

  auto toVector4 = [&buffer](auto value) {
    buffer += '(' + std::to_string(value->X) + ' ' + std::to_string(value->Y) +
              ' ' + std::to_string(value->Z) + ' ' + std::to_string(value->W) +
              ")";
  };

  auto toVector3 = [&buffer](auto value) {
    buffer += '(' + std::to_string(value->X) + ' ' + std::to_string(value->Y) +
              ' ' + std::to_string(value->Z) + ")";
  };

  if (aTypeName == "hkInt8") {
    toString(reinterpret_cast<const int8 *>(data));
  } else if (aTypeName == "hkUint8" || aTypeName == "unsigned char") {
    toString(reinterpret_cast<const uint8 *>(data));
  } else if (aTypeName == "hkInt16" || aTypeName == "short") {
    toString(reinterpret_cast<const int16 *>(data));
  } else if (aTypeName == "hkUint16" || aTypeName == "unsigned short") {
    toString(reinterpret_cast<const uint16 *>(data));
  } else if (aTypeName == "hkInt32" || aTypeName == "int") {
    toString(reinterpret_cast<const int32 *>(data));
  } else if (aTypeName == "hkUint32" || aTypeName == "unsigned int" ||
             aTypeName == "hknpConstraintId") {
    toString(reinterpret_cast<const uint32 *>(data));
  } else if (aTypeName == "hkInt64") {
    toString(reinterpret_cast<const int64 *>(data));
  } else if (aTypeName == "hkUint64" || aTypeName == "hkUlong") {
    toString(reinterpret_cast<const uint64 *>(data));
  } else if (aTypeName == "hkReal" || aTypeName == "float") {
    toString(reinterpret_cast<const float *>(data));
  } else if (aTypeName == "hkHalf16") {
    auto mtdc = reinterpret_cast<const uint16 *>(data);

    for (size_t p = 0; p < numItems; p++, mtdc++) {
      buffer += std::to_string(float16::ToFloat(*mtdc)) + ' ';
    }

    node.append_buffer(buffer.data(), buffer.size());
  } else if (aTypeName == "hkUFloat8") {
    using ufloat8 = esFloat<3, 5, false>;

    auto mtdc = reinterpret_cast<const uint8 *>(data);

    for (size_t p = 0; p < numItems; p++, mtdc++) {
      buffer += std::to_string(ufloat8::ToFloat(*mtdc)) + ' ';
    }

    node.append_buffer(buffer.data(), buffer.size());
  } else if (aTypeName == "hkVector4" || aTypeName == "hkVector4f" ||
             aTypeName == "hkQuaternion") {
    auto mtdc = reinterpret_cast<const Vector4 *>(data);

    for (size_t p = 0; p < numItems; p++) {
      toVector4(mtdc++);
      buffer.push_back('\n');
    }
    node.append_buffer(buffer.data(), buffer.size());
  } else if (aTypeName == "hkMatrix4") {
    auto mtdc = reinterpret_cast<const Vector4 *>(data);

    for (size_t p = 0; p < numItems; p++) {
      toVector4(mtdc++);
      toVector4(mtdc++);
      toVector4(mtdc++);
      toVector4(mtdc++);
      buffer.push_back('\n');
    }

    node.append_buffer(buffer.data(), buffer.size());
  } else if (aTypeName == "hkMatrix3") {
    auto mtdc = reinterpret_cast<const Vector4 *>(data);

    for (size_t p = 0; p < numItems; p++) {
      toVector3(mtdc++);
      toVector3(mtdc++);
      toVector3(mtdc++);
      buffer.push_back('\n');
    }

    node.append_buffer(buffer.data(), buffer.size());
  } else if (aTypeName == "hkQsTransform") {
    auto mtdc = reinterpret_cast<const Vector4 *>(data);

    for (size_t p = 0; p < numItems; p++) {
      toVector3(mtdc++);
      toVector4(mtdc++);
      toVector3(mtdc++);
      buffer.push_back('\n');
    }

    node.append_buffer(buffer.data(), buffer.size());
  } else if (aTypeName == "hkTransform") {
    auto mtdc = reinterpret_cast<const Vector4 *>(data);

    for (size_t p = 0; p < numItems; p++) {
      toVector3(mtdc++);
      toVector3(mtdc++);
      toVector3(mtdc++);
      toVector3(mtdc++);
      buffer.push_back('\n');
    }

    node.append_buffer(buffer.data(), buffer.size());
  } else if (aTypeName == "hkRefPtr" || aTypeName == "hkRefVariant") {
    auto mtdc = reinterpret_cast<const char *const *>(data);

    for (size_t p = 0; p < numItems; p++) {
      auto aPtr = *mtdc;
      if (!aPtr) {
        buffer += "null ";
      } else {
        buffer += std::to_string(reinterpret_cast<uintptr_t>(aPtr)) + ' ';
      }
      mtdc++;
    }

    node.append_buffer(buffer.data(), buffer.size());
  } else if (aTypeName == "hkStringPtr") {
    auto mtdc = reinterpret_cast<const char *const *>(data);

    for (size_t p = 0; p < numItems; p++, mtdc++) {
      auto aNode = node.append_child("hkcstring");
      auto aPtr = *mtdc;
      if (aPtr) {
        es::string_view strng(aPtr);
        aNode.append_buffer(strng.data(), strng.size());
      }
    }
  } else {
    if (!type.size) {
      throw std::runtime_error("size is 0");
    }

    for (size_t p = 0; p < numItems; p++) {
      XMLSingleType(type, data + type.size * p, node);
    }
  }
}

static void XMLSingleType(const ClassDesc &type, const char *data,
                          pugi::xml_node node) {
  char strBuffer[0x40]{};
  auto typeName = type.className;

  auto toString = [&](auto val, auto fmt) {
    auto wrchars = snprintf(strBuffer, sizeof(strBuffer), fmt, *val);
    node.append_buffer(strBuffer, wrchars);
  };

  auto toVector4 = [](auto value) {
    return '(' + std::to_string(value->X) + ' ' + std::to_string(value->Y) +
           ' ' + std::to_string(value->Z) + ' ' + std::to_string(value->W) +
           ')';
  };

  auto toVector3 = [](auto value) {
    return '(' + std::to_string(value->X) + ' ' + std::to_string(value->Y) +
           ' ' + std::to_string(value->Z) + ')';
  };

  if (typeName == "hkInt8") {
    toString(reinterpret_cast<const int8 *>(data), "%" PRIi8);
  } else if (typeName == "hkUint8" || typeName == "unsigned char") {
    toString(reinterpret_cast<const uint8 *>(data), "%" PRIu8);
  } else if (typeName == "hkInt16" || typeName == "short") {
    toString(reinterpret_cast<const int16 *>(data), "%" PRIi16);
  } else if (typeName == "hkUint16" || typeName == "unsigned short") {
    toString(reinterpret_cast<const uint16 *>(data), "%" PRIu16);
  } else if (typeName == "hkInt32" ||
             typeName == "int") //||
                                // typeName == "hknpConstraintId")
  {
    toString(reinterpret_cast<const int32 *>(data), "%" PRIi32);
  } else if (typeName == "hkUint32" || typeName == "unsigned int") {
    toString(reinterpret_cast<const uint32 *>(data), "%" PRIu32);
  } else if (typeName == "hkInt64") {
    toString(reinterpret_cast<const int64 *>(data), "%" PRIi64);
  } else if (typeName == "hkUint64" || typeName == "hkUlong") {
    toString(reinterpret_cast<const uint64 *>(data), "%" PRIu64);
  } else if (typeName == "hkReal" || typeName == "float") {
    toString(reinterpret_cast<const float *>(data), "%f");
  } else if (typeName == "hkHalf16") {
    uint16 encoded = *reinterpret_cast<const uint16 *>(data);
    float cvted = float16::ToFloat(encoded);
    toString(&cvted, "%f");
  } else if (typeName == "hkUFloat8") {
    using ufloat8 = esFloat<3, 5, false>;
    uint8 encoded = *reinterpret_cast<const uint8 *>(data);
    float cvted = ufloat8::ToFloat(encoded);
    toString(&cvted, "%f");
  } else if (typeName == "hkVector4" || typeName == "hkVector4f" ||
             typeName == "hkQuaternion") {
    auto mtd = reinterpret_cast<const Vector4 *>(data);
    std::string buffer = '(' + std::to_string(mtd->X) + ' ' +
                         std::to_string(mtd->Y) + ' ' + std::to_string(mtd->Z) +
                         ' ' + std::to_string(mtd->W) + ')';
    node.append_buffer(buffer.data(), buffer.size());
  } else if (typeName == "hkMatrix4") {
    auto mtdc = reinterpret_cast<const Vector4 *>(data);
    std::string buffer = toVector4(mtdc++);
    buffer += toVector4(mtdc++);
    buffer += toVector4(mtdc++);
    buffer += toVector4(mtdc++);

    node.append_buffer(buffer.data(), buffer.size());
  } else if (typeName == "hkQsTransform") {
    auto mtdc = reinterpret_cast<const Vector4 *>(data);
    std::string buffer = toVector3(mtdc++);
    buffer += toVector4(mtdc++);
    buffer += toVector3(mtdc++);

    node.append_buffer(buffer.data(), buffer.size());
  } else if (typeName == "hkTransform") {
    auto mtdc = reinterpret_cast<const Vector4 *>(data);
    std::string buffer = toVector3(mtdc++);
    buffer += toVector3(mtdc++);
    buffer += toVector3(mtdc++);
    buffer += toVector3(mtdc++);

    node.append_buffer(buffer.data(), buffer.size());
  } else if (typeName == "hkMatrix3") {
    auto mtdc = reinterpret_cast<const Vector4 *>(data);
    std::string buffer = toVector3(mtdc++);
    buffer += toVector3(mtdc++);
    buffer += toVector3(mtdc++);

    node.append_buffer(buffer.data(), buffer.size());
  } else if (typeName == "hkEnum" || typeName == "hkFlags") {
    XMLSingleType(*type.templateArguments[1].argType, data, node);
  } else if (typeName == "hkRefPtr" || typeName == "hkRefVariant") {
    auto aPtr = *reinterpret_cast<const char *const *>(data);
    if (!aPtr) {
      node.append_buffer("null", 4);
    } else {
      char strBuffer[0x40]{};
      auto wrchars = snprintf(strBuffer, sizeof(strBuffer), "%p", aPtr);
      node.append_buffer(strBuffer, wrchars);
    }
  } else if (typeName == "hkStringPtr") {
    auto aPtr = *reinterpret_cast<const char *const *>(data);
    if (aPtr) {
      es::string_view strng(aPtr);
      node.append_buffer(strng.data(), strng.size());
    }
  } else if (typeName == "hkBool" || typeName == "bool") {
    bool isTrue = *reinterpret_cast<const bool *>(data);
    node.append_buffer(isTrue ? "true" : "false", 4 + !isTrue);
  } else if (typeName == "hkArray") {
    auto aType = type.pointer;
    auto mtd = reinterpret_cast<const hkRealArray<char> *>(data);
    node.append_attribute("numelements").set_value(mtd->count);
    XMLArray(*aType, mtd->data, mtd->count, node);
  } else if (typeName == "hkRelArray") {
    auto aType = type.pointer;
    auto mtd = reinterpret_cast<const uint16 *>(data);
    node.append_attribute("numelements").set_value(mtd[1]);
    XMLArray(*aType, reinterpret_cast<const char *>(data) + *mtd, mtd[1], node);
  } else if (type.layout[BodyLayoutFlags::IsPointer]) {
    if (type.flags[BodyFlags::InlineArray]) {
      uint16 numItems = static_cast<uint32>(type.flags) >> 8;
      XMLArray(*type.pointer, data, numItems, node);
    } else {
      auto aPtr = *reinterpret_cast<const char *const *>(data);
      if (!aPtr) {
        node.append_buffer("null", 4);
      } else {
        char strBuffer[0x40]{};
        auto wrchars = snprintf(strBuffer, sizeof(strBuffer), "%p", aPtr);
        node.append_buffer(strBuffer, wrchars);
      }
    }
  } else if (type.layout[BodyLayoutFlags::IsClass] || type.base) {
    auto aNode = node.append_child("hkobject");
    XMLClass(type, data, aNode);
  } else {
    std::string buffer("UNKTYPE: ");
    buffer += typeName;
    node.append_buffer(buffer.data(), buffer.size());
  }
}

static void XMLClass(const ClassDesc &type, const char *ptr,
                     pugi::xml_node node) {
  if (type.base) {
    XMLClass(*type.base, ptr, node);
  }

  for (auto &m : type.members) {
    if (m.flags[MemberFlags::IgnoreSerialization]) {
      auto cmnt = node.append_child(pugi::xml_node_type::node_comment);
      cmnt.set_value(m.name.data());
      continue;
    }

    auto data = ptr + m.offset;
    auto mNode = node.append_child("hkparam");
    auto typeName = m.type->className;
    mNode.append_attribute("name").set_value(m.name.data());

    XMLSingleType(*m.type, data, mNode);
  }
}

void hkxNewHeader::ToXML(pugi::xml_node node) const {
  node = node.append_child("hkpackfile");
  node = node.append_child("hksection");
  node.append_attribute("name").set_value("__data__");

  for (auto &f : classEntries) {
    const int32 clsID = f.Size() - 1;

    if (clsID < 0) {
      continue;
    }

    auto &type = typeDescriptors[clsID];

    // 0x10000000 - virtual class
    // 0x20000000 - pointer
    if (f.sizeAndFlags & 0x10000000) {
      const char *ptr = dataBuffer.data() + f.tag;
      auto cNode = node.append_child("hkobject");
      char strBuffer[0x40]{};
      snprintf(strBuffer, sizeof(strBuffer), "%p", ptr);
      cNode.append_attribute("name").set_value(strBuffer);
      cNode.append_attribute("class").set_value(type.className.data());

      XMLClass(type, ptr, cNode);
    }
  }
}
