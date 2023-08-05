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

#include "internal/hka_animationbinding.hpp"

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

  if (hdl.toolset > HK510) {
    pugi::xml_node floatNode = hdl.node->append_child(_hkParam);
    floatNode.append_attribute(_hkName).set_value(
        "floatTrackToFloatSlotIndices");
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
  }

  if (hdl.toolset > HK2011_3) {
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
  std::string_view blendName = GetReflectedEnum<
      BlendHint>()->names[blendHint + (!blendHint || hdl.toolset > HK700 ? 0 : 1)];

  blendNode.append_buffer(blendName.data(), blendName.size());
}
