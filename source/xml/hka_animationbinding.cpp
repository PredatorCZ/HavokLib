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
#include "base.hpp"
#include "internal/hka_animation.hpp"

void hkaAnimationBindingInternalInterface::ToXML(XMLHandle hdl) const {
  std::string buffer;

  if (hdl.toolset > HK550) {
    ::ToXML("originalSkeletonName", GetSkeletonName(), *hdl.node);
  }

  pugi::xml_node aniNode = hdl.node->append_child(_hkParam);
  aniNode.append_attribute(_hkName).set_value("animation");

  if (GetAnimation()) {
    PointerToString(GetAnimation()->GetPointer(), buffer);
    aniNode.append_buffer(buffer.c_str(), buffer.size());
  }

  pugi::xml_node transNode =
      ToXMLArray("transformTrackToBoneIndices",
                 GetNumTransformTrackToBoneIndices(), *hdl.node);

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
    pugi::xml_node floatNode =
        ToXMLArray("floatTrackToFloatSlotIndices",
                   GetNumFloatTrackToFloatSlotIndices(), *hdl.node);

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
    pugi::xml_node partNode =
        ToXMLArray("partitionIndices", GetNumPartitionIndices(), *hdl.node);

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

  BlendHint blendHint = GetBlendHint();
  std::string_view blendName =
      GetReflectedEnum<BlendHint>()
          ->names[blendHint + (!blendHint || hdl.toolset > HK700 ? 0 : 1)];

  ::ToXML("blendHint", blendName, *hdl.node);
}
