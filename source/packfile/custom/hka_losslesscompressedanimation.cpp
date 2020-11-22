/*  Havok Format Library
    Copyright(C) 2020 Lukas Cone

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

namespace {

enum class LosslessTrackType { Identity, Static, Dynamic };

template <class C>
struct hkaLosslessCompressedAnimation_t
    : hkaAnimation_t<typename C::parentClass, hkaAnimationLerpSampler> {
  typedef C value_type;
  typedef hkaAnimation_t<typename C::parentClass, hkaAnimationLerpSampler>
      parent;

  size_t transStride;
  size_t rotStride;
  size_t scaleStride;

  value_type *Data() { return static_cast<value_type *>(this->data.get()); }
  const value_type *Data() const {
    return static_cast<const value_type *>(this->data.get());
  }

  void Process() override {
    auto data = Data();
    this->numFrames = data->numFrames;
    this->frameRate = static_cast<uint32>(this->numFrames / this->Duration());

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

    size_t numItems = data->translationTypeAndOffsets.count;
    transStride = 0;

    for (size_t t = 0; t < numItems; t++) {
      transStride += GetStride(data->translationTypeAndOffsets[t]);
    }

    numItems = data->scaleTypeAndOffsets.count;
    scaleStride = 0;

    for (size_t t = 0; t < numItems; t++) {
      scaleStride += GetStride(data->scaleTypeAndOffsets[t]);
    }

    numItems = data->rotationTypeAndOffsets.count;
    rotStride = 0;

    for (size_t t = 0; t < numItems; t++) {
      rotStride += GetSingleStride(data->rotationTypeAndOffsets[t]);
    }
  }

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override {
    auto data = Data();

    auto GetValue = [frame](uint16 indexType, auto &invalStatic,
                            auto &invalDynamic, auto identity, size_t stride) {
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

    auto GetVector = [&GetValue](
                         const USVector4 &indexType, auto &invalStatic,
                         auto &invalDynamic, auto identity, size_t stride) {
      return Vector4A16(
          GetValue(indexType.X, invalStatic, invalDynamic, identity.X, stride),
          GetValue(indexType.Y, invalStatic, invalDynamic, identity.Y, stride),
          GetValue(indexType.Z, invalStatic, invalDynamic, identity.Z, stride),
          GetValue(indexType.W, invalStatic, invalDynamic, identity.W, stride));
    };

    out.translation = GetVector(
        data->translationTypeAndOffsets[trackID], data->staticTranslations,
        data->dynamicTranslations, Vector4{}, transStride);
    out.scale =
        GetVector(data->scaleTypeAndOffsets[trackID], data->staticScales,
                  data->dynamicScales, Vector4{1, 1, 1, 0}, scaleStride);
    out.rotation =
        GetValue(data->rotationTypeAndOffsets[trackID], data->staticRotations,
                 data->dynamicRotations, Vector4{0, 0, 0, 1}, rotStride);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaLosslessCompressedAnimation2014_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
  hkArray<float, _ipointer> dynamicTranslations;
  hkArray<float, _ipointer> staticTranslations;
  hkArray<USVector4, _ipointer> translationTypeAndOffsets;
  hkArray<Vector4A16, _ipointer> dynamicRotations;
  hkArray<Vector4A16, _ipointer> staticRotations;
  hkArray<uint16, _ipointer> rotationTypeAndOffsets;
  hkArray<float, _ipointer> dynamicScales;
  hkArray<float, _ipointer> staticScales;
  hkArray<USVector4, _ipointer> scaleTypeAndOffsets;
  hkArray<float, _ipointer> floats;
  uint32 numFrames;
};

template <template <class C> class _ipointer, bool rp>
using hkaLosslessCompressedAnimation2014 =
    hkaLosslessCompressedAnimation2014_t_data<_ipointer, hkaAnimation2011, rp>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkaLosslessCompressedAnimationTier
    : hkaLosslessCompressedAnimation2014<_ipointer, rp> {};

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterVersionedClass(hkaLosslessCompressedAnimation, HK2014)};
} // namespace

hkVirtualClass *
hkaLosslessCompressedAnimationInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
