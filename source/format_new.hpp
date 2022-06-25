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

#pragma once
#include "datas/bincore_fwd.hpp"
#include "hklib/hk_packfile.hpp"

#define hkHederTAG 0x30474154

struct hkxNewHeader;

struct hkChunk {
  uint32 sizeAndFlags;
  uint32 tag;

  void Reorder();
  uint32 Size() const { return sizeAndFlags & 0xffffff; }
  bool IsSubChunk() const { return (sizeAndFlags & 0x40000000) != 0; }
  void Read(BinReaderRef rd, hkxNewHeader *root);
};

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
  using StrVec = std::vector<es::string_view>;
  hkToolset toolset;
  std::string dataBuffer;
  std::string classNamesBuffer;
  std::string memberNamesBuffer;

  StrVec classNames;
  StrVec memberNames;
  std::vector<ClassName> weldedClassNames;
  std::vector<classEntryFixup> classEntries;
  VirtualClasses virtualClasses;

  VirtualClasses &GetAllClasses() override { return virtualClasses; }
  hkToolset GetToolset() const override { return toolset; }
  void Load(BinReaderRef rd);
  void DumpClassNames(std::ostream &str);
};
