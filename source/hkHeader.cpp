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

#include "hkHeader.hpp"
#include "datas/endian.hpp"
#include "datas/jenkinshash.hpp"
#include "datas/masterprinter.hpp"
#include "datas/pointer.hpp"
#include <algorithm>
#include <ctype.h>
#include <string>

#include "datas/binreader.hpp"

int hkxHeader::Load(BinReader &rd) {
  rd.Read<hkxHeaderData>(*this);

  if (magic1 != hkMagic1 || magic2 != hkMagic2) {
    printerror("[Havok] Invalid packfile.");
    return 1;
  }

  uint32 currentSectionID = 0;

  layout.big_endian = 1 - layout.big_endian;

  if (layout.big_endian) {
    SwapEndian();
    rd.SwapEndian(true);
  }

  if (maxpredicate != -1)
    rd.Skip(predicateArraySizePlusPadding);

  char *cs = contentsVersionStripped;
  char *cc = contentsVersion;
  uint32 numDots = 0, numChars = 0;

  while (cc) {
    if (*cc == '.')
      numDots++;

    if (isdigit(*cc)) {
      *cs = *cc;
      numChars++;
      if (numDots > 1 || numChars > 3)
        break;
      cs++;
    }
    cc++;
  }

  sections.resize(numSections);

  for (auto &s : sections) {
    s.header = this;
    rd.Read<hkxSectionHeaderData>(s);

    if (Version > 9)
      rd.Seek(16, std::ios_base::cur);

    s.sectionID = currentSectionID;
    currentSectionID++;
  }

  for (auto &s : sections)
    s.Load(&rd);

  for (auto &s : sections)
    s.LoadBuffer(&rd);

  if (ES_X64 && layout.bytesInPointer == 4) {
    for (auto &s : sections)
      s.LinkBuffer86();
  } else {
    for (auto &s : sections)
      s.LinkBuffer();
  }

  return 0;
}

int32 hkxHeader::GetVersion() const { return atoi(contentsVersionStripped); }

void hkxHeaderData::SwapEndian() {
  FByteswapper(Version);
  FByteswapper(numSections);
  FByteswapper(contentsSectionIndex);
  FByteswapper(contentsSectionOffset);
  FByteswapper(contentsClassNameSectionIndex);
  FByteswapper(contentsClassNameSectionOffset);
  FByteswapper(flags);
  FByteswapper(maxpredicate);
  FByteswapper(predicateArraySizePlusPadding);
}

int hkxSectionHeader::Load(BinReader *rd) {
  rd->SetRelativeOrigin(absoluteDataStart);

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

  rd->Seek(localFixupsOffset);
  rd->ReadContainer(localFixups, circaNumLocalFixps);
  rd->Seek(globalFixupsOffset);
  rd->ReadContainer(globalFixups, circaNumGlobalFixps);
  rd->Seek(virtualFixupsOffset);
  rd->ReadContainer(virtualFixups, circaNumVirtualFixps);

  rd->ResetRelativeOrigin();

  return 0;
}

int hkxSectionHeader::LoadBuffer(BinReader *rd) {

  if (!bufferSize)
    return 0;

  rd->Seek(absoluteDataStart);
  rd->ReadContainer(buffer, localFixupsOffset);

  return 0;
}

std::string _hkGenerateClassname(hkxHeader *header,
                                 const std::string &classname) {
  std::string compiledClassname =
      classname + "_t<" + classname + header->contentsVersionStripped;

  if (header->layout.reusePaddingOptimization)
    compiledClassname.append("_rp");

  compiledClassname.append("_t<");

  if (header->layout.bytesInPointer > 4)
    compiledClassname.append("esPointerX64");
  else
    compiledClassname.append("esPointerX86");

  compiledClassname.append(">>");

  return compiledClassname;
}

int hkxSectionHeader::LinkBuffer() {
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

  for (auto &vf : virtualFixups) {
    if (vf.dataoffset != -1) {
      const char *clName =
          header->sections[vf.sectionid].buffer.data() + vf.classnameoffset;
      std::string classname = _hkGenerateClassname(header, clName);

      const JenHash _chash = JenkinsHash(classname.c_str());

      hkVirtualClass *cls = IhkPackFile::ConstructClass(_chash);

      if (cls) {
        cls->SetDataPointer(sectionBuffer + vf.dataoffset);
        cls->name = clName;
        cls->superHash = JenkinsHash(clName);
        cls->header = header;
        if (header->layout.big_endian)
          cls->SwapEndian();
        virtualClasses.emplace_back(cls);
        cls->Process();
      }
    }
  }

#ifdef _MSC_VER
  localFixups.~vector();
  globalFixups.~vector();
  virtualFixups.~vector();
#else
  localFixups.clear();
  globalFixups.clear();
  virtualFixups.clear();
#endif

  return 0;
}

int hkxSectionHeader::LinkBuffer86() {
  char *sectionBuffer = &buffer[0];
  using ptrType = esPointerX86<char>;

  for (auto &lf : localFixups)
    if (lf.pointer != -1) {
      ptrType *ptrPtr = reinterpret_cast<ptrType *>(sectionBuffer + lf.pointer);
      *ptrPtr = sectionBuffer + lf.destination;
    }

  for (auto &gf : globalFixups)
    if (gf.pointer != -1) {
      ptrType *ptrPtr = reinterpret_cast<ptrType *>(sectionBuffer + gf.pointer);
      *ptrPtr = sectionBuffer + gf.destination;
    }

  for (auto &vf : virtualFixups)
    if (vf.dataoffset != -1) {
      const char *clName =
          header->sections[vf.sectionid].buffer.data() + vf.classnameoffset;
      std::string classname = _hkGenerateClassname(header, clName);

      const JenHash _chash = JenkinsHash(classname.c_str());

      hkVirtualClass *cls = IhkPackFile::ConstructClass(_chash);

      if (cls) {
        cls->SetDataPointer(sectionBuffer + vf.dataoffset);
        cls->name = clName;
        cls->superHash = JenkinsHash(clName);
        cls->header = header;
        if (header->layout.big_endian)
          cls->SwapEndian();
        virtualClasses.emplace_back(cls);
        cls->Process();
      }
    }

    std::move(localFixups);
#ifdef _MSC_VER
  localFixups.~vector();
  globalFixups.~vector();
  virtualFixups.~vector();
#else
  localFixups.clear();
  globalFixups.clear();
  virtualFixups.clear();
#endif

  return 0;
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
