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

#include "datas/endian.hpp"

#include "format_old.hpp"
#include "datas/except.hpp"
#include "datas/jenkinshash.hpp"
#include "datas/master_printer.hpp"
#include "datas/pointer.hpp"
#include "fixups.hpp"
#include "hklib/hka_skeleton.hpp"
#include "internal/hk_internal_api.hpp"
#include <algorithm>
#include <ctype.h>
#include <string>
#include <unordered_map>

#include "datas/binreader.hpp"
#include "datas/binwritter.hpp"

void hkxHeader::Load(BinReaderRef rd) {
  rd.Read<hkxHeaderData>(*this);

  if (magic1 != hkMagic1) {
    throw es::InvalidHeaderError(magic1);
  }

  if (magic2 != hkMagic2) {
    throw es::InvalidHeaderError(magic2);
  }

  uint32 currentSectionID = 0;

  if (!layout.littleEndian) {
    SwapEndian();
    rd.SwapEndian(true);
  }

  if (maxpredicate != -1) {
    rd.Skip(predicateArraySizePlusPadding);
  }

  GenerateToolset();

  sections.resize(numSections);

  for (auto &s : sections) {
    s.header = this;
    rd.Read<hkxSectionHeaderData>(s);

    if (version > 9) {
      rd.Seek(16, std::ios_base::cur);
    }

    s.sectionID = currentSectionID;
    currentSectionID++;
  }

  for (auto &s : sections) {
    s.Load(rd);
  }

  for (auto &s : sections) {
    s.LoadBuffer(rd);
  }

  if (ES_X64 && layout.bytesInPointer == 4) {
    for (auto &s : sections) {
      s.LinkBuffer86();
      s.Finalize();
    }
  } else {
    for (auto &s : sections) {
      s.LinkBuffer();
      s.Finalize();
    }
  }
}

void hkxHeader::GenerateToolset() {
  uint32 versions[3] = {};
  char *cc = contentsVersion;
  size_t cVer = 0;

  while (cc && *cc && cVer < 3) {
    if (isdigit(*cc)) {
      char *endPtr = nullptr;
      versions[cVer++] = std::strtol(cc, &endPtr, 10);
      cc = endPtr;
    } else {
      cc++;
    }
  }

  auto convert = [&]() {
    switch (versions[0]) {
    case 5: {
      switch (versions[1]) {
      case 0:
        return HK500;
      case 1:
        return HK510;
      case 5:
        return HK550;
      }
      return HKUNKVER;
    }

    case 6: {
      switch (versions[1]) {
      case 0:
        return HK600;
      case 1:
        return HK610;
      case 5:
        return HK650;
      case 6:
        return HK660;
      }
      return HKUNKVER;
    }

    case 7: {
      switch (versions[1]) {
      case 0:
        return HK700;
      case 1:
        return HK710;
      }
      return HKUNKVER;
    }

    case 2010: {
      switch (versions[1]) {
      case 1:
        return HK2010_1;
      case 2:
        return HK2010_2;
      }
      return HKUNKVER;
    }

    case 2011: {
      switch (versions[1]) {
      case 1:
        return HK2011_1;
      case 2:
        return HK2011_2;
      case 3:
        return HK2011_3;
      }
      return HKUNKVER;
    }

    case 2012: {
      switch (versions[1]) {
      case 1:
        return HK2012_1;
      case 2:
        return HK2012_2;
      }
      return HKUNKVER;
    }

    case 2013: {
      switch (versions[1]) {
      case 1:
        return HK2013;
      }
      return HKUNKVER;
    }

    case 2014: {
      switch (versions[1]) {
      case 1:
        return HK2014;
      }
      return HKUNKVER;
    }

    default:
      return HKUNKVER;
    }
  };

  toolset = convert();

  if (toolset == HKUNKVER) {
    throw es::InvalidVersionError(toolset);
  }
}

