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

#include "internal/hka_interleavedanimation.hpp"

void hkaInterleavedAnimationInternalInterface::ToXML(XMLHandle hdl) const {
  hkaAnimationInternalInterface::ToXML(hdl);

  const size_t numTransforms = GetNumTransforms();
  static const char *ident = "\n\t\t\t\t";
  std::string buffer = ident;

  pugi::xml_node transNode = hdl.node->append_child(_hkParam);
  transNode.append_attribute(_hkName).set_value("transforms");
  transNode.append_attribute(_hkNumElements).set_value(numTransforms);

  for (size_t t = 0; t < numTransforms; t++) {
    buffer += ::to_string(*GetTransform(t)) + ident;
  }

  if (buffer.size()) {
    buffer.pop_back();
    transNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  const size_t numFloats = GetNumFloats();
  buffer = ident;
  size_t cc = 0;

  pugi::xml_node floatsNode = hdl.node->append_child(_hkParam);
  floatsNode.append_attribute(_hkName).set_value("floats");
  floatsNode.append_attribute(_hkNumElements).set_value(numFloats);

  for (size_t t = 0; t < numFloats; t++) {
    buffer += std::to_string(GetFloat(t));
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
    floatsNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }
}
