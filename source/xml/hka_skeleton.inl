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

#include "internal/hka_skeleton.hpp"

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

  for (auto s : BoneTransforms())
    buffer += ::to_string(*s) + ident;

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

  if (hdl.toolset > HK510) {
    pugi::xml_node floatSlotsNode = hdl.node->append_child(_hkParam);
    floatSlotsNode.append_attribute(_hkName).set_value("floatSlots");
    floatSlotsNode.append_attribute(_hkNumElements)
        .set_value(GetNumFloatSlots());

    for (auto s : FloatSlots()) {
      floatSlotsNode.append_child("hkcstring")
          .append_buffer(s.data(), s.size());
    }
  }

  if (hdl.toolset > HK2011_3) {
    pugi::xml_node partsNode = hdl.node->append_child(_hkParam);
    partsNode.append_attribute(_hkName).set_value("partitions");
    partsNode.append_attribute(_hkNumElements).set_value(GetNumFloatSlots());

    for (auto s : Partitions()) {
      pugi::xml_node hkobj = partsNode.append_child(_hkObject);
      ExportReflectedClass(s, hkobj);
    }
  }
}
