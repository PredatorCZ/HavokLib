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

#include "internal/hkx_environment.hpp"

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
