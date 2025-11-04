/*  Havok Format Library
    Copyright(C) 2016-2025 Lukas Cone

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

#include "internal/hkx_attributeholder.hpp"
#include "base.hpp"

void hkxAttributeHolderInternalInterface::ToXML(XMLHandle hdl) const {
  pugi::xml_node groupsNode = ToXMLArray("attributeGroups", Size(), *hdl.node);

  for (const auto &g : *this) {
    auto obj = groupsNode.append_child(_hkObject);
    ::ToXML(_hkName, g->Name(), obj);
    auto attrs = ToXMLArray("attributes", g->Size(), obj);

    for (auto &a : *g) {
      auto obj = attrs.append_child(_hkObject);
      ::ToXML(_hkName, a.name, obj);
      ::ToXML("value", a.value, obj, hdl.toolset);
    }
  }
}
