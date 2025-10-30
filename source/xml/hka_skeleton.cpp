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

#include "internal/hka_skeleton.hpp"
#include "base.hpp"

void hkaSkeletonInternalInterface::ToXML(XMLHandle hdl) const {
  ::ToXML(_hkName, Name(), *hdl.node);
  pugi::xml_node parentNode =
      ToXMLArray("parentIndices", GetNumBones(), *hdl.node);
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

  pugi::xml_node bonesNode = ToXMLArray("bones", GetNumBones(), *hdl.node);

  if (hdl.toolset > HK660) {
    for (auto b : BoneNames()) {
      pugi::xml_node boneNode = bonesNode.append_child(_hkObject);
      ::ToXML(_hkName, b, boneNode);
      ::ToXML("lockTranslation", "false", boneNode);
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

      ::ToXML(_hkName, b, boneNode);
      ::ToXML("lockTranslation", "false", boneNode);

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

  pugi::xml_node refsNode =
      ToXMLArray("referencePose", GetNumBones(), *hdl.node);
  buffer = ident;

  for (auto s : BoneTransforms())
    buffer += ::to_string(*s) + ident;

  if (buffer.size()) {
    buffer.pop_back();
    refsNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  if (hdl.toolset > HK550) {
    pugi::xml_node framesNode =
        ToXMLArray("localFrames", GetNumLocalFrames(), *hdl.node);

    for (auto s : LocalFrames()) {
      pugi::xml_node frameNode = framesNode.append_child(_hkObject);

      std::string _buffer;
      PointerToString(s.localFrame, _buffer);
      ::ToXML("localFrame", _buffer, frameNode);
      ::ToXML("boneIndex", s.boneIndex, frameNode);
    }
  }

  if (hdl.toolset > HK710) {
    pugi::xml_node refsNode =
        ToXMLArray("referenceFloats", GetNumReferenceFloats(), *hdl.node);

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
    pugi::xml_node floatSlotsNode =
        ToXMLArray("floatSlots", GetNumFloatSlots(), *hdl.node);

    for (auto s : FloatSlots()) {
      floatSlotsNode.append_child("hkcstring")
          .append_buffer(s.data(), s.size());
    }
  }

  if (hdl.toolset > HK2011_3) {
    pugi::xml_node partsNode = ToXMLArray("partitions", GetNumPartitions(), *hdl.node);

    for (auto s : Partitions()) {
      pugi::xml_node hkobj = partsNode.append_child(_hkObject);
      ExportReflectedClass(s, hkobj);
    }
  }
}
