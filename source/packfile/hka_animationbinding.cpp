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

#include "hklib/hka_animation.hpp"
#include "internal/hka_animationbinding.hpp"
#include "base.hpp"

namespace {

template <class C>
struct hkaAnimationBinding_t : hkaAnimationBindingInternalInterface {
  uni::Element<C> data;
  // std::unique_ptr<hkxEnvironmentSaver> saver;

  void SetDataPointer(void *ptr) override {
    data = {static_cast<C *>(ptr), false};
  }
  const void *GetPointer() const override { return data.get(); }

  void SwapEndian() override { data->SwapEndian(); }
  es::string_view GetSkeletonName() const override {
    return data->GetSkeletonName();
  }
  const hkaAnimation *GetAnimation() const override {
    return data->GetAnimation(header);
  }
  BlendHint GetBlendHint() const override { return data->GetBlendHint(); }
  size_t GetNumTransformTrackToBoneIndices() const override {
    return data->GetNumTransformTrackToBoneIndices(0);
  }
  int16 GetTransformTrackToBoneIndex(size_t id) const override {
    return data->GetTransformTrackToBoneIndex(id);
  }
  size_t GetNumFloatTrackToFloatSlotIndices() const override {
    return data->GetNumFloatTrackToFloatSlotIndices(0);
  }
  int16 GetFloatTrackToFloatSlotIndex(size_t id) const override {
    return data->GetFloatTrackToFloatSlotIndex(id);
  }
  size_t GetNumPartitionIndices() const override {
    return data->GetNumPartitionIndices(0);
  }
  int16 GetPartitionIndex(size_t id) const override {
    return data->GetPartitionIndex(id);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaAnimationBinding_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;

  const hkaAnimation *GetAnimation(IhkPackFile *header) const {
    return dynamic_cast<const hkaAnimation *>(
        header->GetClass(this->animation));
  }
  BlendHint GetBlendHint() const { return this->blendHint; }
  int16 GetTransformTrackToBoneIndex(size_t id) const {
    return this->TransformTrackToBoneIndices[id];
  }

  GetNum(TransformTrackToBoneIndices);
  GetNum(FloatTrackToFloatSlotIndices);
  GetNum(PartitionIndices);

  AccessMethod(PartitionIndex, (int16)-1) { return this->PartitionIndices[id]; }

  AccessMethod(SkeletonName, (const char *)nullptr) {
    return this->SkeletonName;
  }

  AccessMethod(FloatTrackToFloatSlotIndex, (int16)-1) {
    return this->FloatTrackToFloatSlotIndices[id];
  }

  SwapMethod(PartitionIndices) {
    FByteswapper(GetNumPartitionIndices(0));
    const size_t numPartitions = GetNumPartitionIndices(0);

    for (size_t i = 0; i < numPartitions; i++)
      FByteswapper(this->PartitionIndices[i]);
  }

  SwapMethod(FloatTrackToFloatSlotIndices) {
    FByteswapper(GetNumFloatTrackToFloatSlotIndices(0));
    const size_t numFloatTrackToFloatSlotIndices =
        GetNumFloatTrackToFloatSlotIndices(0);

    for (size_t i = 0; i < numFloatTrackToFloatSlotIndices; i++)
      FByteswapper(this->FloatTrackToFloatSlotIndices[i]);
  }

  void SwapEndian() {
    FByteswapper(GetNumTransformTrackToBoneIndices(0));
    const size_t numTransformTrackToBoneIndices =
        GetNumTransformTrackToBoneIndices(0);

    for (size_t i = 0; i < numTransformTrackToBoneIndices; i++)
      FByteswapper(this->TransformTrackToBoneIndices[i]);

    SwapPartitionIndices(0);
    SwapFloatTrackToFloatSlotIndices(0);
  }
};

template <template <class C> class _ipointer>
struct hkaAnimationBinding500_t_data {
  _ipointer<hkaAnimation> animation;
  _ipointer<int16> TransformTrackToBoneIndices;
  uint32 numTransformTrackToBoneIndices;
  BlendHint blendHint;
};

template <template <class C> class _ipointer>
struct hkaAnimationBinding550_t_data {
  _ipointer<hkaAnimation> animation;
  _ipointer<int16> TransformTrackToBoneIndices;
  uint32 numTransformTrackToBoneIndices;
  _ipointer<int16> FloatTrackToFloatSlotIndices;
  uint32 numFloatTrackToFloatSlotIndices;
  BlendHint blendHint;
};

template <template <class C> class _ipointer>
struct hkaAnimationBinding600_t_data {
  _ipointer<char> SkeletonName;
  _ipointer<hkaAnimation> animation;
  _ipointer<int16> TransformTrackToBoneIndices;
  uint32 numTransformTrackToBoneIndices;
  _ipointer<int16> FloatTrackToFloatSlotIndices;
  uint32 numFloatTrackToFloatSlotIndices;
  BlendHint blendHint;
};

template <template <class C> class _ipointer>
struct hkaAnimationBinding700_t_data : hkReferenceObject<_ipointer> {
  _ipointer<char> SkeletonName;
  _ipointer<hkaAnimation> animation;
  hkArray<int16, _ipointer> TransformTrackToBoneIndices;
  hkArray<int16, _ipointer> FloatTrackToFloatSlotIndices;
  BlendHint blendHint;
};

template <template <class C> class _ipointer>
struct hkaAnimationBinding2012_t_data : hkReferenceObject<_ipointer> {
  _ipointer<char> SkeletonName;
  _ipointer<hkaAnimation> animation;
  hkArray<int16, _ipointer> TransformTrackToBoneIndices;
  hkArray<int16, _ipointer> FloatTrackToFloatSlotIndices;
  hkArray<int16, _ipointer> PartitionIndices;
  BlendHint blendHint;
};

template <template <class C> class _ipointer>
struct hkaAnimationBinding2016_t_data : hkReferenceObject2016<_ipointer> {
  _ipointer<char> SkeletonName;
  _ipointer<hkaAnimation> animation;
  hkArray<int16, _ipointer> TransformTrackToBoneIndices;
  hkArray<int16, _ipointer> FloatTrackToFloatSlotIndices;
  hkArray<int16, _ipointer> PartitionIndices;
  BlendHint blendHint;
};

template <template <class C> class _ipointer>
using hkaAnimationBinding500 =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding500_t_data>;

template <template <class C> class _ipointer>
using hkaAnimationBinding550 =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding550_t_data>;

template <template <class C> class _ipointer>
using hkaAnimationBinding600 =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding600_t_data>;

template <template <class C> class _ipointer>
using hkaAnimationBinding700 =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding700_t_data>;

template <template <class C> class _ipointer>
using hkaAnimationBinding2012 =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding2012_t_data>;

template <template <class C> class _ipointer>
using hkaAnimationBinding2016 =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding2016_t_data>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkaAnimationBindingTier : hkaAnimationBinding700<_ipointer> {};

hkClassTier0Alias(hkaAnimationBinding, 500, 500);
hkClassTier0Alias(hkaAnimationBinding, 500, 510);
hkClassTier0Alias(hkaAnimationBinding, 500, 550);
hkClassTier0Alias(hkaAnimationBinding, 600, 600);
hkClassTier0Alias(hkaAnimationBinding, 600, 610);
hkClassTier0Alias(hkaAnimationBinding, 600, 650);
hkClassTier0Alias(hkaAnimationBinding, 600, 660);
hkClassTier0Alias(hkaAnimationBinding, 2012, 2012_1);
hkClassTier0Alias(hkaAnimationBinding, 2012, 2012_2);
hkClassTier0Alias(hkaAnimationBinding, 2012, 2013);
hkClassTier0Alias(hkaAnimationBinding, 2012, 2014);
hkClassTier0Alias(hkaAnimationBinding, 2012, 2015);
hkClassTier0Alias(hkaAnimationBinding, 2016, 2016);
hkClassTier0Alias(hkaAnimationBinding, 2016, 2017);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterFullClass(hkaAnimationBinding)};
} // namespace

hkVirtualClass *hkaAnimationBindingInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
