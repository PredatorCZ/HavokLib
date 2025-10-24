/*  Havok Format Library
    Copyright(C) 2016-2025 Lukas Cone

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
#include "internal/hk_internal_api.hpp"
#include "spike/except.hpp"
#include "spike/io/binreader.hpp"
#include "spike/io/binwritter.hpp"
#include "spike/master_printer.hpp"
#include <map>

template <class C> void PtrGuard(const C *val) {
  if (!val) {
    throw std::runtime_error("nullptr detected");
  }
}

using ReadFunc = void (*)(BinReaderRef, hkChunk *, hkxNewHeader *);

template <uint32 tag>
ReadFunc Read = [](BinReaderRef, hkChunk *, hkxNewHeader *) {
  throw std::logic_error("Unimplemented function");
};

template <>
ReadFunc Read<1> = [](BinReaderRef rd, hkChunk *holder, hkxNewHeader *) {
  PtrGuard(holder);

  rd.Skip(holder->Size());
};

using ReadCompFunc = void (*)(BinReaderRef, hkChunk *, hkCompendiumData *);

template <uint32 tag>
ReadCompFunc ReadComp = [](BinReaderRef, hkChunk *, hkCompendiumData *) {
  throw std::logic_error("Unimplemented function");
};

template <>
ReadCompFunc ReadComp<1> =
    [](BinReaderRef rd, hkChunk *holder, hkCompendiumData *) {
      PtrGuard(holder);

      rd.Skip(holder->Size());
    };

template <>
ReadFunc Read<CompileFourCC("INDX")> =
    [](BinReaderRef, hkChunk *, hkxNewHeader *) {};

template <>
ReadCompFunc ReadComp<CompileFourCC("TYPE")> =
    [](BinReaderRef, hkChunk *, hkCompendiumData *) {};

template <>
ReadFunc Read<CompileFourCC("SDKV")> =
    [](BinReaderRef rd, hkChunk *, hkxNewHeader *root) {
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
        case 2018:
          return HK2018;
        case 2019:
          return HK2019;
        default:
          return HKUNKVER;
        }
      };

      root->toolset = convert();

      if (root->toolset == HKUNKVER) {
        throw es::InvalidVersionError(version);
      }
    };

template <>
ReadFunc Read<CompileFourCC("DATA")> =
    [](BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
      PtrGuard(holder);
      PtrGuard(root);

      const uint32 dataSize = holder->Size();
      rd.ReadContainer(root->dataBuffer, dataSize);
    };

void ReadTypeNames(BinReaderRef rd, hkChunk *holder, std::string &outBuffer,
                   hkCompendiumData::StrVec &outVec) {
  PtrGuard(holder);

  const uint32 bufferSize = holder->Size();
  rd.ReadContainer(outBuffer, bufferSize);
  char *buffer = &outBuffer[0];
  std::string_view lastName = buffer;

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

template <>
ReadCompFunc ReadComp<CompileFourCC("TSTR")> =
    [](BinReaderRef rd, hkChunk *holder, hkCompendiumData *root) {
      ReadTypeNames(rd, holder, root->classNamesBuffer, root->classNames);
    };

template <>
ReadCompFunc ReadComp<CompileFourCC("TST1")> = ReadComp<CompileFourCC("TSTR")>;

template <>
ReadCompFunc ReadComp<CompileFourCC("FSTR")> =
    [](BinReaderRef rd, hkChunk *holder, hkCompendiumData *root) {
      ReadTypeNames(rd, holder, root->memberNamesBuffer, root->memberNames);
    };

template <>
ReadCompFunc ReadComp<CompileFourCC("FST1")> = ReadComp<CompileFourCC("FSTR")>;

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

template <>
ReadCompFunc ReadComp<CompileFourCC("TNAM")> = [](BinReaderRef rd,
                                                  hkChunk *holder,
                                                  hkCompendiumData *root) {
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

template <>
ReadCompFunc ReadComp<CompileFourCC("TNA1")> = ReadComp<CompileFourCC("TNAM")>;

template <>
ReadCompFunc ReadComp<CompileFourCC("ITEM")> =
    [](BinReaderRef rd, hkChunk *holder, hkCompendiumData *root) {
      PtrGuard(holder);
      PtrGuard(root);

      const uint32 numFixups = holder->Size() / sizeof(classEntryFixup);

      rd.ReadContainer(root->classEntries, numFixups);
    };

template <>
ReadFunc Read<CompileFourCC("PTCH")> =
    [](BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
      PtrGuard(holder);
      PtrGuard(root);
      hkCompendiumData &cData = root->Compendium();

      if (!cData.weldedClassNames.size()) {
        throw std::runtime_error("File is missing type infos.");
      }

      const size_t endPos = rd.Tell() + holder->Size();

      while (rd.Tell() < endPos) {
        uint32 classNameIndex;
        rd.Read(classNameIndex);

        std::string_view toclassname =
            cData.weldedClassNames[classNameIndex - 1].className;

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
            const classEntryFixup &xfix = cData.classEntries[*retarget];

            uint16 *relRetarget = reinterpret_cast<uint16 *>(retarget);
            *relRetarget = xfix.tag - cPointer;
            *(relRetarget + 1) = xfix.count;
          } else {
            uint64 *retarget =
                reinterpret_cast<uint64 *>(&root->dataBuffer[0] + cPointer);
            const classEntryFixup &xfix = cData.classEntries[*retarget];

            *retarget =
                reinterpret_cast<uint64>(xfix.tag + root->dataBuffer.data());

            if (isHKArray) {
              *(retarget + 1) = xfix.count;
            }
          }
        }
      }

      for (auto &f : cData.classEntries) {
        const int32 clsID = f.Size() - 1;

        if (clsID < 0) {
          continue;
        }

        std::string_view clName = cData.weldedClassNames[clsID].className;
        const JenHash chash(clName);
        CRule rule(root->toolset, false, 8); // No way to detect so far
        IhkVirtualClass *clsn = hkVirtualClass::Create(chash, rule);
        auto cls = const_cast<hkVirtualClass *>(
            safe_deref_cast<const hkVirtualClass>(clsn));

        if (cls) {
          cls->SetDataPointer(&root->dataBuffer[0] + f.tag);
          cls->className = clName;
          cls->AddHash(clName);
          cls->header = root;
          root->virtualClasses.emplace_back(clsn);
          cls->Process();
        }
      }

      es::Dispose(cData.classEntries);
    };

template <>
ReadFunc Read<CompileFourCC("TCRF")> =
    [](BinReaderRef rd, hkChunk *holder, hkxNewHeader *root) {
      PtrGuard(holder);
      PtrGuard(root);

      const size_t savepos = rd.Tell();

      uint64 signature;
      rd.Read(signature);

      const int32 diff = static_cast<int32>(holder->Size()) -
                         static_cast<int32>(rd.Tell() - savepos);
      rd.Skip(diff);

      if (!std::holds_alternative<hkCompendium *>(root->compendium)) {
        throw std::runtime_error("no external compendium to be referenced");
      }

      hkCompendium *comp = std::get<hkCompendium *>(root->compendium);
      if (!comp->signatures.contains(signature)) {
        throw std::runtime_error("external compendium signature mismatch");
      }
    };

template <uint32 fourcc> constexpr auto make() {
  return std::make_pair(fourcc, Read<fourcc>);
}

template <uint32 fourcc> constexpr auto makeSkip() {
  return std::make_pair(fourcc, Read<1>);
}

static const std::map<uint32, ReadFunc> hkChunkRegistry = {
    makeSkip<CompileFourCC("TAG0")>(), //
    make<CompileFourCC("INDX")>(),     //
    make<CompileFourCC("SDKV")>(),     //
    make<CompileFourCC("DATA")>(),     //
    make<CompileFourCC("PTCH")>(),     //
    make<CompileFourCC("TCRF")>(),     //
};

template <uint32 fourcc> constexpr auto makeComp() {
  return std::make_pair(fourcc, ReadComp<fourcc>);
}

template <uint32 fourcc> constexpr auto makeCompSkip() {
  return std::make_pair(fourcc, ReadComp<1>);
}

static const std::map<uint32, ReadCompFunc> hkCompChunkRegistry = {
    makeCompSkip<CompileFourCC("TPTR")>(), //
    makeCompSkip<CompileFourCC("TPAD")>(), //
    makeCompSkip<CompileFourCC("THSH")>(), //
    makeCompSkip<CompileFourCC("TBOD")>(), // TODO
    makeCompSkip<CompileFourCC("TBDY")>(), // TODO
    makeComp<CompileFourCC("TYPE")>(),     //
    makeComp<CompileFourCC("TSTR")>(),     //
    makeComp<CompileFourCC("FSTR")>(),     //
    makeComp<CompileFourCC("TNAM")>(),     //
    makeComp<CompileFourCC("TST1")>(),     //
    makeComp<CompileFourCC("FST1")>(),     //
    makeComp<CompileFourCC("TNA1")>(),     //
    makeComp<CompileFourCC("ITEM")>(),     //
};

void hkChunk::Reorder() {
  FByteswapper(sizeAndFlags);
  sizeAndFlags = ((sizeAndFlags & 0xffffff) - 8) | (sizeAndFlags & 0xff000000);
}

void hkxNewHeader::Load(BinReaderRef rd) {
  hkChunk hdr;
  rd.Read(hdr);
  hdr.Reorder();

  if (hdr.tag != HK_HEADER_TAG) {
    throw es::InvalidHeaderError(hdr.tag);
  }

  const uint32 fileSize = hdr.Size();

  while (rd.Tell() < fileSize) {
    hkChunk chunk;
    rd.Read(chunk);
    chunk.Reorder();

    if (hkCompChunkRegistry.count(chunk.tag)) {
      hkCompChunkRegistry.at(chunk.tag)(rd, &chunk, &Compendium());
      continue;
    }

    if (!hkChunkRegistry.count(chunk.tag)) {
      auto dec = es::InvalidHeaderError::DecompileFourCC(chunk.tag, 4);
      printwarning("[Havok] Unhandled tag chunk: " << dec);
      rd.Skip(chunk.Size());
      return;
    }

    hkChunkRegistry.at(chunk.tag)(rd, &chunk, this);
  }
}

void hkxNewHeader::DumpClassNames(std::ostream &str) {
  auto &cData = Compendium();

  for (auto &cl : cData.weldedClassNames) {
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

void hkCompendium::Load(BinReaderRef rd) {
  hkChunk hdr;
  rd.Read(hdr);
  hdr.Reorder();

  if (hdr.tag != HK_HEADER_TCM) {
    throw es::InvalidHeaderError(hdr.tag);
  }

  {
    hkChunk chunk;
    rd.Read(chunk);
    chunk.Reorder();

    if (chunk.tag != CompileFourCC("TCID")) {
      throw es::InvalidHeaderError(chunk.tag);
    }

    const uint32 numSignatures = chunk.Size() / sizeof(uint64);
    std::vector<uint64> signatures;

    rd.ReadContainer(signatures, numSignatures);
    this->signatures = {signatures.begin(), signatures.end()};
  }

  const uint32 fileSize = hdr.Size();

  while (rd.Tell() < fileSize) {
    hkChunk chunk;
    rd.Read(chunk);
    chunk.Reorder();

    if (!hkCompChunkRegistry.count(chunk.tag)) {
      auto dec = es::InvalidHeaderError::DecompileFourCC(chunk.tag, 4);
      printwarning("[Havok] Unhandled tag chunk: " << dec);
      rd.Skip(chunk.Size());
      return;
    }

    hkCompChunkRegistry.at(chunk.tag)(rd, &chunk, this);
  }
}
