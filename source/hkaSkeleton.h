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
#include "datas/disabler.hpp"
#include "hkInternalInterfaces.h"
#include "hkObjectBase.h"

template <class C> struct hkaSkeleton_t : hkaSkeletonInternalInterface {
  C *Data;
  hkClassConstructor(hkaSkeleton_t<C>);

  size_t GetNumBones() const override { return Data->GetNumBones(); }
  es::string_view GetBoneName(size_t id) const override {
    return Data->GetBoneName(id);
  }
  const hkQTransform *GetBoneTM(size_t id) const override {
    if (id < Data->GetNumTransforms())
      return Data->GetBoneTM(id);

    return nullptr;
  }
  int16 GetBoneParentID(size_t id) const override {
    if (id < Data->GetNumParentIndices())
      return Data->GetBoneParentID(id);

    return -1;
  }
  es::string_view Name() const override { return Data->GetSkeletonName(); };
  size_t GetNumFloatSlots() const override { return Data->GetNumFloatSlots(); };
  es::string_view GetFloatSlot(size_t id) const override {
    return Data->GetFloatSlot(id);
  };
  size_t GetNumLocalFrames() const override {
    return Data->GetNumLocalFrames();
  };
  hkLocalFrameOnBone GetLocalFrame(size_t id) const override {
    return Data->GetLocalFrame(id);
  };
  size_t GetNumPartitions() const { return Data->GetNumPartitions(); };
  hkaPartition GetPartition(size_t id) const override {
    return Data->GetPartition(id);
  };
  size_t GetNumReferenceFloats() const override {
    return Data->GetNumReferenceFloats();
  };
  float GetReferenceFloat(size_t id) const override {
    return Data->GetReferenceFloat(id);
  }

  void SwapEndian() override { Data->SwapEndian(); }
};

template <template <class C> class _ipointer> struct hkLocalFrameOnBone_t {
  _ipointer<hkLocalFrame> localFrame;
  int32 boneIndex;
};

template <template <class C> class _ipointer> struct hkLocalFrameOnBone2_t {
  _ipointer<hkLocalFrame> localFrame;
  int16 boneIndex;
};

template <template <class C> class _ipointer> struct hkaBone_t {
  _ipointer<char> name;
  int32 lockTranslation;
};

