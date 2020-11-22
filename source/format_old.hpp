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
#include "hklib/hk_packfile.hpp"

#define hkMagic1 0x57e0e057
#define hkMagic2 0x10c0c010

struct hkxHeader;

struct hkxHeaderlayout {
  /*
  This is basically pointer size, so 4 bytes for x86 and 8 bytes for x64
  architectures
  */
  uint8 bytesInPointer, littleEndian,
      /*
      derived classes start instead of base class end padding, rather after,
      very usefull thing this technique is applied only on non POD classes eg:
      struct Base {}; we must create base class  for A or A will be handled as
      POD class, hence compiler will have forbidden access to padding
        struct A : Base {int i; char c;};
        sizeof (A) == 8
        struct B : A {short s;};
        sizeof (B) == 8
      so compiled struct looks like this:
        struct B
        {
                int i;
                char c;
                char pad;
                short s;
        }
      instead of:
        struct B
        {
                int i;
                char c;
                char pad[3];
                short s;
                char pad[2];
        }
      this technique is used by many compilers like gcc, clang, mac compilers,
      this is not used by MSVC, or any MS like compilers
      */
      reusePaddingOptimization,
      /*
      empty base class have sizeof only 1 byte, however this byte is not passed
      to dervied classes, when optimization is applied eg: struct A {};
      sizeof(A) == 1 struct B:A{int i;}; sizeof(B) == 4 this does not apply if A
      is member of struct, in that case, it will be handled as char? thankfully
      this technique doesn't use sn and gcc v2 compilers (I cannot event find SN
      compiler reference, lol)
      */
      emptyBaseClassOptimization;
};

struct hkxSectionHeaderData {
  char sectionTag[20];
  uint32 absoluteDataStart, localFixupsOffset, globalFixupsOffset,
      virtualFixupsOffset, exportsOffset, importsOffset, bufferSize;

  void SwapEndian();
};

struct hkxSectionHeader : hkxSectionHeaderData {

  struct hkxLocalFixup {
    int32 pointer, destination;
    void SwapEndian();
  };

  struct hkxGlobalFixup {
    int32 pointer, sectionid, destination;
    void SwapEndian();
  };

  struct hkxVirtualFixup {
    int32 dataoffset, sectionid, classnameoffset;
    void SwapEndian();
  };

  uint32 sectionID;
  std::string buffer;
  std::vector<hkxLocalFixup> localFixups;
  std::vector<hkxGlobalFixup> globalFixups;
  std::vector<hkxVirtualFixup> virtualFixups;
  IhkPackFile::VirtualClasses virtualClasses;
  hkxHeader *header;
  void Load(BinReaderRef rd);
  void LoadBuffer(BinReaderRef rd);
  void LinkBuffer();
  void LinkBuffer86();
  void Finalize();
};

struct hkxHeaderData {
  uint32 magic1, magic2, userTag, version;
  hkxHeaderlayout layout;
  int32 numSections, contentsSectionIndex, contentsSectionOffset,
      contentsClassNameSectionIndex, contentsClassNameSectionOffset;
  char contentsVersion[16];
  uint32 flags;
  int16 maxpredicate, predicateArraySizePlusPadding;

  hkxHeaderData()
      : magic1(hkMagic1), magic2(hkMagic2), userTag(), numSections(2),
        contentsSectionIndex(1), contentsSectionOffset(0),
        contentsClassNameSectionIndex(0), contentsClassNameSectionOffset(75),
        flags(), maxpredicate(), predicateArraySizePlusPadding() {}

  void SwapEndian();
};

struct hkxHeader : hkxHeaderData, IhkPackFile {
  hkToolset toolset;

  std::vector<hkxSectionHeader> sections;

  hkxHeader() : toolset(HKUNKVER) {}

  void Load(BinReaderRef rd);
  void Save(BinWritterRef wr, const VirtualClasses &classes) const;
  hkxSectionHeader *GetDataSection() { return &sections[contentsSectionIndex]; }
  VirtualClasses &GetAllClasses() override {
    return GetDataSection()->virtualClasses;
  }
  hkToolset GetToolset() const override { return toolset; }

private:
  void GenerateToolset();
};
