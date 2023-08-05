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

#include "base.hpp"
#include "hklib/hka_animation.hpp"
#include "internal/hka_animationbinding.hpp"
#include <span>

#include "hka_animation_binding.inl"

struct hkaAnimationBindingMidInterface : hkaAnimationBindingInternalInterface {
  clgen::hkaAnimationBinding::Interface interface;

  hkaAnimationBindingMidInterface(clgen::LayoutLookup rules, char *data)
      : interface {
    data, rules
  } {
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    clgen::EndianSwap(interface);

    for (std::span<int16> indices(interface.TransformTrackToBoneIndices(),
                                  interface.NumTransformTrackToBoneIndices());
         auto &i : indices) {
      FByteswapper(i);
    }

    for (std::span<int16> indices(interface.FloatTrackToFloatSlotIndices(),
                                  interface.NumFloatTrackToFloatSlotIndices());
         auto &i : indices) {
      FByteswapper(i);
    }

    for (std::span<int16> indices(interface.PartitionIndices(),
                                  interface.NumPartitionIndices());
         auto &i : indices) {
      FByteswapper(i);
    }
  }

  std::string_view GetSkeletonName() const override {
    return interface.SkeletonName();
  }

  const hkaAnimation *GetAnimation() const override {
    return safe_deref_cast<const hkaAnimation>(
        header->GetClass(interface.Animation()));
  }

  BlendHint GetBlendHint() const override { return interface.BlendHint(); }

  size_t GetNumTransformTrackToBoneIndices() const override {
    return interface.NumTransformTrackToBoneIndices();
  }
  int16 GetTransformTrackToBoneIndex(size_t id) const override {
    return interface.TransformTrackToBoneIndices()[id];
  }
  size_t GetNumFloatTrackToFloatSlotIndices() const override {
    return interface.NumFloatTrackToFloatSlotIndices();
  }
  int16 GetFloatTrackToFloatSlotIndex(size_t id) const override {
    return interface.FloatTrackToFloatSlotIndices()[id];
  }
  size_t GetNumPartitionIndices() const override {
    return interface.NumPartitionIndices();
  }
  int16 GetPartitionIndex(size_t id) const override {
    return interface.PartitionIndices()[id];
  }
};

IhkVirtualClass *hkaAnimationBindingInternalInterface::Create(CRule rule) {
  return new hkaAnimationBindingMidInterface{
      clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};
}
