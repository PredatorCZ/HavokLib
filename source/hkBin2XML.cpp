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

#include "datas/masterprinter.hpp"
#include "hkInternalInterfaces.h"
#include "pugixml.hpp"

#include "hkXML.h"

void hkNamedVariant::ToXML(XMLHandle hdl) const {
  pugi::xml_node &objectNode = *hdl.node;

  pugi::xml_node nameNode = objectNode.append_child(_hkParam);
  nameNode.append_attribute(_hkName).set_value(_hkName);
  nameNode.append_buffer(name.data(), name.size());

  pugi::xml_node classNameNode = objectNode.append_child(_hkParam);
  classNameNode.append_attribute(_hkName).set_value("className");
  classNameNode.append_buffer(className.data(), className.size());

  pugi::xml_node variantNode = objectNode.append_child(_hkParam);
  variantNode.append_attribute(_hkName).set_value("variant");

  std::string buffer;

  if (hdl.toolset < HK710)
    buffer.push_back('(');

  PointerToString(pointer->GetPointer(), buffer);

  if (hdl.toolset < HK710) {
    buffer.append(" null)");
  }
  variantNode.append_buffer(buffer.c_str(), buffer.size());
}

void hkRootLevelContainerInternalInterface::ToXML(XMLHandle hdl) const {
  pugi::xml_node vartiantNode = hdl.node->append_child(_hkParam);
  vartiantNode.append_attribute(_hkName).set_value("namedVariants");

  int numVariants = 0;

  for (auto &v : *this) {
    if (!v.pointer) {
      printwarning("[Havok XML] Couldn't export variant: \""
                   << v.name << "\" undefined class: " << v.className);
      continue;
    }

    pugi::xml_node objectNode = vartiantNode.append_child(_hkObject);
    v.ToXML({&objectNode, hdl.toolset});
    numVariants++;
  }

  vartiantNode.append_attribute(_hkNumElements).set_value(numVariants);
}

