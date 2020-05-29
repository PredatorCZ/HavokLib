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

#pragma once
#include "HavokApi.hpp"
#include "datas/disabler.hpp"
#include "hkInternalInterfaces.h"
#include "hkObjectBase.h"

template <class C>
struct hkaAnimationBinding_t : hkaAnimationBindingInternalInterface {
  C *Data;
  hkClassConstructor(hkaAnimationBinding_t);
  void SwapEndian() override { Data->SwapEndian(); }
  es::string_view GetSkeletonName() const override {
    return Data->GetSkeletonName();
  }
  const hkaAnimation *GetAnimation() const override {
    return Data->GetAnimation(header);
  }
  BlendHint GetBlendHint() const override { return Data->GetBlendHint(); }
  size_t GetNumTransformTrackToBoneIndices() const override {
    return Data->GetNumTransformTrackToBoneIndices();
  }
  int16 GetTransformTrackToBoneIndex(size_t id) const override {
    return Data->GetTransformTrackToBoneIndex(id);
  }
  size_t GetNumFloatTrackToFloatSlotIndices() const override {
    return Data->GetNumFloatTrackToFloatSlotIndices();
  }
  int16 GetFloatTrackToFloatSlotIndex(size_t id) const override {
    return Data->GetFloatTrackToFloatSlotIndex(id);
  }
  size_t GetNumPartitionIndices() const override {
    return Data->GetNumPartitionIndices();
  }
  int16 GetPartitionIndex(size_t id) const override {
    return Data->GetPartitionIndex(id);
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
    return this->transformTrackToBoneIndices[id];
  }
  int16 GetFloatTrackToFloatSlotIndex(size_t id) const {
    return this->floatTrackToFloatSlotIndices[id];
  }

  ADD_DISABLERS(_parent<_ipointer>, noPartitions, noSkeletonName);

  enablePtrPair(transformTrackToBoneIndices)
      GetNumTransformTrackToBoneIndices() const {
    return this->numTransformTrackToBoneIndices;
  }
  enablehkArray(transformTrackToBoneIndices)
      GetNumTransformTrackToBoneIndices() const {
    return this->transformTrackToBoneIndices.count;
  }
  enablePtrPairRef(transformTrackToBoneIndices)
      GetNumTransformTrackToBoneIndices() {
    return this->numTransformTrackToBoneIndices;
  }
  enablehkArrayRef(transformTrackToBoneIndices)
      GetNumTransformTrackToBoneIndices() {
    return this->transformTrackToBoneIndices.count;
  }

  enablePtrPair(floatTrackToFloatSlotIndices)
      GetNumFloatTrackToFloatSlotIndices() const {
    return this->numFloatTrackToFloatSlotIndices;
  }
  enablehkArray(floatTrackToFloatSlotIndices)
      GetNumFloatTrackToFloatSlotIndices() const {
    return this->floatTrackToFloatSlotIndices.count;
  }
  enablePtrPairRef(floatTrackToFloatSlotIndices)
      GetNumFloatTrackToFloatSlotIndices() {
    return this->numFloatTrackToFloatSlotIndices;
  }
  enablehkArrayRef(floatTrackToFloatSlotIndices)
      GetNumFloatTrackToFloatSlotIndices() {
    return this->floatTrackToFloatSlotIndices.count;
  }

  disabledFunction(noPartitions, size_t) GetNumPartitionIndices() const {
    return 0;
  }
  enabledFunction(noPartitions, size_t) GetNumPartitionIndices() const {
    return GetNumPartitionIndices();
  }
  enabledFunction(noPartitions, uint32 &) GetNumPartitionIndices() {
    return this->partitionIndices.count;
  }

  disabledFunction(noPartitions, int16) GetPartitionIndex(size_t) const {
    return -1;
  }
  enabledFunction(noPartitions, int16) GetPartitionIndex(size_t id) const {
    return this->partitionIndices[id];
  }

  disabledFunction(noSkeletonName, const char *) GetSkeletonName() const {
    return nullptr;
  }
  enabledFunction(noSkeletonName, const char *) GetSkeletonName() const {
    return this->skeletonName;
  }

  disabledFunction(noPartitions, void) SwapPartitionIndices() {}
  enabledFunction(noPartitions, void) SwapPartitionIndices() {
    FByteswapper(GetNumPartitionIndices());
    const size_t numPartitions = GetNumPartitionIndices();

    for (size_t i = 0; i < numPartitions; i++)
      FByteswapper(this->partitionIndices[i]);
  }

