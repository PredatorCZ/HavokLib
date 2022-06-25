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

#include "internal/hk_rootlevelcontainer.hpp"

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

  if (hdl.toolset < HK700)
    buffer.push_back('(');

  PointerToString(pointer->GetPointer(), buffer);

  if (hdl.toolset < HK700) {
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