void hkaAnimationContainerInternalInterface::ToXML(XMLHandle hdl) const {
  pugi::xml_node skelNode = hdl.node->append_child(_hkParam);
  skelNode.append_attribute(_hkName).set_value("skeletons");
  skelNode.append_attribute(_hkNumElements).set_value(GetNumSkeletons());

  pugi::xml_node animNode = hdl.node->append_child(_hkParam);
  animNode.append_attribute(_hkName).set_value("animations");
  animNode.append_attribute(_hkNumElements).set_value(GetNumAnimations());

  pugi::xml_node bindNode = hdl.node->append_child(_hkParam);
  bindNode.append_attribute(_hkName).set_value("bindings");
  bindNode.append_attribute(_hkNumElements).set_value(GetNumBindings());

  pugi::xml_node attNode = hdl.node->append_child(_hkParam);
  attNode.append_attribute(_hkName).set_value("attachments");
  attNode.append_attribute(_hkNumElements).set_value(GetNumAttachments());

  pugi::xml_node skinNode = hdl.node->append_child(_hkParam);
  skinNode.append_attribute(_hkName).set_value("skins");
  skinNode.append_attribute(_hkNumElements).set_value(GetNumSkins());

  std::string buffer;

  for (auto &s : Skeletons()) {
    PointerToString(s.GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    skelNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  for (auto &s : Animations()) {
    PointerToString(s.GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    animNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  for (auto &s : Bindings()) {
    if (!&s)
      continue;

    PointerToString(s.GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    bindNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  for (auto &s : Attachments()) {
    PointerToString(s.GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    attNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  for (auto &s : MeshBinds()) {
    PointerToString(s.GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    skinNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }
}

std::string to_string(const hkQTransform &tm) {
  std::string retVal;
  retVal.push_back('(');
  retVal.append(reinterpret_cast<const Vector &>(tm.translation).ToString())
      .append(")(")
      .append(tm.rotation.ToString())
      .append(")(")
      .append(reinterpret_cast<const Vector &>(tm.scale).ToString())
      .push_back(')');
  return retVal;
}

void hkaSkeletonInternalInterface::ToXML(XMLHandle hdl) const {
  pugi::xml_node nameNode = hdl.node->append_child(_hkParam);
  nameNode.append_attribute(_hkName).set_value(_hkName);
  auto skName = Name();
  nameNode.append_buffer(skName.data(), skName.size());

  pugi::xml_node parentNode = hdl.node->append_child(_hkParam);
  parentNode.append_attribute(_hkName).set_value("parentIndices");
  parentNode.append_attribute(_hkNumElements).set_value(GetNumBones());

  static const char *ident = "\n\t\t\t\t";

  std::string buffer = ident;
  int cc = 0;

  for (const auto s : BoneParentIDs()) {
    buffer += std::to_string(s);
    cc++;

    if (cc % 0x10)
      buffer += ' ';
    else
      buffer += ident;
  }

  if (buffer.size()) {
    buffer.pop_back();
    buffer += ident;
    buffer.pop_back();
    parentNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  pugi::xml_node bonesNode = hdl.node->append_child(_hkParam);
  bonesNode.append_attribute(_hkName).set_value("bones");
  bonesNode.append_attribute(_hkNumElements).set_value(GetNumBones());

  if (hdl.toolset > HK660) {
    for (auto b : BoneNames()) {
      pugi::xml_node boneNode = bonesNode.append_child(_hkObject);

      pugi::xml_node boneNameNode = boneNode.append_child(_hkParam);
      boneNameNode.append_attribute(_hkName).set_value(_hkName);
      boneNameNode.append_buffer(b.data(), b.size());

      pugi::xml_node boneLockTSNode = boneNode.append_child(_hkParam);
      boneLockTSNode.append_attribute(_hkName).set_value("lockTranslation");
      boneLockTSNode.append_buffer("false", 6);
    }
  } else {
    pugi::xml_node datanode = hdl.node->parent();
    buffer = ident;
    cc = 0;

    for (auto b : BoneNames()) {
      std::string _buffer;
      PointerToString(b.data(), _buffer);

      pugi::xml_node boneNode = datanode.append_child(_hkObject);
      boneNode.append_attribute(_hkName).set_value(_buffer.c_str());
      boneNode.append_attribute(_hkClass).set_value("hkaBone");

      pugi::xml_node boneNameNode = boneNode.append_child(_hkParam);
      boneNameNode.append_attribute(_hkName).set_value(_hkName);
      boneNameNode.append_buffer(b.data(), b.size());

      pugi::xml_node boneLockTSNode = boneNode.append_child(_hkParam);
      boneLockTSNode.append_attribute(_hkName).set_value("lockTranslation");
      boneLockTSNode.append_buffer("false", 6);

      buffer += _buffer;
      cc++;

      if (cc % 0x10)
        buffer += ' ';
      else
        buffer += ident;
    }

    if (buffer.size()) {
      buffer.pop_back();
      buffer += ident;
      buffer.pop_back();
      bonesNode.append_buffer(buffer.c_str(), buffer.size());
      buffer.clear();
    }
  }

  pugi::xml_node refsNode = hdl.node->append_child(_hkParam);
  refsNode.append_attribute(_hkName).set_value("referencePose");
  refsNode.append_attribute(_hkNumElements).set_value(GetNumBones());
  buffer = ident;

  for (auto &s : BoneTransforms())
    buffer += ::to_string(s) + ident;

  if (buffer.size()) {
    buffer.pop_back();
    refsNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  if (hdl.toolset > HK550) {
    pugi::xml_node refsNode = hdl.node->append_child(_hkParam);
    refsNode.append_attribute(_hkName).set_value("localFrames");
    refsNode.append_attribute(_hkNumElements).set_value(GetNumLocalFrames());

    for (auto s : LocalFrames()) {
      std::string _buffer;
      PointerToString(s.localFrame, _buffer);

      std::string boneIndexStr = std::to_string(s.boneIndex);
      pugi::xml_node frameNode = bonesNode.append_child(_hkObject);

      pugi::xml_node localFrameNode = frameNode.append_child(_hkParam);
      localFrameNode.append_attribute(_hkName).set_value("localFrame");
      localFrameNode.append_buffer(_buffer.c_str(), _buffer.size());

      pugi::xml_node boneIndex = frameNode.append_child(_hkParam);
      boneIndex.append_attribute(_hkName).set_value("boneIndex");
      localFrameNode.append_buffer(boneIndexStr.c_str(), boneIndexStr.size());
    }
  }

  if (hdl.toolset > HK710) {
    pugi::xml_node refsNode = hdl.node->append_child(_hkParam);
    refsNode.append_attribute(_hkName).set_value("referenceFloats");
    refsNode.append_attribute(_hkNumElements)
        .set_value(GetNumReferenceFloats());

    buffer = ident;
    cc = 0;

    for (const float &s : ReferenceFloats()) {
      buffer += std::to_string(s);
      cc++;

      if (cc % 0x10)
        buffer += ' ';
      else
        buffer += ident;
    }

    if (buffer.size()) {
      buffer.pop_back();
      buffer += ident;
      buffer.pop_back();
      refsNode.append_buffer(buffer.c_str(), buffer.size());
      buffer.clear();
    }
  }

  pugi::xml_node floatSlotsNode = hdl.node->append_child(_hkParam);
  floatSlotsNode.append_attribute(_hkName).set_value("floatSlots");
  floatSlotsNode.append_attribute(_hkNumElements).set_value(GetNumFloatSlots());

  for (auto s : FloatSlots()) {
    floatSlotsNode.append_child("hkcstring").append_buffer(s.data(), s.size());
  }

  if (hdl.toolset > HK2011_2) {
    pugi::xml_node partsNode = hdl.node->append_child(_hkParam);
    partsNode.append_attribute(_hkName).set_value("partitions");
    partsNode.append_attribute(_hkNumElements).set_value(GetNumFloatSlots());

    for (auto s : Partitions()) {
      pugi::xml_node hkobj = partsNode.append_child(_hkObject);
      ExportReflectedClass(s, hkobj);
    }
  }
}

void hkaAnnotationTrackInternalInterface::ToXML(XMLHandle hdl) const {
  std::string _buff;

  pugi::xml_node tracksNode = hdl.node->append_child(_hkParam);
  tracksNode.append_attribute(_hkName).set_value(
      hdl.toolset == HK550 ? _hkName : "trackName");
  auto aName = this->GetName();
  tracksNode.append_buffer(aName.data(), aName.size());

  pugi::xml_node annotsNode = hdl.node->append_child(_hkParam);
  annotsNode.append_attribute(_hkName).set_value("annotations");
  annotsNode.append_attribute(_hkNumElements).set_value(Size());

  for (auto a : *this) {
    pugi::xml_node cObjNode = annotsNode.append_child(_hkObject);

    pugi::xml_node timeNode = cObjNode.append_child(_hkParam);
    timeNode.append_attribute("time");
    _buff = std::to_string(a.time);
    timeNode.append_buffer(_buff.c_str(), _buff.size());

    pugi::xml_node textNode = cObjNode.append_child(_hkParam);
    textNode.append_attribute("text");
    textNode.append_buffer(a.text.data(), a.text.size());
  }
}

void hkaAnimationInternalInterface::ToXML(XMLHandle hdl) const {
  std::string _buff;

  pugi::xml_node typeNode = hdl.node->append_child(_hkParam);
  typeNode.append_attribute(_hkName).set_value("type");
  auto anType = GetAnimationTypeName();
  typeNode.append_buffer(anType.data(), anType.size());

  pugi::xml_node durationNode = hdl.node->append_child(_hkParam);
  durationNode.append_attribute(_hkName).set_value("duration");
  _buff = std::to_string(Duration());
  durationNode.append_buffer(_buff.c_str(), _buff.size());

  pugi::xml_node numTransNode = hdl.node->append_child(_hkParam);
  numTransNode.append_attribute(_hkName).set_value("numberOfTransformTracks");
  _buff = std::to_string(GetNumOfTransformTracks());
  numTransNode.append_buffer(_buff.c_str(), _buff.size());

  pugi::xml_node numFloatsNode = hdl.node->append_child(_hkParam);
  numFloatsNode.append_attribute(_hkName).set_value("numberOfFloatTracks");
  _buff = std::to_string(GetNumOfFloatTracks());
  numFloatsNode.append_buffer(_buff.c_str(), _buff.size());

  pugi::xml_node exMotionNode = hdl.node->append_child(_hkParam);
  exMotionNode.append_attribute(_hkName).set_value("extractedMotion");
  _buff.clear();
  PointerToString(GetExtractedMotion() ? GetExtractedMotion()->GetPointer()
                                       : nullptr,
                  _buff);
  exMotionNode.append_buffer(_buff.c_str(), _buff.size());

  pugi::xml_node annotsNode = hdl.node->append_child(_hkParam);
  annotsNode.append_attribute(_hkName).set_value("annotationTracks");
  annotsNode.append_attribute(_hkNumElements).set_value(GetNumAnnotations());

  if (hdl.toolset > HK660) {
    for (auto a : *this) {
      pugi::xml_node annotNode = annotsNode.append_child(_hkObject);
      XMLHandle annotHandle = hdl;
      annotHandle.node = &annotNode;

      dynamic_cast<const hkaAnnotationTrackInternalInterface *>(a.get())->ToXML(
          annotHandle);
    }

  } else {
    pugi::xml_node mainNode = hdl.node->parent();
    static const char *ident = "\n\t\t\t\t";
    std::string buffer = ident;
    int cc = 0;

    for (auto a : *this) {
      std::string _buffer;
      PointerToString(a.get(), _buffer);

      buffer += _buffer;
      cc++;

      if (cc % 0x10)
        buffer += ' ';
      else
        buffer += ident;

      pugi::xml_node annotNode = mainNode.append_child(_hkObject);
      annotNode.append_attribute(_hkName).set_value(_buffer.c_str());
      annotNode.append_attribute(_hkClass).set_value("hkaAnnotationTrack");

      XMLHandle annotHandle = hdl;
      annotHandle.node = &annotNode;

      dynamic_cast<const hkaAnnotationTrackInternalInterface *>(a.get())->ToXML(
          annotHandle);
    }

    if (buffer.size()) {
      buffer.pop_back();
      buffer += ident;
      buffer.pop_back();
      annotsNode.append_buffer(buffer.c_str(), buffer.size());
      buffer.clear();
    }
  }
}

void hkaInterleavedAnimationInternalInterface::ToXML(XMLHandle hdl) const {
  dynamic_cast<const hkaAnimationInternalInterface*>(this)->ToXML(hdl);

  const size_t numTransforms = GetNumTransforms();
  static const char *ident = "\n\t\t\t\t";
  std::string buffer = ident;

  pugi::xml_node transNode = hdl.node->append_child(_hkParam);
  transNode.append_attribute(_hkName).set_value("transforms");
  transNode.append_attribute(_hkNumElements).set_value(numTransforms);

  for (size_t t = 0; t < numTransforms; t++) {
    buffer += ::to_string(*GetTransform(t)) + ident;
  }

  if (buffer.size()) {
    buffer.pop_back();
    transNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  const size_t numFloats = GetNumFloats();
  buffer = ident;
  size_t cc = 0;

  pugi::xml_node floatsNode = hdl.node->append_child(_hkParam);
  floatsNode.append_attribute(_hkName).set_value("floats");
  floatsNode.append_attribute(_hkNumElements).set_value(numFloats);

  for (size_t t = 0; t < numFloats; t++) {
    buffer += std::to_string(GetFloat(t));
    cc++;

    if (cc % 0x10)
      buffer += ' ';
    else
      buffer += ident;
  }

  if (buffer.size()) {
    buffer.pop_back();
    buffer += ident;
    buffer.pop_back();
    floatsNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }
}

void hkaAnimationBindingInternalInterface::ToXML(XMLHandle hdl) const {
  std::string buffer;

  if (hdl.toolset > HK550) {
    pugi::xml_node skelNode = hdl.node->append_child(_hkParam);
    skelNode.append_attribute(_hkName).set_value("originalSkeletonName");

    auto skelName = GetSkeletonName();

    if (!skelName.empty())
      skelNode.append_buffer(skelName.data(), skelName.size());
  }

  pugi::xml_node aniNode = hdl.node->append_child(_hkParam);
  aniNode.append_attribute(_hkName).set_value("animation");

  if (GetAnimation()) {
    PointerToString(GetAnimation()->GetPointer(), buffer);
    aniNode.append_buffer(buffer.c_str(), buffer.size());
  }

  pugi::xml_node transNode = hdl.node->append_child(_hkParam);
  transNode.append_attribute(_hkName).set_value("transformTrackToBoneIndices");
  transNode.append_attribute(_hkNumElements)
      .set_value(GetNumTransformTrackToBoneIndices());

  static const char *ident = "\n\t\t\t\t";
  buffer = ident;
  int cc = 0;

  for (short t : TransformTrackToBoneIndices()) {
    buffer += std::to_string(t);
    cc++;

    if (cc % 0x10)
      buffer += ' ';
    else
      buffer += ident;
  }

  if (buffer.size()) {
    buffer.pop_back();
    buffer += ident;
    buffer.pop_back();
    transNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  pugi::xml_node floatNode = hdl.node->append_child(_hkParam);
  floatNode.append_attribute(_hkName).set_value("floatTrackToFloatSlotIndices");
  floatNode.append_attribute(_hkNumElements)
      .set_value(GetNumFloatTrackToFloatSlotIndices());

  buffer = ident;
  cc = 0;

  for (short t : FloatTrackToFloatSlotIndices()) {
    buffer += std::to_string(t);
    cc++;

    if (cc % 0x10)
      buffer += ' ';
    else
      buffer += ident;
  }

  if (buffer.size()) {
    buffer.pop_back();
    buffer += ident;
    buffer.pop_back();
    floatNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  if (hdl.toolset > HK2011) {
    pugi::xml_node partNode = hdl.node->append_child(_hkParam);
    partNode.append_attribute(_hkName).set_value("partitionIndices");
    partNode.append_attribute(_hkNumElements)
        .set_value(GetNumPartitionIndices());

    buffer = ident;
    cc = 0;

    for (short t : PartitionIndices()) {
      buffer += std::to_string(t);
      cc++;

      if (cc % 0x10)
        buffer += ' ';
      else
        buffer += ident;
    }

    if (buffer.size()) {
      buffer.pop_back();
      buffer += ident;
      buffer.pop_back();
      partNode.append_buffer(buffer.c_str(), buffer.size());
      buffer.clear();
    }
  }

  pugi::xml_node blendNode = hdl.node->append_child(_hkParam);
  blendNode.append_attribute(_hkName).set_value("blendHint");

  BlendHint blendHint = GetBlendHint();
  auto blendName = GetReflectedEnum<
      BlendHint>()[blendHint + (!blendHint || hdl.toolset > HK710 ? 0 : 1)];

  blendNode.append_buffer(blendName.data(), blendName.size());
}

void hkxEnvironmentInternalInterface::ToXML(XMLHandle hdl) const {
  pugi::xml_node varsNode = hdl.node->append_child(_hkParam);
  varsNode.append_attribute(_hkName).set_value("variables");
  varsNode.append_attribute(_hkNumElements).set_value(Size());

  for (auto v : *this) {
    pugi::xml_node varNode = varsNode.append_child(_hkObject);
    pugi::xml_node nameNode = varNode.append_child(_hkParam);
    nameNode.append_attribute(_hkName).set_value(_hkName);
    nameNode.append_buffer(v.name.data(), v.name.size());

    pugi::xml_node valueNode = varNode.append_child(_hkParam);
    valueNode.append_attribute(_hkName).set_value("value");
    valueNode.append_buffer(v.value.data(), v.value.size());
  }
}