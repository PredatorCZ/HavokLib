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

#include "toolset.hpp"

#include "datas/binreader.hpp"
#include "datas/binwritter.hpp"
#include "datas/except.hpp"
#include "datas/master_printer.hpp"
#include "format_new.hpp"
#include "format_old.hpp"
#include "hklib/hk_rootlevelcontainer.hpp"
#include "internal/hk_internal_api.hpp"

IhkPackFile::Ptr IhkPackFile::Create(const std::string &fileName) {
  BinReader rd(fileName);

  struct {
    uint32 ID1, ID2;
  } testerStruct;

  rd.Read(testerStruct);
  rd.Seek(0);

  if (testerStruct.ID1 == hkMagic1 && testerStruct.ID2 == hkMagic2) {
    auto hdr = std::make_unique<hkxHeader>();
    hdr->Load(rd);
    return hdr;
  } else if (testerStruct.ID2 == hkHederTAG) {
    auto hdr = std::make_unique<hkxNewHeader>();
    hdr->Load(rd);
    return hdr;
  }

  throw es::InvalidHeaderError(testerStruct.ID1);
}

const IhkVirtualClass *IhkPackFile::GetClass(const void *ptr) {
  VirtualClasses &classes = GetAllClasses();

  for (auto &c : classes) {
    if (c->GetPointer() == ptr)
      return c.get();
  }

  return nullptr;
}

IhkPackFile::VirtualClassesRef IhkPackFile::GetClasses(JenHash hash) {
  VirtualClasses &classes = GetAllClasses();
  VirtualClassesRef buffa;

  for (auto &c : classes) {
    auto cls = checked_deref_cast<const hkVirtualClass>(c.get());

    if (cls->HasHash(hash)) {
      buffa.push_back(c.get());
    }
  }

  return buffa;
}

void IhkPackFile::ToPackFile(const std::string &fileName, hkToolset toolset,
                             uint32 rule) {
  if (toolset == HKUNKVER || toolset > HK2014) {
    throw es::InvalidVersionError(toolset);
  }

  hkxHeaderlayout layout;
  layout.emptyBaseClassOptimization = rule & 0xf;
  layout.reusePaddingOptimization = (rule >> 4) & 0xf;
  layout.littleEndian = (rule >> 8) & 0xf;
  layout.bytesInPointer = (rule >> 12) & 0xf;

  if (layout.bytesInPointer > 4) {
    layout.bytesInPointer = 8;
  } else {
    layout.bytesInPointer = 4;
  }

  if (layout.littleEndian > 1) {
    printwarning(
        "[Havok] Unknown layout rule littleEndian, expected [0,1], got: "
        << layout.littleEndian << ". Handling as true.");
    layout.littleEndian = 1;
  }

  if (layout.emptyBaseClassOptimization > 1) {
    printwarning("[Havok] Unknown layout rule emptyBaseClassOptimization, "
                 "expected [0,1], got: "
                 << layout.emptyBaseClassOptimization << ". Handling as true.");
    layout.emptyBaseClassOptimization = 1;
  }

  if (layout.reusePaddingOptimization > 1) {
    printwarning("[Havok] Unknown layout rule reusePaddingOptimization, "
                 "expected [0,1], got: "
                 << layout.reusePaddingOptimization << ". Handling as true.");
    layout.reusePaddingOptimization = 1;
  }

  BinWritter wr(fileName);

  auto prop = xmlToolsetProps.at(toolset);

  hkxHeader hkHead;
  hkHead.layout = layout;
  hkHead.version = prop.version;
  hkHead.toolset = toolset;
  memcpy(hkHead.contentsVersion, prop.name, sizeof(hkHead.contentsVersion));
  hkHead.Save(wr, GetAllClasses());
}

const hkRootLevelContainer *IhkPackFile::GetRootLevelContainer() {
  return safe_deref_cast<const hkRootLevelContainer>(
      GetClasses(hkRootLevelContainer::GetHash())[0]);
}

void hkVirtualClass::Save(BinWritterRef , hkFixups &) const {}
