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

#include "internal/hkx_environment.hpp"
#include "base.hpp"

void hkxEnvironmentInternalInterface::ToXML(XMLHandle hdl) const {
  pugi::xml_node varsNode = ToXMLArray("variables", Size(), *hdl.node);

  for (auto v : *this) {
    pugi::xml_node varNode = varsNode.append_child(_hkObject);
    ::ToXML(_hkName, v.name, varNode);
    ::ToXML("value", v.value, varNode);
  }
}