void hkxHeaderData::SwapEndian() {
  FByteswapper(version);
  FByteswapper(numSections);
  FByteswapper(contentsSectionIndex);
  FByteswapper(contentsSectionOffset);
  FByteswapper(contentsClassNameSectionIndex);
  FByteswapper(contentsClassNameSectionOffset);
  FByteswapper(flags);
  FByteswapper(maxpredicate);
  FByteswapper(predicateArraySizePlusPadding);
}

void hkxSectionHeader::Load(BinReaderRef rd) {
  rd.SetRelativeOrigin(absoluteDataStart);

  const int32 virtualEOF =
      (exportsOffset == -1 ? importsOffset : exportsOffset);
  const int32 circaNumLocalFixps =
      (globalFixupsOffset - localFixupsOffset) / sizeof(hkxLocalFixup);
  const int32 circaNumGlobalFixps =
      (virtualFixupsOffset - globalFixupsOffset) / sizeof(hkxGlobalFixup);
  const int32 circaNumVirtualFixps =
      (virtualEOF - virtualFixupsOffset) / sizeof(hkxVirtualFixup);

  localFixups.reserve(circaNumLocalFixps);
  globalFixups.reserve(circaNumGlobalFixps);
  virtualFixups.reserve(circaNumVirtualFixps);

  rd.Seek(localFixupsOffset);
  rd.ReadContainer(localFixups, circaNumLocalFixps);
  rd.Seek(globalFixupsOffset);
  rd.ReadContainer(globalFixups, circaNumGlobalFixps);
  rd.Seek(virtualFixupsOffset);
  rd.ReadContainer(virtualFixups, circaNumVirtualFixps);

  rd.ResetRelativeOrigin();
}

void hkxSectionHeader::LoadBuffer(BinReaderRef rd) {

  if (!bufferSize)
    return;

  rd.Seek(absoluteDataStart);
  rd.ReadContainer(buffer, localFixupsOffset);
}

void hkxSectionHeader::LinkBuffer() {
  char *sectionBuffer = &buffer[0];
  using ptrType = esPointerX64<char>;

  for (auto &lf : localFixups) {
    if (lf.pointer != -1) {
      ptrType *ptrPtr = reinterpret_cast<ptrType *>(sectionBuffer + lf.pointer);
      *ptrPtr = sectionBuffer + lf.destination;
    }
  }

  for (auto &gf : globalFixups) {
    if (gf.pointer != -1) {
      ptrType *ptrPtr = reinterpret_cast<ptrType *>(sectionBuffer + gf.pointer);
      *ptrPtr = &header->sections[gf.sectionid].buffer[0] + gf.destination;
    }
  }

  es::Dispose(localFixups);
  es::Dispose(globalFixups);
}

void hkxSectionHeader::LinkBuffer86() {
  char *sectionBuffer = &buffer[0];
  using ptrType = esPointerX86<char>;

  for (auto &lf : localFixups) {
    if (lf.pointer != -1) {
      ptrType *ptrPtr = reinterpret_cast<ptrType *>(sectionBuffer + lf.pointer);
      *ptrPtr = sectionBuffer + lf.destination;
    }
  }

  for (auto &gf : globalFixups) {
    if (gf.pointer != -1) {
      ptrType *ptrPtr = reinterpret_cast<ptrType *>(sectionBuffer + gf.pointer);
      *ptrPtr = sectionBuffer + gf.destination;
    }
  }

  es::Dispose(localFixups);
  es::Dispose(globalFixups);
}

