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

#include "base.hpp"
#include "internal/hka_meshbinding.hpp"
#include "internal/hkx_mesh.hpp"

#include "hka_mesh_binding.inl"

struct hkaMeshBindingMidInterface : hkaMeshBindingInternalInterface {
  clgen::hkaMeshBinding::Interface interface;

  hkaMeshBindingMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    clgen::EndianSwap(interface);
    auto mappings = interface.Mappings();
    const uint32 numMappings = interface.NumMappings();

    for (uint32 i = 0; i < numMappings; i++, mappings.Next()) {
      clgen::EndianSwap(mappings);
      auto mapping = mappings.Mapping();
      const uint32 numIndices = mappings.NumMapping();

      for (uint32 j = 0; j < numIndices; j++) {
        FByteswapper(mapping[j]);
      }
    }

    es::Matrix44 *boneTms = interface.BoneFromSkinMeshTransforms();
    const uint32 numBoneTMs = interface.NumBoneFromSkinMeshTransforms();

    for (uint32 i = 0; i < numBoneTMs; i++) {
      FByteswapper(boneTms[i]);
    }
  }

  uni::Element<hkxMesh> Mesh() const override {
    auto mesh = interface.Mesh();
    return {hkxMeshInternalInterface::Create(&mesh), true};
  }

  std::string_view SkeletonName() const override {
    return safe_sw(interface.SkeletonName());
  }

  std::string_view Name() const override { return safe_sw(interface.Name()); }

  uni::Element<hkaSkeleton> Skeleton() const override { return {}; }

  size_t NumMappings() const override { return interface.NumMappings(); }

  std::span<const int16> Mappings(size_t at) const override {
    return {interface.Mappings().Next(at).Mapping(),
            interface.Mappings().Next(at).NumMapping()};
  }

  std::span<const es::Matrix44> BoneFromSkinMeshTransforms() const override {
    return {interface.BoneFromSkinMeshTransforms(),
            interface.NumBoneFromSkinMeshTransforms()};
  }
};

CREATE_HK_CLASS(hkaMeshBinding);
