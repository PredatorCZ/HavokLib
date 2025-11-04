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
#include "hkx_attribute_holder.hpp"

#include "hkx_material.inl"

struct hkxMaterialMidInterface : hkxMaterialInternalInterface,
                                 hkxAttributeHolderMidInterface {
  clgen::hkxMaterial::Interface interface;

  hkxMaterialMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxMaterialMidInterface(const clgen::hkxMaterial::Interface &interface)
      : interface{interface} {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    hkxAttributeHolderMidInterface::SwapEndian();
    clgen::EndianSwap(interface);
    auto stages = interface.Stages();
    const uint32 numStages = interface.NumStages();

    for (uint32 s = 0; s < numStages; s++, stages.Next()) {
      clgen::EndianSwap(stages);
    }

    Vector4A16 tmp = interface.DiffuseColor();
    FByteswapper(tmp);
    interface.DiffuseColor(tmp);

    tmp = interface.AmbientColor();
    FByteswapper(tmp);
    interface.AmbientColor(tmp);

    tmp = interface.SpecularColor();
    FByteswapper(tmp);
    interface.SpecularColor(tmp);

    tmp = interface.EmissiveColor();
    FByteswapper(tmp);
    interface.EmissiveColor(tmp);

    Vector2 tmp2 = interface.UvMapOffset();
    FByteswapper(tmp2);
    interface.UvMapOffset(tmp2);

    tmp2 = interface.UvMapScale();
    FByteswapper(tmp2);
    interface.UvMapScale(tmp2);

    auto props = interface.Properties();
    const uint32 numProps = interface.NumProperties();

    for (uint32 p = 0; p < numProps; p++) {
      FByteswapper(props[p].key);
      FByteswapper(props[p].value);
    }
  }

  clgen::hkxAttributeHolder::Interface AttribHolder() const override {
    return interface.BasehkxAttributeHolder();
  }

  std::string_view Name() const override { return safe_sw(interface.Name()); }
  size_t GetNumStages() const override { return interface.NumStages(); }
  hkxMaterialTextureStage GetStage(size_t id) const override {
    auto stage = interface.Stages().Next(id);
    return {
        .texture = static_cast<const hkxMaterialInternalInterface *>(this)
                       ->header->GetClass(stage.Texture().Object()),
        .usageHint = stage.UsageHint(),
        .tcoordChannel = stage.TcoordChannel(),
    };
  }
  Vector4A16 DiffuseColor() const override { return interface.DiffuseColor(); }
  Vector4A16 AmbientColor() const override { return interface.AmbientColor(); }
  Vector4A16 SpecularColor() const override {
    return interface.SpecularColor();
  }
  Vector4A16 EmissiveColor() const override {
    return interface.EmissiveColor();
  }
  size_t GetNumSubMaterials() const override {
    return interface.NumSubMaterials();
  }
  uni::Element<const hkxMaterial> GetSubMaterial(size_t id) const override {
    auto subMat = interface.SubMaterials().Next(id);
    return {new hkxMaterialMidInterface(**subMat), true};
  }
  const IhkVirtualClass *ExtraData() const override {
    const void *ptr = nullptr;
    if (interface.LayoutVersion() >= HK700) {
      ptr = interface.ExtraDataHK700().data;
    } else {
      ptr = interface.ExtraData().Object();
    }
    return static_cast<const hkxMaterialInternalInterface *>(this)
        ->header->GetClass(ptr);
  }

  Vector2 UVMapScale() const override {
    return interface.LayoutVersion() >= HK2012_1 ? interface.UvMapScale()
                                                 : Vector2(1);
  }

  Vector2 UVMapOffset() const override { return interface.UvMapOffset(); }

  float UVMapRotation() const override { return interface.UvMapRotation(); }

  UVMappingAlgorithm UVMapAlgorithm() const override {
    return interface.LayoutVersion() >= HK2012_1
               ? interface.UvMapAlgorithm()
               : UVMappingAlgorithm::STYPE_3DSMAX;
  }

  float SpecularMultiplier() const override {
    return interface.LayoutVersion() >= HK2012_1
               ? interface.SpecularMultiplier()
               : 1;
  }

  float SpecularExponent() const override {
    return interface.LayoutVersion() >= HK2012_1 ? interface.SpecularExponent()
                                                 : 1;
  }

  Transparency GetTransparency() const override {
    return interface.Transparency();
  }

  std::span<const hkxMaterialProperty> GetProperties() const override {
    return {interface.Properties(), interface.NumProperties()};
  }
};

hkxMaterialInternalInterface *
hkxMaterialInternalInterface::Create(void *interface) {
  return new hkxMaterialMidInterface(
      *static_cast<clgen::hkxMaterial::Interface *>(interface));
}

CREATE_HK_CLASS(hkxMaterial);