template <template <class C> class _ipointer> struct hkaPartition_t {
  _ipointer<char> Name;
  int16 startBoneIndex;
  uint16 numBones;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaSkeleton_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;
  ADD_DISABLERS(parent_class, noPartitions, noReferenceFloats, noLocalFrames);

  enablePtrPair(bones) GetNumBones() const { return this->numBones; }
  enablehkArray(bones) GetNumBones() const { return this->bones.count; }
  enablePtrPairRef(bones) GetNumBones() { return this->numBones; }
  enablehkArrayRef(bones) GetNumBones() { return this->bones.count; }
  enablePtrPair(bones) GetNumParentIndices() const {
    return this->numParentIndicies;
  }
  enablehkArray(bones) GetNumParentIndices() const {
    return this->parentIndicies.count;
  }
  enablePtrPairRef(bones) GetNumParentIndices() {
    return this->numParentIndicies;
  }
  enablehkArrayRef(bones) GetNumParentIndices() {
    return this->parentIndicies.count;
  }

  enablePtrPair(bones) GetNumTransforms() const { return this->numTransforms; }
  enablehkArray(bones) GetNumTransforms() const {
    return this->transforms.count;
  }
  enablePtrPairRef(bones) GetNumTransforms() { return this->numTransforms; }
  enablehkArrayRef(bones) GetNumTransforms() { return this->transforms.count; }

  disabledFunction(noLocalFrames, size_t) GetNumLocalFrames() const {
    return 0;
  };
  enabledFunction(noLocalFrames, size_t) GetNumLocalFrames() const {
    return _GetNumLocalFrames();
  };

  disabledFunction(noLocalFrames, uint32 &) GetNumLocalFrames() {
    static uint32 _dummy;
    return _dummy = 0;
  };
  enabledFunction(noLocalFrames, uint32 &) GetNumLocalFrames() {
    return _GetNumLocalFrames();
  };

  enablePtrPair(floatSlots) GetNumFloatSlots() const {
    return this->numFloatSlots;
  }
  enablehkArray(floatSlots) GetNumFloatSlots() const {
    return this->floatSlots.count;
  }
  enablePtrPairRef(floatSlots) GetNumFloatSlots() {
    return this->numFloatSlots;
  }
  enablehkArrayRef(floatSlots) GetNumFloatSlots() {
    return this->floatSlots.count;
  }

  enablePtrPair(localFrames) _GetNumLocalFrames() const {
    return this->numLocalFrames;
  }
  enablehkArray(localFrames) _GetNumLocalFrames() const {
    return this->localFrames.count;
  }
  enablePtrPairRef(localFrames) _GetNumLocalFrames() {
    return this->numLocalFrames;
  }
  enablehkArrayRef(localFrames) _GetNumLocalFrames() {
    return this->localFrames.count;
  }

  enablePtrPairArg(bones, const char *) GetBoneName(size_t id) const {
    return this->bones[id]->name;
  }
  enablehkArrayArg(bones, const char *) GetBoneName(size_t id) const {
    return this->bones[id].name;
  }

  const hkQTransform *GetBoneTM(size_t id) const {
    return &this->transforms[id];
  }
  int16 GetBoneParentID(size_t id) const { return this->parentIndicies[id]; }
  const char *GetSkeletonName() const { return this->Name; };
  const char *GetFloatSlot(size_t id) const { return this->floatSlots[id]; };

  enabledFunction(noLocalFrames, const hkLocalFrameOnBone)
      GetLocalFrame(size_t id) const {
    return hkLocalFrameOnBone{this->localFrames[id].localFrame,
                              this->localFrames[id].boneIndex};
  };

  disabledFunction(noLocalFrames, const hkLocalFrameOnBone)
      GetLocalFrame(size_t id) const {
    return hkLocalFrameOnBone{};
  };

  disabledFunction(noPartitions, size_t) GetNumPartitions() const { return 0; }
  enabledFunction(noPartitions, size_t) GetNumPartitions() const {
    return GetNumPartitions();
  }
  enabledFunction(noPartitions, uint32 &) GetNumPartitions() {
    return this->partitions.count;
  }

  disabledFunction(noReferenceFloats, size_t) GetNumReferenceFloats() const {
    return 0;
  }
  enabledFunction(noReferenceFloats, size_t) GetNumReferenceFloats() const {
    return this->referenceFloats.count;
  }

  disabledFunction(noPartitions, const hkaPartition)
      GetPartition(size_t id) const {
    return hkaPartition{};
  }
  enabledFunction(noPartitions, const hkaPartition)
      GetPartition(size_t id) const {
    return hkaPartition{static_cast<const char *>(this->partitions[id].Name),
                        this->partitions[id].startBoneIndex,
                        this->partitions[id].numBones};
  }

  disabledFunction(noReferenceFloats, const float)
      GetReferenceFloat(size_t id) const {
    return 0.0f;
  }
  enabledFunction(noReferenceFloats, const float)
      GetReferenceFloat(size_t id) const {
    return this->referenceFloats[id];
  }

  disabledFunction(noLocalFrames, void) SwapLocalFrames() {}
  enabledFunction(noLocalFrames, void) SwapLocalFrames() {
    FByteswapper(GetNumLocalFrames());
    const size_t numLocalFrames = GetNumLocalFrames();

    for (size_t i = 0; i < numLocalFrames; i++)
      FByteswapper(this->localFrames[i].boneIndex);
  }
  disabledFunction(noReferenceFloats, void) SwapRefFloats() {}
  enabledFunction(noReferenceFloats, void) SwapRefFloats() {
    FByteswapper(GetNumFloatSlots());
    const size_t numRefFloats = GetNumFloatSlots();

    for (size_t i = 0; i < numRefFloats; i++)
      FByteswapper(this->referenceFloats[i]);
  }
  disabledFunction(noPartitions, void) SwapPartitions() {}
  enabledFunction(noPartitions, void) SwapPartitions() {
    FByteswapper(GetNumPartitions());
    const size_t numPartitions = GetNumPartitions();

    for (size_t i = 0; i < numPartitions; i++) {
      FByteswapper(this->partitions[i].startBoneIndex);
      FByteswapper(this->partitions[i].numBones);
    }
  }

  void SwapEndian() {
    FByteswapper(GetNumBones());
    FByteswapper(GetNumParentIndices());
    FByteswapper(GetNumTransforms());

    const size_t numBones = GetNumBones();

    short *pData = this->parentIndicies;
    short *pDataEnd = pData + numBones;

    for (; pData != pDataEnd; pData++)
      FByteswapper(*pData);

    float *data = reinterpret_cast<float *>(
        static_cast<hkQTransform *>(this->transforms));
    float *dataEnd = data + numBones * 12;

    for (; data != dataEnd; data++)
      FByteswapper(*data);

    SwapLocalFrames();
    SwapRefFloats();
    SwapPartitions();
  }
};

template <template <class C> class _ipointer>
struct hkaSkeleton550_t_sharedData {
  _ipointer<char> Name;
  _ipointer<short> parentIndicies;
  uint32 numParentIndicies;
  _ipointer<_ipointer<hkaBone_t<_ipointer>>> bones;
  uint32 numBones;
  _ipointer<hkQTransform> transforms;
  uint32 numTransforms;
  _ipointer<_ipointer<char>> floatSlots;
  uint32 numFloatSlots;

  // uint32 numLocalFrames;
  static _ipointer<hkLocalFrameOnBone_t<_ipointer>> localFrames;

  void noPartitions();
  void noReferenceFloats();
  void noLocalFrames();
};

