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
#include "datas/macroLoop.hpp"
#include "datas/master_printer.hpp"
#include "internal/hk_internal_api.hpp"
#include <map>

struct _chunkCC {
  chunkCC c;
  _chunkCC() = default;
  _chunkCC(_chunkCC input, char item, uint32 index) : c(input.c) {
    c.fourCC[index] = item;
  }
};

constexpr uint32 _ToFourCC(const char *input, const _chunkCC chnk = {},
                           uint32 index = 2) {
  return index > 5 ? chnk.c.hash
                   : _ToFourCC(input, _chunkCC(chnk, input[index], index - 2),
                               index + 1);
}

#define classFourCC(iname) _ToFourCC(#iname)

int chSDKVRead(BinReader *rd, hkChunk *, hkxNewHeader *root) {
  if (!root)
    return 1;

  char buff[8];

  rd->Read(buff);
  buff[4] = 0;

  uint32 version = atoi(buff);

  auto convert = [&]() {
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
    printerror("[Havok] Unknown toolset version: " << buff);
  }

  return 0;
}

int chDATARead(BinReader *rd, hkChunk *holder, hkxNewHeader *root) {
  if (!holder || !root)
    return 1;

  const uint32 dataSize = holder->Size();
  rd->ReadContainer(root->dataBuffer, dataSize);

  return 0;
}

int ReadTypeNames(BinReader *rd, hkChunk *holder, hkxNewHeader *root,
                  std::string &outBuffer, _clVec &outVec) {
  if (!holder || !root)
    return 1;

  const uint32 bufferSize = holder->Size();
  rd->ReadContainer(outBuffer, bufferSize);
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

  return 0;
}

int chTSTRRead(BinReader *rd, hkChunk *holder, hkxNewHeader *root) {
  return ReadTypeNames(rd, holder, root, root->classNamesBuffer,
                       root->classNames);
}

int chFSTRRead(BinReader *rd, hkChunk *holder, hkxNewHeader *root) {
  return ReadTypeNames(rd, holder, root, root->memberNamesBuffer,
                       root->memberNames);
}

int32 ReadCompressedInt(BinReader *rd) {
  uint8 firstInt;
  int32 resultInt = 0;

  rd->Read(firstInt);

  const bool flag1 = (firstInt & 0x80) == 0x80;
  const bool flag2 = (firstInt & 0xC0) == 0xC0;
  const bool flag3 = (firstInt & 0xE0) == 0xE0;

  if (flag3) {
    rd->Read(resultInt);
    resultInt |= (firstInt & 0xf) << 4;
  } else if (flag2) {
    printerror("[Havok] Unhandled int compression : 0xC0!")
  } else if (flag1) {
    uint8 secondInt;
    rd->Read(secondInt);
    resultInt = secondInt | ((static_cast<int32>(firstInt) & 0xf) << 8);
  } else
    resultInt = firstInt;

  return resultInt;
}

int chTNAMRead(BinReader *rd, hkChunk *holder, hkxNewHeader *root) {
  if (!holder || !root)
    return 1;

  const size_t savepos = rd->Tell();
  const uint32 numClasses = ReadCompressedInt(rd) - 1;

  root->weldedClassNames.resize(numClasses);

  for (auto &c : root->weldedClassNames) {
    const uint32 classNameIndex = ReadCompressedInt(rd);
    c.className = root->classNames[classNameIndex];

    uint8 numTemplateArgs;
    rd->Read(numTemplateArgs);
    c.templateArguments.resize(numTemplateArgs);

    for (auto &t : c.templateArguments) {
      uint32 argNameIndex = ReadCompressedInt(rd);
      t.argName = root->classNames[argNameIndex];

      argNameIndex = ReadCompressedInt(rd);
      t.argType = &root->weldedClassNames[argNameIndex - 1];
    }
  }

  const int32 diff = static_cast<int32>(holder->Size()) -
                     static_cast<int32>(rd->Tell() - savepos);

  if (diff < 0) {
    printwarning("[Havok] TNAM chunk read too much data, possible incorrect "
                 "class map, proceeding to next chunk.");
  }

  rd->Skip(diff);

  return 0;
}

int chTNA1Read(BinReader *rd, hkChunk *holder, hkxNewHeader *root) {
  return chTNAMRead(rd, holder, root);
}

int chITEMRead(BinReader *rd, hkChunk *holder, hkxNewHeader *root) {
  if (!holder || !root)
    return 1;

  const uint32 numFixups = holder->Size() / sizeof(classEntryFixup);

  rd->ReadContainer(root->classEntries, numFixups);

  return 0;
}

