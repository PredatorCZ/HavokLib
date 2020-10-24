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

#include "internal/hka_annotationtrack.hpp"

void hkaAnnotationTrackInternalInterface::ToXML(XMLHandle hdl) const {
  std::string _buff;

  pugi::xml_node tracksNode = hdl.node->append_child(_hkParam);
  tracksNode.append_attribute(_hkName).set_value(
      hdl.toolset < HK600 ? _hkName : "trackName");
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
