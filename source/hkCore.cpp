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
#include "datas/binwritter.hpp"
#include "datas/flags.hpp"
#include "datas/master_printer.hpp"
#include "hkHeader.hpp"
#include "hkNewHeader.h"
#include "pugixml.hpp"
#include <unordered_map>

#include "hkXML.h"

REFLECTOR_CREATE(hkaPartition, 1, VARNAMES, name, startBoneIndex, numBones);

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
  enum xmlToolsetPropFlags : uint8 { TopLevelObject, MaxPredicate };
  esFlags<uint8, xmlToolsetPropFlags> flags;
  uint8 version;
  const char name[16];
};

namespace std {
template <> struct hash<hkToolset> {
  constexpr size_t operator()(const hkToolset &s) const noexcept {
    return static_cast<size_t>(s);
  }
};
} // namespace std

static const std::unordered_map<hkToolset, xmlToolsetProp> xmlToolsetProps = {
    {HK500, xmlToolsetProp{{}, 5, "Havok-5.0.0-r1"}},
    {HK510, xmlToolsetProp{{}, 5, "Havok-5.1.0-r1"}},
    {HK550, xmlToolsetProp{{}, 5, "Havok-5.5.0-r1"}},
    {HK600,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 6, "Havok-6.0.0-r1"}},
    {HK610,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 6, "Havok-6.1.0-r1"}},
    {HK650,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 6, "Havok-6.5.0-r1"}},
    {HK660,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 7, "Havok-6.6.0-r1"}},
    {HK700,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 7, "Havok-7.0.0-r1"}},
    {HK710,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 7, "Havok-7.1.0-r1"}},
    {HK2010_1,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 8, "hk_2010.1.0-r1"}},
    {HK2010_2,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 8, "hk_2010.2.0-r1"}},
    {HK2011_1,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2011.1.0-r1"}},
    {HK2011_2,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2011.2.0-r1"}},
    {HK2011_3,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2011.3.0-r1"}},
    {HK2012_1,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2012.1.0-r1"}},
    {HK2012_2,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2012.2.0-r1"}},
    {HK2013,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2013.1.0-r1"}},
    {HK2014, xmlToolsetProp{{xmlToolsetProp::TopLevelObject,
                             xmlToolsetProp::MaxPredicate},
                            11,
                            "hk_2014.1.0-r1"}},
};

pugi::xml_document IhkPackFile::ToXML(hkToolset toolsetVersion) {
  pugi::xml_document doc;
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

    if (cls->HasHash(hkRootLevelContainer::GetHash()) &&
        propRef.flags[xmlToolsetProp::TopLevelObject])
      master.append_attribute("toplevelobject").set_value(addrAttr.as_string());

    cls->ToXML({&classNode, toolsetVersion});
  }

  return doc;
}

int IhkPackFile::ToXML(es::string_view fileName, hkToolset toolset) {
  std::string fleName = fileName.to_string();
  return ToXML(fleName, toolset);
}

int IhkPackFile::ToXML(const std::string &fileName, hkToolset toolset) {
  auto xDoc = ToXML(toolset);
  return !xDoc.save_file(fileName.data());
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

// https://en.wikipedia.org/wiki/Slerp
static Vector4A16 SLerp(const Vector4A16 &v0, const Vector4A16 &_v1, float t) {
  Vector4A16 v1 = _v1;
  float dot = v0.Dot(v1);

  // If the dot product is negative, slerp won't take
  // the shorter path. Fix by reversing one quaternion.
  if (dot < 0.0f) {
    v1 *= -1;
    dot *= -1;
  }

  static const float DOT_THRESHOLD = 0.9995f;
  if (dot > DOT_THRESHOLD) {
    // If the inputs are too close for comfort, linearly interpolate
    // and normalize the result.

    Vector4A16 result = v0 + (v1 - v0) * t;
    return result.Normalize();
  }

  const float theta00 = acos(dot);   // theta00 = angle between input vectors
  const float theta01 = theta00 * t; // theta01 = angle between v0 and result
  const float theta02 = sin(theta01);
  const float theta03 = 1.0f / sin(theta00);
  const float s0 = cos(theta01) - dot * theta02 * theta03;
  const float s1 = theta02 * theta03;

  return (v0 * s0) + (v1 * s1);
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
    output.rotation = ::SLerp(start.rotation, end.rotation, delta);
    output.scale = ::Lerp(start.scale, end.scale, delta);
  }
}

static uni::RTSValue ConvertRefFrame(const Vector4A16 &input) {
  uni::RTSValue retVal;
  retVal.translation = Vector4A16(input, 1.f);
  retVal.rotation.Z = sinf(input.W * 0.5);
  retVal.rotation.W = cosf(input.W * 0.5);
  return retVal;
}

void hkaAnimatedReferenceFrameInternalInterface::GetValue(uni::RTSValue &output,
                                                          float time) const {
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
    output = ConvertRefFrame(cFrame);
  } else {
    auto frame0 = GetRefFrame(frame++);
    auto frame1 = GetRefFrame(frame);

    hkQTransform start = ConvertRefFrame(frame0);
    hkQTransform end = ConvertRefFrame(frame1);

    output.translation = ::Lerp(start.translation, end.translation, delta);
    output.rotation = ::SLerp(start.rotation, end.rotation, delta);
  }
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

#define hkRegisterCreator(cname)                                               \
  {JenkinsHashC(#cname), cname##InternalInterface::Create},
#define hkCreatorAlias(cname, aliasName)                                       \
  { JenkinsHashC(#aliasName), cname##InternalInterface::Create }

static const std::unordered_map<JenHash, hkVirtualClass *(*)(CRule)>
    hkConstrRegistry{
        hkCreatorAlias(hkaInterleavedAnimation,
                       hkaInterleavedSkeletalAnimation),
        hkCreatorAlias(hkaInterleavedAnimation,
                       hkaInterleavedUncompressedAnimation),
        hkCreatorAlias(hkaSplineCompressedAnimation,
                       hkaSplineSkeletalAnimation),
        hkCreatorAlias(hkaDeltaCompressedAnimation,
                       hkaDeltaCompressedSkeletalAnimation),
        hkCreatorAlias(hkaWaveletCompressedAnimation,
                       hkaWaveletCompressedSkeletalAnimation),
        StaticFor(hkRegisterCreator, hkRootLevelContainer, hkxEnvironment,
                  hkaSkeleton, hkaSplineCompressedAnimation,
                  hkaDeltaCompressedAnimation, hkaWaveletCompressedAnimation,
                  hkaAnimationContainer, hkaDefaultAnimatedReferenceFrame)};

hkVirtualClass *hkVirtualClass::Create(JenHash hash, CRule rule) {
  auto found = hkConstrRegistry.find(hash);

  if (es::IsEnd(hkConstrRegistry, found)) {
    return nullptr;
  }

  auto madeClass = found->second(rule);
  madeClass->rule = rule;

  return madeClass;
}