void hkxSectionHeader::Finalize() {
  char *sectionBuffer = &buffer[0];

  for (auto &vf : virtualFixups) {
    if (vf.dataoffset != -1) {
      es::string_view clName =
          header->sections[vf.sectionid].buffer.data() + vf.classnameoffset;
      const JenHash chash(clName);
      CRule rule(header->toolset, header->layout.reusePaddingOptimization,
                 header->layout.bytesInPointer > 4);
      hkVirtualClass *cls = hkVirtualClass::Create(chash, rule);

      if (cls) {
        cls->SetDataPointer(sectionBuffer + vf.dataoffset);
        cls->className = clName;
        cls->AddHash(clName);
        cls->header = header;
        if (!header->layout.littleEndian)
          cls->SwapEndian();
        virtualClasses.emplace_back(cls);
        cls->Process();
      }
    }
  }

  es::Dispose(virtualFixups);
}

void hkxSectionHeader::hkxLocalFixup::SwapEndian() {
  FByteswapper(pointer);
  FByteswapper(destination);
}

void hkxSectionHeader::hkxGlobalFixup::SwapEndian() {
  FByteswapper(pointer);
  FByteswapper(destination);
  FByteswapper(sectionid);
}

void hkxSectionHeader::hkxVirtualFixup::SwapEndian() {
  FByteswapper(dataoffset);
  FByteswapper(sectionid);
  FByteswapper(classnameoffset);
}

void hkxSectionHeaderData::SwapEndian() {
  FByteswapper(absoluteDataStart);
  FByteswapper(localFixupsOffset);
  FByteswapper(globalFixupsOffset);
  FByteswapper(virtualFixupsOffset);
  FByteswapper(exportsOffset);
  FByteswapper(importsOffset);
  FByteswapper(bufferSize);
}

