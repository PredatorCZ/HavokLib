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

#include "HavokXMLApi.hpp"
#include "datas/binreader.hpp"
#include "datas/flags.hpp"
#include "datas/masterprinter.hpp"
#include "hkHeader.hpp"
#include "hkNewHeader.h"
#include "pugixml.hpp"
#include <unordered_map>

#include "hkXML.h"

REFLECTOR_CREATE(hkaPartition, 1, VARNAMES, name, startBoneIndex, numBones);
REGISTER_ENUMS(BlendHint, hkaAnimationType)

IhkPackFile::IhkPackFile()
{
  RegisterLocalEnums();
  REGISTER_CLASSES(hkaPartition);
}

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

struct xmlToolsetProp {
  enum xmlToolsetPropFlags { TopLevelObject, MaxPredicate };
  esFlags<uint8, xmlToolsetPropFlags> flags;
  char version[3];
  const char name[16];
};

static const std::unordered_map<hkXMLToolsets, xmlToolsetProp> xmlToolsetProps =
    {
        {HK550, xmlToolsetProp{{}, "5", "Havok-5.5.0-r1"}},
        {HK660,
         xmlToolsetProp{xmlToolsetProp::TopLevelObject, "7", "Havok-6.6.0-r1"}},
        {HK710,
         xmlToolsetProp{xmlToolsetProp::TopLevelObject, "7", "Havok-7.1.0-r1"}},
        {HK2010_2,
         xmlToolsetProp{xmlToolsetProp::TopLevelObject, "8", "hk_2010.2.0-r1"}},
        {HK2011,
         xmlToolsetProp{xmlToolsetProp::TopLevelObject, "9", "hk_2011.1.0-r1"}},
        {HK2011_2,
         xmlToolsetProp{xmlToolsetProp::TopLevelObject, "9", "hk_2011.2.0-r1"}},
        {HK2012_2,
         xmlToolsetProp{xmlToolsetProp::TopLevelObject, "9", "hk_2012.2.0-r1"}},
        {HK2013,
         xmlToolsetProp{xmlToolsetProp::TopLevelObject, "9", "hk_2013.1.0-r1"}},
        {HK2014,
         xmlToolsetProp{
             esFlags<uint8, xmlToolsetProp::xmlToolsetPropFlags>(
                 xmlToolsetProp::TopLevelObject, xmlToolsetProp::MaxPredicate),
             "11", "hk_2014.1.0-r1"}},
};

void IhkPackFile::_GenerateXML(pugi::xml_document &doc,
                               hkXMLToolsets toolsetVersion) {
  pugi::xml_node master = doc.append_child("hkpackfile");

  const xmlToolsetProp &propRef = xmlToolsetProps.at(toolsetVersion);

  master.append_attribute("classversion").set_value(propRef.version);
  master.append_attribute("contentsversion").set_value(propRef.name);

  if (propRef.flags[xmlToolsetProp::MaxPredicate]) {
    master.append_attribute("maxpredicate").set_value(21);
    master.append_attribute("predicates");
  }

  pugi::xml_node dataSection = master.append_child("hksection");
  dataSection.append_attribute(_hkName).set_value("__data__");

  const VirtualClasses &allClasses = GetAllClasses();

  for (auto &c : allClasses) {
    hkVirtualClass *cls = dynamic_cast<hkVirtualClass *>(c.get());
    pugi::xml_node classNode = dataSection.append_child(_hkObject);
    pugi::xml_attribute addrAttr = classNode.append_attribute(_hkName);

    std::string _buffer;
    PointerToString(cls->GetPointer(), _buffer);

    addrAttr.set_value(_buffer.c_str());
    classNode.append_attribute(_hkClass).set_value(
        cls->GetClassName(toolsetVersion).c_str());

    if (cls->superHash == hkRootLevelContainer::HASH &&
        propRef.flags[xmlToolsetProp::TopLevelObject])
      master.append_attribute("toplevelobject").set_value(addrAttr.as_string());

    cls->ToXML({&classNode, toolsetVersion});
  }
}

template <class _Ty>
int IhkPackFile::_ExportXML(const _Ty *fileName, hkXMLToolsets toolsetVersion) {
  pugi::xml_document doc;
  _GenerateXML(doc, toolsetVersion);

  return !doc.save_file(fileName);
}

template int IhkPackFile::_ExportXML(const wchar_t *fileName,
                                     hkXMLToolsets toolsetVersion);
template int IhkPackFile::_ExportXML(const char *fileName,
                                     hkXMLToolsets toolsetVersion);

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

    if (cls->superHash == hash)
      buffa.push_back(c.get());
  }

  return buffa;
}

void hkaSkeletonInternalInterface::Process() {
  storage.resize(GetNumBones());
  size_t curId = 0;

  for (auto &b : storage) {
    b.id = curId;
    b.name = GetBoneName(curId);
    int16 prentID = GetBoneParentID(curId);
    b.parent = prentID < 0 ? nullptr : &storage[prentID];
    b.tm = GetBoneTM(curId);

    curId++;
  }
}

void hkaAniTrackHandle::GetValue(uni::RTSValue &output, float time) const {
  hdl->GetValue(output, time, index);
}

static Vector4A16 Lerp(const Vector4A16 &v1, const Vector4A16 &v2,
                         float delta) {
    return v1 + (v2 - v1) * delta;
  }

void hkaAnimationLerpSampler::GetValue(uni::RTSValue &output, float time,
                                       size_t trackID) const {
  const float frameFull = time * frameRate;
  int32 frame = static_cast<int32>(frameFull);
  float delta = frameFull - frame;

  if (frame >= numFrames) {
    frame = static_cast<int32>(numFrames - 1);
    delta = 0.f;
  }

  if (!delta) {
    GetFrame(trackID, frame, output);
  } else {
    hkQTransform start;
    GetFrame(trackID, frame++, start);
    hkQTransform end;
    GetFrame(trackID, frame, end);

    output.translation = ::Lerp(start.translation, end.translation, delta);
    output.rotation = ::Lerp(start.rotation, end.rotation, delta);
    output.scale = ::Lerp(start.scale, end.scale, delta);
  }
}

static uni::RTSValue ConvertRefFrame(const Vector4 &input) {
  uni::RTSValue retVal;
  retVal.translation = Vector4A16(input, 1.f);
  retVal.rotation = Vector4A16(0, 0, 0, input.W);
  retVal.rotation.QComputeElement<2>();
  return retVal;
}

void hkaAnimatedReferenceFrameInternalInterface::GetValue(uni::RTSValue &output, float time) const {
  const float frameFull = time * frameRate;
  int32 frame = static_cast<int32>(frameFull);
  float delta = frameFull - frame;
  const auto numFrames = GetNumFrames();

  if (frame >= numFrames) {
    frame = static_cast<int32>(numFrames - 1);
    delta = 0.f;
  }

  if (!delta) {
    auto cFrame = GetRefFrame(frame);
    output = ConvertRefFrame(*cFrame);
  } else {
    auto frame0 = GetRefFrame(frame++);
    auto frame1 = GetRefFrame(frame);

    hkQTransform start = ConvertRefFrame(*frame0);
    hkQTransform end = ConvertRefFrame(*frame1);

    output.translation = ::Lerp(start.translation, end.translation, delta);
    output.rotation = ::Lerp(start.rotation, end.rotation, delta);
  }
}

