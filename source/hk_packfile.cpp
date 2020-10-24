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

#include "toolset.hpp"

#include "datas/binreader.hpp"
#include "datas/binwritter.hpp"
#include "datas/master_printer.hpp"
#include "hklib/hk_rootlevelcontainer.hpp"
#include "internal/hk_internal_api.hpp"
#include "format_new.hpp"
#include "format_old.hpp"

IhkPackFile *IhkPackFile::Create(const std::string &fileName,
                                 bool suppressErrors) {
  BinReader rd(fileName);

  if (!rd.IsValid()) {
    if (!suppressErrors) {
      printerror("[Havok] Cannot open file. " << fileName);
    }

    return nullptr;
  }

  struct {
    int ID1, ID2;
  } testerStruct;

  rd.Read(testerStruct);
  rd.Seek(0);

  if (testerStruct.ID1 == hkMagic1 && testerStruct.ID2 == hkMagic2) {
    hkxHeader *hdr = new hkxHeader{};

    if (hdr->Load(rd)) {
      delete hdr;
      hdr = nullptr;
    }
    return hdr;
  } else if (testerStruct.ID2 == hkHederTAG) {
    hkxNewHeader *hdr = new hkxNewHeader{};

    if (hdr->Load(&rd)) {
      delete hdr;
      hdr = nullptr;
    }
    return hdr;
  } else if (!suppressErrors) {
    printerror("[Havok] Invalid packfile.");
  }

  return nullptr;
}

IhkPackFile *IhkPackFile::Create(es::string_view fileName,
                                 bool suppressErrors) {
  std::string fleName = fileName.to_string();
  return Create(fleName);
}

const IhkVirtualClass *IhkPackFile::GetClass(const void *ptr) {
  VirtualClasses &classes = GetAllClasses();

  for (auto &c : classes) {
    hkVirtualClass *cls = dynamic_cast<hkVirtualClass *>(c.get());

    if (cls->GetPointer() == ptr)
      return c.get();
  }

  return nullptr;
}

IhkPackFile::VirtualClassesRef IhkPackFile::GetClasses(JenHash hash) {
  VirtualClasses &classes = GetAllClasses();
  VirtualClassesRef buffa;

  for (auto &c : classes) {
    hkVirtualClass *cls = dynamic_cast<hkVirtualClass *>(c.get());

    if (cls->HasHash(hash)) {
      buffa.push_back(c.get());
    }
  }

  return buffa;
}

int IhkPackFile::ToPackFile(const std::string &fileName, hkToolset toolset,
                            uint32 rule) {
  if (toolset == HKUNKVER) {
    printerror("[Havok] Unknown toolset version.");
    return 1;
  }

  if (toolset > HK2014) {
    printerror("[Havok] Unsupported toolset version.");
    return 2;
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

  if (!wr.IsValid()) {
    printerror("[Havok] Cannot create file.") return 3;
  }

  auto prop = xmlToolsetProps.at(toolset);

  hkxHeader hkHead;
  hkHead.layout = layout;
  hkHead.version = prop.version;
  hkHead.toolset = toolset;
  strncpy(hkHead.contentsVersion, prop.name, strlen(prop.name) + 1);
  hkHead.Save(wr, GetAllClasses());

  return 0;
}

hkRootLevelContainer *IhkPackFile::GetRootLevelContainer() {
  return dynamic_cast<hkRootLevelContainer *>(
    GetClasses(hkRootLevelContainer::GetHash())[0]);
}
