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

#include "internal/hka_animationcontainer.hpp"
#include "base.hpp"
#include "hklib/hka_boneattachment.hpp"
#include "hklib/hka_meshbinding.hpp"
#include "internal/hka_animation.hpp"
#include "internal/hka_animationbinding.hpp"
#include "internal/hka_skeleton.hpp"

void hkaAnimationContainerInternalInterface::ToXML(XMLHandle hdl) const {
  pugi::xml_node skelNode =
      ToXMLArray("skeletons", GetNumSkeletons(), *hdl.node);

  pugi::xml_node animNode =
      ToXMLArray("animations", GetNumAnimations(), *hdl.node);

  pugi::xml_node bindNode = ToXMLArray("bindings", GetNumBindings(), *hdl.node);

  pugi::xml_node attNode =
      ToXMLArray("attachments", GetNumAttachments(), *hdl.node);

  pugi::xml_node skinNode = ToXMLArray("skins", GetNumSkins(), *hdl.node);

  std::string buffer;

  for (auto s : Skeletons()) {
    PointerToString(s->GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    skelNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  for (auto s : Animations()) {
    PointerToString(s->GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    animNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  for (auto s : Bindings()) {
    if (!s)
      continue;

    PointerToString(s->GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    bindNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  for (auto s : Attachments()) {
    PointerToString(s->GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    attNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }

  for (auto s : MeshBinds()) {
    PointerToString(s->GetPointer(), buffer);
    buffer += ' ';
  }

  if (buffer.size()) {
    buffer.pop_back();
    skinNode.append_buffer(buffer.c_str(), buffer.size());
    buffer.clear();
  }
}
