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

#include "internal/hkx_material.hpp"
#include "base.hpp"

static const char *TEX_TYPES[]{
    /**/ //
    "TEX_UNKNOWN",
    "TEX_DIFFUSE",
    "TEX_REFLECTION",
    "TEX_BUMP",
    "TEX_NORMAL",
    "TEX_DISPLACEMENT",
    "TEX_SPECULAR",
    "TEX_SPECULARANDGLOSS",
    "TEX_OPACITY",
    "TEX_EMISSIVE",
    "TEX_REFRACTION",
    "TEX_GLOSS",
    "TEX_NOTEXPORTED",
};

void hkxMaterialInternalInterface::ToXML(XMLHandle hdl) const {
  hkxAttributeHolderInternalInterface::ToXML(hdl);
  ::ToXML(_hkName, Name(), *hdl.node);

  auto stages = ToXMLArray("stages", GetNumStages(), *hdl.node);

  for (hkxMaterialTextureStage s : Stages()) {
    auto obj = stages.append_child(_hkObject);
    ::ToXML("texture", s.texture, obj, hdl.toolset);
    ::ToXML("usageHint", TEX_TYPES[uint32(s.usageHint)], obj);
    ::ToXML("tcoordChannel", s.tcoordChannel, obj);
  }

  std::string buffer = std::to_string(DiffuseColor());
  ::ToXML("diffuseColor", buffer, *hdl.node);
  buffer = std::to_string(AmbientColor());
  ::ToXML("ambientColor", buffer, *hdl.node);
  buffer = std::to_string(SpecularColor());
  ::ToXML("specularColor", buffer, *hdl.node);
  buffer = std::to_string(EmissiveColor());
  ::ToXML("emissiveColor", buffer, *hdl.node);

  auto subMats = ToXMLArray("subMaterials", GetNumSubMaterials(), *hdl.node);
  buffer.clear();

  for (auto &&s : SubMaterials()) {
    std::string _buffer;
    PointerToString(s->GetPointer(), _buffer);
    buffer += _buffer;
    buffer.push_back(' ');
  }

  static const char *ident = "\n\t\t\t\t";

  if (buffer.size()) {
    buffer.pop_back();
    buffer += ident;
    buffer.pop_back();
    subMats.append_buffer(buffer.c_str(), buffer.size());
  }

  buffer.clear();
  PointerToString(ExtraData()->GetPointer(), buffer);
  ::ToXML("extraData", buffer, *hdl.node);
}
