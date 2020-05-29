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

#pragma once
#include "datas/endian.hpp"

#include "hkInternalInterfaces.h"

#define hkHederTAG 0x30474154

class BinReader;
struct hkxNewHeader;

union chunkCC {
  char fourCC[4];
  uint32 hash;
  bool operator==(uint32 input) { return input == hash; }
  bool operator!=(uint32 input) { return input != hash; }
};

class hkChunk {
  uint32 sizeAndFlags;

public:
  chunkCC tag;
  void Reorder() {
    FByteswapper(sizeAndFlags);
    sizeAndFlags = ((sizeAndFlags & 0xffffff) - 8) | sizeAndFlags & 0xff000000;
  }
  uint32 Size() const { return sizeAndFlags & 0xffffff; }
  bool IsSubChunk() const {
    return (sizeAndFlags & 0x40000000) != 0;
  }
  int Read(BinReader *rd, hkxNewHeader *root);
};

typedef std::vector<es::string_view> _clVec;
struct ClassName;

struct ClassTemplateArgument {
  es::string_view argName;
  ClassName *argType;
};

struct ClassName {
  es::string_view className;
  std::vector<ClassTemplateArgument> templateArguments;
};

struct classEntryFixup : hkChunk {
  uint32 count;
};

struct hkxNewHeader : IhkPackFile, hkChunk {
  char contentsVersionStripped[5];
  std::string dataBuffer;
  std::string classNamesBuffer;
  std::string memberNamesBuffer;

  _clVec classNames;
  _clVec memberNames;
  std::vector<ClassName> weldedClassNames;
  std::vector<classEntryFixup> classEntries;
  VirtualClasses virtualClasses;

  VirtualClasses &GetAllClasses() override { return virtualClasses; }
  int32 GetVersion() const override;
  int Load(BinReader *rd);
  void DumpClassNames(std::ostream &str);
};