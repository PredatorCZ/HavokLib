/*  Havok Format Library
    Copyright(C) 2016-2022 Lukas Cone

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
#include "datas/binwritter.hpp"
#include "datas/except.hpp"
#include "datas/master_printer.hpp"
#include "internal/hk_internal_api.hpp"
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
    resultInt |= (firstInt & 0xf) << 4;
  } else if (flag2) {
    throw std::logic_error("Unhandled int compression : 0xC0!");
  } else if (flag1) {
    uint8 secondInt;
    rd.Read(secondInt);
    resultInt = secondInt | ((static_cast<int32>(firstInt) & 0xf) << 8);
  } else
    resultInt = firstInt;

  return resultInt;
}

template <> struct Read<CompileFourCC("TNAM")> {
  static auto e(BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
    PtrGuard(holder);
    PtrGuard(root);

    const size_t savepos = rd.Tell();
    const uint32 numClasses = ReadCompressedInt(rd) - 1;

    root->weldedClassNames.resize(numClasses);

    for (auto &c : root->weldedClassNames) {
      const uint32 classNameIndex = ReadCompressedInt(rd);
      c.className = root->classNames[classNameIndex];

      uint8 numTemplateArgs;
      rd.Read(numTemplateArgs);
      c.templateArguments.resize(numTemplateArgs);

      for (auto &t : c.templateArguments) {
        uint32 argNameIndex = ReadCompressedInt(rd);
        t.argName = root->classNames[argNameIndex];

        argNameIndex = ReadCompressedInt(rd);
        t.argType = &root->weldedClassNames[argNameIndex - 1];
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

    if (!root->weldedClassNames.size()) {
      throw std::runtime_error("File is missing type infos.");
    }

    const size_t endPos = rd.Tell() + holder->Size();

    while (rd.Tell() < endPos) {
      uint32 classNameIndex;
      rd.Read(classNameIndex);

      es::string_view toclassname =
          root->weldedClassNames[classNameIndex - 1].className;

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

      es::string_view clName = root->weldedClassNames[clsID].className;
      const JenHash chash(clName);
      CRule rule(root->toolset, false, 8); // No way to detect so far
      IhkVirtualClass *clsn = hkVirtualClass::Create(chash, rule);
      auto cls = const_cast<hkVirtualClass *>(safe_deref_cast<const hkVirtualClass>(clsn));

      if (cls) {
        cls->SetDataPointer(&root->dataBuffer[0] + f.tag);
        cls->className = clName;
        cls->AddHash(clName);
        cls->header = root;
        root->virtualClasses.emplace_back(clsn);
        cls->Process();
      }
    }

    es::Dispose(root->classEntries);
  };
};

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
    makeSkip<CompileFourCC("TBOD")>(), // TODO
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

void hkxNewHeader::DumpClassNames(std::ostream &str) {
  for (auto &cl : weldedClassNames) {
    str << cl.className;

    if (cl.templateArguments.size()) {
      str << '<';

      for (auto &t : cl.templateArguments) {
        str << "typename " << t.argName << " = " << t.argType->className
            << ", ";
      }

      str.seekp(-2, std::ios_base::cur);
      str << '>';
    }

    str << '\n';
  }
}
