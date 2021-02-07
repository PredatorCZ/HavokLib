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
#include "datas/bincore_fwd.hpp"
#include "datas/flags.hpp"
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

struct ClassDesc;

struct ClassTemplateArgument {
  es::string_view argName;
  ClassDesc *argType = nullptr;
};

enum class BodyLayoutFlags : uint16 {
  Flags,
  IsPointer,
  Version,
  ObjectSize,
  AbstractValue, //?? default value?
  IsClass,
  Interfaces,
  Unknown,
};

enum class BodyFlags : uint32 {
  Pointer = 3,
  InlineArray = 5,
};

enum class MemberFlags {
  IgnoreSerialization,
};

struct BodyMember {
  es::string_view name;
  es::Flags<MemberFlags> flags;
  uint32 offset;
  ClassDesc *type = nullptr;
};

struct BodyInterface {
  ClassDesc *type;
  uint32 flags;
};

struct ClassDesc {
  es::string_view className;
  std::vector<ClassTemplateArgument> templateArguments;

  ClassDesc *base = nullptr;
  ClassDesc *pointer = nullptr;
  es::Flags<BodyLayoutFlags> layout;
  es::Flags<BodyFlags> flags;
  uint16 size = 0;
  uint16 alignment = 0;
  uint32 version = 0;
  uint64 absstrVal = 0;
  std::vector<BodyMember> members;
  std::vector<BodyInterface> interfaces;
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
  std::vector<ClassDesc> typeDescriptors;
  std::vector<classEntryFixup> classEntries;
  VirtualClasses virtualClasses;

  VirtualClasses &GetAllClasses() override { return virtualClasses; }
  hkToolset GetToolset() const override { return toolset; }
  void Load(BinReaderRef rd);
  void DumpClassDeclarationsAsCPP(std::ostream &str) const;
  void ToXML(pugi::xml_node node) const;
};