  void SwapEndian() {
    FByteswapper(GetNumTransformTrackToBoneIndices());
    const size_t numTransformTrackToBoneIndices =
        GetNumTransformTrackToBoneIndices();

    for (size_t i = 0; i < numTransformTrackToBoneIndices; i++)
      FByteswapper(this->transformTrackToBoneIndices[i]);

    FByteswapper(GetNumFloatTrackToFloatSlotIndices());
    const size_t numFloatTrackToFloatSlotIndices =
        GetNumFloatTrackToFloatSlotIndices();

    for (size_t i = 0; i < numFloatTrackToFloatSlotIndices; i++)
      FByteswapper(this->floatTrackToFloatSlotIndices[i]);

    SwapPartitionIndices();
  }
};

template <template <class C> class _ipointer>
struct hkaAnimationBinding550_t_data {
  _ipointer<hkaAnimation> animation;
  _ipointer<int16> transformTrackToBoneIndices;
  uint32 numTransformTrackToBoneIndices;
  _ipointer<int16> floatTrackToFloatSlotIndices;
  uint32 numFloatTrackToFloatSlotIndices;
  BlendHint blendHint;
  void noPartitions();
  void noSkeletonName();
};

template <template <class C> class _ipointer>
using hkaAnimationBinding550_t =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding550_t_data>;
template <template <class C> class _ipointer>
struct hkaAnimationBinding550_rp_t : hkaAnimationBinding550_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimationBinding660_t_data {
  _ipointer<char> skeletonName;
  _ipointer<hkaAnimation> animation;
  _ipointer<int16> transformTrackToBoneIndices;
  uint32 numTransformTrackToBoneIndices;
  _ipointer<int16> floatTrackToFloatSlotIndices;
  uint32 numFloatTrackToFloatSlotIndices;
  BlendHint blendHint;
  void noPartitions();
};

template <template <class C> class _ipointer>
using hkaAnimationBinding660_t =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding660_t_data>;
template <template <class C> class _ipointer>
struct hkaAnimationBinding660_rp_t : hkaAnimationBinding660_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimationBinding710_t_data : hkReferenceObject<_ipointer> {
  _ipointer<char> skeletonName;
  _ipointer<hkaAnimation> animation;
  hkArray<int16, _ipointer> transformTrackToBoneIndices;
  hkArray<int16, _ipointer> floatTrackToFloatSlotIndices;
  BlendHint blendHint;
  void noPartitions();
};

template <template <class C> class _ipointer>
using hkaAnimationBinding710_t =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding710_t_data>;
template <template <class C> class _ipointer>
struct hkaAnimationBinding710_rp_t : hkaAnimationBinding710_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimationBinding2010_t : hkaAnimationBinding710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationBinding2010_rp_t : hkaAnimationBinding710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationBinding2011_t : hkaAnimationBinding710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationBinding2011_rp_t : hkaAnimationBinding710_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimationBinding2012_t_data : hkReferenceObject<_ipointer> {
  _ipointer<char> skeletonName;
  _ipointer<hkaAnimation> animation;
  hkArray<int16, _ipointer> transformTrackToBoneIndices;
  hkArray<int16, _ipointer> floatTrackToFloatSlotIndices;
  hkArray<int16, _ipointer> partitionIndices;
  BlendHint blendHint;
};

template <template <class C> class _ipointer>
using hkaAnimationBinding2012_t =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding2012_t_data>;
template <template <class C> class _ipointer>
struct hkaAnimationBinding2012_rp_t : hkaAnimationBinding2012_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimationBinding2013_t : hkaAnimationBinding2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationBinding2013_rp_t : hkaAnimationBinding2012_rp_t<_ipointer> {
};
template <template <class C> class _ipointer>
struct hkaAnimationBinding2014_t : hkaAnimationBinding2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationBinding2014_rp_t : hkaAnimationBinding2012_rp_t<_ipointer> {
};
template <template <class C> class _ipointer>
struct hkaAnimationBinding2015_t : hkaAnimationBinding2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationBinding2015_rp_t : hkaAnimationBinding2012_rp_t<_ipointer> {
};

template <template <class C> class _ipointer>
struct hkaAnimationBinding2016_t_data : hkReferenceObject2016<_ipointer> {
  _ipointer<char> skeletonName;
  _ipointer<hkaAnimation> animation;
  hkArray<int16, _ipointer> transformTrackToBoneIndices;
  hkArray<int16, _ipointer> floatTrackToFloatSlotIndices;
  hkArray<int16, _ipointer> partitionIndices;
  BlendHint blendHint;
};

template <template <class C> class _ipointer>
using hkaAnimationBinding2016_t =
    hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding2016_t_data>;
template <template <class C> class _ipointer>
struct hkaAnimationBinding2016_rp_t : hkaAnimationBinding2016_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimationBinding2017_t : hkaAnimationBinding2016_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationBinding2017_rp_t : hkaAnimationBinding2016_rp_t<_ipointer> {
};