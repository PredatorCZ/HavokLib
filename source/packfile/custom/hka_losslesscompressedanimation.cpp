/*  Havok Format Library
    Copyright(C) 2020-2022 Lukas Cone

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

#include "internal/custom/hka_losslesscompressedanimation.hpp"
#include "../hka_animation.hpp"
#include "hka_animation_lossless_compressed.inl"
#include <span>

enum class LosslessTrackType { Identity, Static, Dynamic };

struct hkaLosslessCompressedAnimationMidInterface
    : hkaLosslessCompressedAnimationInternalInterface,
      hkaAnimationMidInterface<hkaAnimationLerpSampler> {
  clgen::hkaLosslessCompressedAnimation::Interface interface;
  size_t transStride;
  size_t rotStride;
  size_t scaleStride;

  hkaLosslessCompressedAnimationMidInterface(clgen::LayoutLookup rules,
                                             char *data)
      : interface {
    data, rules
  } {
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  clgen::hkaAnimation::Interface Anim() const override {
    return interface.BasehkaAnimation();
  }

  void SwapEndian() override {
    hkaAnimationMidInterface<hkaAnimationLerpSampler>::SwapEndian();
    clgen::EndianSwap(interface);
  }

  void Process() override {
    numFrames = interface.NumFrames();
    frameRate = static_cast<uint32>(numFrames / Duration());

    auto GetSingleStride = [](uint16 indexType) {
      LosslessTrackType ttypeX = static_cast<LosslessTrackType>(indexType & 3);
      if (ttypeX == LosslessTrackType::Dynamic) {
        return 1;
      }

      return 0;
    };

    auto GetStride = [&GetSingleStride](const USVector4 &indexType) {
      return GetSingleStride(indexType.X) + GetSingleStride(indexType.Y) +
             GetSingleStride(indexType.Z) + GetSingleStride(indexType.W);
    };

    transStride = 0;
    scaleStride = 0;
    rotStride = 0;

    for (std::span<USVector4> refs(interface.TranslationTypeAndOffsets(),
                                   interface.NumTranslationTypeAndOffsets());
         auto &i : refs) {
      transStride += GetStride(i);
    }

    for (std::span<USVector4> refs(interface.ScaleTypeAndOffsets(),
                                   interface.NumScaleTypeAndOffsets());
         auto &i : refs) {
      scaleStride += GetStride(i);
    }

    for (std::span<uint16> refs(interface.RotationTypeAndOffsets(),
                                interface.NumRotationTypeAndOffsets());
         auto &i : refs) {
      rotStride += GetSingleStride(i);
    }
  }

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override {
    auto GetValue = [frame](uint16 indexType, auto invalStatic,
                            auto invalDynamic, auto identity, size_t stride) {
      LosslessTrackType ttype = static_cast<LosslessTrackType>(indexType & 3);
      uint16 index = indexType >> 2;

      switch (ttype) {
      case LosslessTrackType::Static:
        return invalStatic[index];
      case LosslessTrackType::Dynamic:
        return invalDynamic[index + frame * stride];
      default:
        return decltype(invalStatic[index]){identity};
      }
    };

    auto GetVector = [&GetValue](USVector4 indexType, auto invalStatic,
                                 auto invalDynamic, auto identity,
                                 size_t stride) {
      return Vector4A16(
          GetValue(indexType.X, invalStatic, invalDynamic, identity.X, stride),
          GetValue(indexType.Y, invalStatic, invalDynamic, identity.Y, stride),
          GetValue(indexType.Z, invalStatic, invalDynamic, identity.Z, stride),
          GetValue(indexType.W, invalStatic, invalDynamic, identity.W, stride));
    };

    out.translation =
        GetVector(interface.TranslationTypeAndOffsets()[trackID],
                  interface.StaticTranslations(),
                  interface.DynamicTranslations(), Vector4{}, transStride);
    out.scale = GetVector(interface.ScaleTypeAndOffsets()[trackID],
                          interface.StaticScales(), interface.DynamicScales(),
                          Vector4{1, 1, 1, 0}, scaleStride);
    out.rotation =
        GetValue(interface.RotationTypeAndOffsets()[trackID],
                 interface.StaticRotations(), interface.DynamicRotations(),
                 Vector4{0, 0, 0, 1}, rotStride);
  }
};

hkVirtualClass *
hkaLosslessCompressedAnimationInternalInterface::Create(CRule rule) {
  return new hkaLosslessCompressedAnimationMidInterface{
      clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};
}
