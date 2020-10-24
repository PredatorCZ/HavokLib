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

#include "internal/hka_animatedreferenceframe.hpp"
#include "internal/hka_animation.hpp"
#include "internal/hka_annotationtrack.hpp"

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

  if (hdl.toolset > HK510) {
    pugi::xml_node numFloatsNode = hdl.node->append_child(_hkParam);
    numFloatsNode.append_attribute(_hkName).set_value("numberOfFloatTracks");
    _buff = std::to_string(GetNumOfFloatTracks());
    numFloatsNode.append_buffer(_buff.c_str(), _buff.size());
  }

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
    for (auto a : Annotations()) {
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

    for (auto a : Annotations()) {
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