void hkxHeader::Save(BinWritterRef wr, const VirtualClasses &classes) const {
  if (!sections.empty()) {
    throw std::logic_error(
        "Cannot save loaded header! Use IhkPackFile::ToPackFile().");
  }

  wr.SwapEndian((layout.littleEndian != 0) != LittleEndian());
  wr.Write<hkxHeaderData>(*this);

  hkxSectionHeader classSection;
  es::string_view classSectionTag = "__classnames__";
  strncpy(classSection.sectionTag, classSectionTag.data(),
          classSectionTag.size() + 1);

  wr.Push();
  wr.Write<hkxSectionHeaderData>(classSection);

  if (version == 11) {
    wr.Skip(16);
  }

  hkxSectionHeader mainSection;
  es::string_view sectionTag = "__data__";
  strncpy(mainSection.sectionTag, sectionTag.data(), sectionTag.size() + 1);

  wr.Write<hkxSectionHeaderData>(mainSection);

  if (version == 11) {
    wr.Skip(16);
  }

  classSection.absoluteDataStart = static_cast<uint32>(wr.Tell());
  wr.SetRelativeOrigin(wr.Tell(), false);

  VirtualClasses refClasses;
  hkFixups fixups;
  std::unordered_map<const IhkVirtualClass *, IhkVirtualClass *> clsRemap;

  static const es::string_view reqClassNames[] = {
      "hkClass", "hkClassMember", "hkClassEnum", "hkClassEnumItem"};

  for (auto &c : reqClassNames) {
    wr.Write<uint32>(0);
    wr.Write('\t');
    wr.WriteContainer(c);
    wr.Skip(1);
  }

  CRule rule(toolset, layout.reusePaddingOptimization,
             layout.bytesInPointer > 4);

  for (auto &c : classes) {
    auto dc = dynamic_cast<const hkVirtualClass *>(c.get());
    auto clName = dc->GetClassName(toolset);
    auto nClass = hkVirtualClass::Create(clName, rule);

    if (!nClass) {
      printerror("[Havok] Cannot export unregistered class: " << clName);
      continue;
    }

    clsRemap[c.get()] = nClass;
    nClass->Reflect(c.get());

    if (wr.SwappedEndian()) {
      nClass->SwapEndian();
    }

    refClasses.emplace_back(nClass);

    wr.Write<uint32>(0);
    wr.Write('\t');

    fixups.finals.emplace_back(wr.Tell());
    wr.WriteContainer(clName);
    wr.Skip(1);

    if (toolset < HK700 && clName == "hkaSkeleton") {
      wr.Write<uint32>(0);
      wr.Write('\t');

      const size_t numBones =
          dynamic_cast<hkaSkeleton *>(c.get())->GetNumBones();

      for (size_t i = 0; i < numBones; i++) {
        fixups.finals.emplace_back(wr.Tell(), c.get());
      }
      wr.WriteT("hkaBone");
    }
  }

  wr.ResetRelativeOrigin(false);
  wr.ApplyPadding();
  classSection.bufferSize =
      static_cast<uint32>(wr.Tell() - classSection.absoluteDataStart);
  classSection.exportsOffset = classSection.bufferSize;
  classSection.globalFixupsOffset = classSection.bufferSize;
  classSection.importsOffset = classSection.bufferSize;
  classSection.localFixupsOffset = classSection.bufferSize;
  classSection.virtualFixupsOffset = classSection.bufferSize;
  wr.ApplyPadding();

  mainSection.absoluteDataStart = static_cast<uint32>(wr.Tell());
  wr.SetRelativeOrigin(wr.Tell(), false);

  size_t curFixup = 0;
  std::unordered_map<IhkVirtualClass *, size_t> savedClasses;

  for (auto &c : refClasses) {
    wr.ApplyPadding();
    const auto clsOffset = wr.Tell();
    savedClasses[c.get()] = clsOffset;
    fixups.finals[curFixup++].destination = clsOffset;

    auto cls = dynamic_cast<hkVirtualClass *>(c.get());
    cls->Save(wr, fixups);

    while (curFixup < fixups.finals.size() &&
           fixups.finals[curFixup].destClass) {
      curFixup++;
    }
  }

  for (auto &l : fixups.locals) {
    hkxSectionHeader::hkxLocalFixup lFix;
    lFix.pointer = static_cast<int32>(l.strOffset);

    if (l.destClass) {
      auto sClass = clsRemap[l.destClass];
      lFix.destination = static_cast<int32>(savedClasses[sClass]);
    } else {
      lFix.destination = static_cast<int32>(l.destination);
    }

    mainSection.localFixups.push_back(lFix);
  }

  for (auto &l : fixups.finals) {
    hkxSectionHeader::hkxVirtualFixup lFix;
    lFix.sectionid = 0;
    lFix.classnameoffset = l.strOffset;
    lFix.dataoffset = l.destination;
    mainSection.virtualFixups.push_back(lFix);
  }

  for (auto &l : fixups.globals) {
    hkxSectionHeader::hkxGlobalFixup lFix;
    lFix.sectionid = 1;
    lFix.pointer = l.strOffset;
    lFix.destination = l.destination;
    mainSection.globalFixups.push_back(lFix);
  }

  wr.ApplyPadding();
  mainSection.localFixupsOffset = static_cast<int32>(wr.Tell());
  wr.WriteContainer(mainSection.localFixups);
  if (mainSection.localFixups.size() & 1) {
    wr.Write<int64>(-1);
  }

  wr.ApplyPadding();
  mainSection.globalFixupsOffset = static_cast<int32>(wr.Tell());
  wr.WriteContainer(mainSection.globalFixups);
  mainSection.virtualFixupsOffset = static_cast<int32>(wr.Tell());
  wr.WriteContainer(mainSection.virtualFixups);

  const size_t pad = GetPadding(wr.Tell(), 16) / 4;

  for (size_t p = 0; p < pad; p++) {
    wr.Write<int32>(-1);
  }

  mainSection.bufferSize = static_cast<int32>(wr.Tell());
  mainSection.exportsOffset = mainSection.bufferSize;
  mainSection.importsOffset = mainSection.bufferSize;

  wr.ResetRelativeOrigin(false);
  wr.Pop();
  wr.Write<hkxSectionHeaderData>(classSection);

  if (version == 11) {
    wr.Skip(16);
  }

  wr.Write<hkxSectionHeaderData>(mainSection);
}