template <template <class C> class _ipointer>
struct hkaSkeleton660_t_sharedData {
  _ipointer<char> Name;
  _ipointer<short> parentIndicies;
  uint32 numParentIndicies;
  _ipointer<_ipointer<hkaBone_t<_ipointer>>> bones;
  uint32 numBones;
  _ipointer<hkQTransform> transforms;
  uint32 numTransforms;
  _ipointer<_ipointer<char>> floatSlots;
  uint32 numFloatSlots;
  _ipointer<hkLocalFrameOnBone_t<_ipointer>> localFrames;
  uint32 numLocalFrames;

  void noPartitions();
  void noReferenceFloats();
};

template <template <class C> class _ipointer>
using hkaSkeleton550_t =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton550_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaSkeleton550_rp_t : hkaSkeleton550_t<_ipointer> {};

template <template <class C> class _ipointer>
using hkaSkeleton660_t =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton660_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaSkeleton660_rp_t : hkaSkeleton660_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaSkeleton710_t_sharedData : hkReferenceObject<_ipointer> {
  _ipointer<char> Name;
  hkArray<int16, _ipointer> parentIndicies;
  hkArray<hkaBone_t<_ipointer>, _ipointer> bones;
  hkArray<hkQTransform, _ipointer> transforms;
  hkArray<_ipointer<char>, _ipointer> floatSlots;
  hkArray<hkLocalFrameOnBone_t<_ipointer>, _ipointer> localFrames;

  void noPartitions();
  void noReferenceFloats();
};

template <template <class C> class _ipointer>
using hkaSkeleton710_t =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton710_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaSkeleton710_rp_t : hkaSkeleton710_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaSkeleton2010_t_sharedData : hkReferenceObject<_ipointer> {
  _ipointer<char> Name;
  hkArray<int16, _ipointer> parentIndicies;
  hkArray<hkaBone_t<_ipointer>, _ipointer> bones;
  hkArray<hkQTransform, _ipointer> transforms;
  hkArray<float, _ipointer> referenceFloats;
  hkArray<_ipointer<char>, _ipointer> floatSlots;
  hkArray<hkLocalFrameOnBone_t<_ipointer>, _ipointer> localFrames;

  void noPartitions();
};

template <template <class C> class _ipointer>
using hkaSkeleton2010_t =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton2010_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaSkeleton2010_rp_t : hkaSkeleton2010_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaSkeleton2011_t_sharedData : hkReferenceObject<_ipointer> {
  _ipointer<char> Name;
  hkArray<int16, _ipointer> parentIndicies;
  hkArray<hkaBone_t<_ipointer>, _ipointer> bones;
  hkArray<hkQTransform, _ipointer> transforms;
  hkArray<float, _ipointer> referenceFloats;
  hkArray<_ipointer<char>, _ipointer> floatSlots;
  hkArray<hkLocalFrameOnBone2_t<_ipointer>, _ipointer> localFrames;

  void noPartitions();
};

template <template <class C> class _ipointer>
using hkaSkeleton2011_t =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton2011_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaSkeleton2011_rp_t : hkaSkeleton2011_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaSkeleton2012_t_sharedData : hkReferenceObject<_ipointer> {
  _ipointer<char> Name;
  hkArray<int16, _ipointer> parentIndicies;
  hkArray<hkaBone_t<_ipointer>, _ipointer> bones;
  hkArray<hkQTransform, _ipointer> transforms;
  hkArray<float, _ipointer> referenceFloats;
  hkArray<_ipointer<char>, _ipointer> floatSlots;
  hkArray<hkLocalFrameOnBone2_t<_ipointer>, _ipointer> localFrames;
  hkArray<hkaPartition_t<_ipointer>, _ipointer> partitions;
};

template <template <class C> class _ipointer>
using hkaSkeleton2012_t =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton2012_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaSkeleton2012_rp_t : hkaSkeleton2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSkeleton2013_t : hkaSkeleton2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSkeleton2013_rp_t : hkaSkeleton2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSkeleton2014_t : hkaSkeleton2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSkeleton2014_rp_t : hkaSkeleton2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSkeleton2015_t : hkaSkeleton2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSkeleton2015_rp_t : hkaSkeleton2012_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaSkeleton2016_t_sharedData : hkReferenceObject2016<_ipointer> {
  _ipointer<char> Name;
  hkArray<int16, _ipointer> parentIndicies;
  hkArray<hkaBone_t<_ipointer>, _ipointer> bones;
  hkArray<hkQTransform, _ipointer> transforms;
  hkArray<float, _ipointer> referenceFloats;
  hkArray<_ipointer<char>, _ipointer> floatSlots;
  hkArray<hkLocalFrameOnBone2_t<_ipointer>, _ipointer> localFrames;
  hkArray<hkaPartition_t<_ipointer>, _ipointer> partitions;
};

template <template <class C> class _ipointer>
using hkaSkeleton2016_t =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton2016_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaSkeleton2016_rp_t : hkaSkeleton2016_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSkeleton2017_t
    : hkaSkeleton_t_shared<_ipointer, hkaSkeleton2016_t_sharedData> {};
template <template <class C> class _ipointer>
struct hkaSkeleton2017_rp_t : hkaSkeleton2016_t<_ipointer> {};