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

#pragma once
#include "hkx_attributeholder.hpp"
#include <span>

enum class TextureType {
  TEX_UNKNOWN,
  TEX_DIFFUSE,
  TEX_REFLECTION,
  TEX_BUMP,
  TEX_NORMAL,
  TEX_DISPLACEMENT,
  TEX_SPECULAR,
  TEX_SPECULARANDGLOSS,
  TEX_OPACITY,
  TEX_EMISSIVE,
  TEX_REFRACTION,
  TEX_GLOSS,
  TEX_NOTEXPORTED,
};

enum class UVMappingAlgorithm : uint32 {
  SRT,
  TRS,
  STYPE_3DSMAX,
  STYPE_MAYA,
};

enum class Transparency : uint8 {
  none = 0,
  alpha = 2,
  additive = 3,
  colorkey = 4,
  subtractive = 9,
};

struct hkxMaterialTextureStage {
  const IhkVirtualClass *texture;
  TextureType usageHint;
  int32 tcoordChannel;
};

struct hkxMaterialProperty {
  uint32 key;
  uint32 value;
};

struct hkxMaterial : hkxAttributeHolder {
  DECLARE_HKCLASS(hkxMaterial)

  virtual std::string_view Name() const = 0;
  virtual size_t GetNumStages() const = 0;
  virtual hkxMaterialTextureStage GetStage(size_t id) const = 0;
  virtual Vector4A16 DiffuseColor() const = 0;
  virtual Vector4A16 AmbientColor() const = 0;
  virtual Vector4A16 SpecularColor() const = 0;
  virtual Vector4A16 EmissiveColor() const = 0;
  virtual size_t GetNumSubMaterials() const = 0;
  virtual uni::Element<const hkxMaterial> GetSubMaterial(size_t id) const = 0;
  virtual const IhkVirtualClass *ExtraData() const = 0;
  virtual Vector2 UVMapScale() const = 0;
  virtual Vector2 UVMapOffset() const = 0;
  virtual float UVMapRotation() const = 0;
  virtual UVMappingAlgorithm UVMapAlgorithm() const = 0;
  virtual float SpecularMultiplier() const = 0;
  virtual float SpecularExponent() const = 0;
  virtual Transparency GetTransparency() const = 0;
  virtual std::span<const hkxMaterialProperty> GetProperties() const = 0;

  typedef uni::VirtualIteratorProxy<hkxMaterial, &hkxMaterial::GetNumStages,
                                    hkxMaterialTextureStage,
                                    &hkxMaterial::GetStage>
      iteratorStages;

  typedef uni::VirtualIteratorProxy<
      hkxMaterial, &hkxMaterial::GetNumSubMaterials,
      uni::Element<const hkxMaterial>, &hkxMaterial::GetSubMaterial>
      iteratorSubMaterials;

  const iteratorStages Stages() const { return iteratorStages(this); }
  const iteratorSubMaterials SubMaterials() const {
    return iteratorSubMaterials(this);
  }
};