int chPTCHRead(BinReader *rd, hkChunk *holder, hkxNewHeader *root) {
  if (!holder || !root)
    return 1;

  if (!root->weldedClassNames.size()) {
    printerror("[Havok] File is missing type infos.");
    return 2;
  }

  const size_t endPos = rd->Tell() + holder->Size();

  while (rd->Tell() < endPos) {
    uint32 classNameIndex;
    rd->Read(classNameIndex);

    es::string_view toclassname =
        root->weldedClassNames[classNameIndex - 1].className;

    bool isHKArray = toclassname == "hkArray";
    bool isHKRelArray = toclassname == "hkRelArray";

    uint32 numPointers;
    rd->Read(numPointers);

    for (uint32 t = 0; t < numPointers; t++) {
      uint32 cPointer;
      rd->Read(cPointer);

      if (isHKRelArray) {
        uint32 *retarget =
            reinterpret_cast<uint32 *>(&root->dataBuffer[0] + cPointer);
        const classEntryFixup &xfix = root->classEntries[*retarget];

        uint16 *relRetarget = reinterpret_cast<uint16 *>(retarget);
        *relRetarget = xfix.tag.hash - cPointer;
        *(relRetarget + 1) = xfix.count;
      } else {
        uint64 *retarget =
            reinterpret_cast<uint64 *>(&root->dataBuffer[0] + cPointer);
        const classEntryFixup &xfix = root->classEntries[*retarget];

        *retarget =
            reinterpret_cast<uint64>(xfix.tag.hash + root->dataBuffer.data());

        if (isHKArray)
          *(retarget + 1) = xfix.count;
      }
    }
  }

  for (auto &f : root->classEntries) {
    const int32 clsID = f.Size() - 1;

    if (clsID < 0)
      continue;

    es::string_view clName = root->weldedClassNames[clsID].className;
    const JenHash _chash = JenkinsHash(clName);
    CRule rule(root->toolset, false, 8); // No way to detect so far
    hkVirtualClass *cls = hkVirtualClass::Create(_chash, rule);

    if (cls) {
      cls->SetDataPointer(&root->dataBuffer[0] + f.tag.hash);
      cls->className = clName;
      cls->AddHash(clName);
      cls->header = root;
      root->virtualClasses.emplace_back(cls);
      cls->Process();
    }
  }

  auto t0 = std::move(root->classEntries);

  return 0;
}

int ReadBlankChunk(BinReader *rd, hkChunk *holder, hkxNewHeader *) {
  if (!holder)
    return 1;

  rd->Skip(holder->Size());
  return 0;
}

int ReadBlankChunkNoSkip(BinReader *, hkChunk *, hkxNewHeader *) { return 0; }

#define chunkRegister(value) {_ToFourCC(#value), value},
#define chunkRegisterBlank(value) {_ToFourCC(#value), ReadBlankChunk},
#define chunkRegisterBlankNoskip(value)                                        \
  {_ToFourCC(#value), ReadBlankChunkNoSkip},

static const std::map<const uint32,
                      int (*)(BinReader *, hkChunk *, hkxNewHeader *)>
    hkChunkRegistry = {
        StaticFor(chunkRegister, chSDKVRead, chDATARead, chTSTRRead, chFSTRRead,
                  chTNAMRead, chITEMRead, chPTCHRead, chTNA1Read)
            StaticFor(chunkRegisterBlank, chTAG0, chTPTR, chTPAD, chTHSH)
                StaticFor(chunkRegisterBlankNoskip, chTYPE, chINDX)};

#define hkRegisterClass(classname)                                             \
  hkChunkRegistry[classname::HASH] = &hkCreateChunk<classname>;

void hkChunk::Reorder() {
  FByteswapper(sizeAndFlags);
  sizeAndFlags =
    ((sizeAndFlags & 0xffffff) - 8) | (sizeAndFlags & 0xff000000);
}

int hkChunk::Read(BinReader *rd, hkxNewHeader *root) {
  rd->Read(*this);
  Reorder();

  if (!hkChunkRegistry.count(tag.hash)) {
    printwarning("[Havok] Unhandled tag chunk: "
                 << tag.fourCC[0] << tag.fourCC[1] << tag.fourCC[2]
                 << tag.fourCC[3]);
    rd->Skip(Size());
    return 0;
  }

  if (hkChunkRegistry.at(tag.hash)(rd, this, root)) {
    printerror("[Havok] Error reading chunk: " << tag.fourCC[0] << tag.fourCC[1]
                                               << tag.fourCC[2]
                                               << tag.fourCC[3]);
    return 1;
  }

  return 0;
}

int hkxNewHeader::Load(BinReader *rd) {
  rd->Read(static_cast<hkChunk &>(*this));
  Reorder();

  if (tag != classFourCC(chTAG0)) {
    printerror("[Havok] Invalid packfile.");
    return 1;
  }

  const uint32 fileSize = Size();

  while (rd->Tell() < fileSize) {
    hkChunk curChunk;

    if (curChunk.Read(rd, this))
      return 1;
  }

  return 0;
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
