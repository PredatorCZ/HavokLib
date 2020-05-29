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
#include "hkaAnimation.h"

template <class C>
struct hkaInterleavedUncompressedAnimation_t
    : hkaInterleavedAnimationInternalInterface,
      hkaSkeletalAnimation_t<typename C::parentClass, hkaAnimationLerpSampler> {
  typedef C value_type;
  typedef hkaSkeletalAnimation_t<typename C::parentClass,
                                 hkaAnimationLerpSampler>
      parent;
  hkClassConstructor(hkaInterleavedUncompressedAnimation_t);
  void SwapEndian() override {
    parent::SwapEndian();
    static_cast<value_type *>(this->Data)->SwapEndian();
  }

  void Process() override {
    this->numFrames = static_cast<uint32>(this->GetNumTransforms() /
                                          this->GetNumOfTransformTracks());
    this->frameRate = static_cast<uint32>(this->numFrames / this->Duration());
  }

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override {
    out = *GetTransform(frame * GetNumOfTransformTracks() + trackID);
  }

  size_t GetNumTransforms() const override {
    return static_cast<value_type *>(this->Data)->NumTransforms();
  }
  size_t GetNumFloats() const override {
    return static_cast<value_type *>(this->Data)->NumFloats();
  }

  const hkQTransform *GetTransform(size_t id) const override {
    return static_cast<value_type *>(this->Data)->GetTransform(id);
  }
  float GetFloat(size_t id) const override {
    return static_cast<value_type *>(this->Data)->GetFloat(id);
  }
};

template <class C>
using hkaInterleavedSkeletalAnimation_t =
    hkaInterleavedUncompressedAnimation_t<C>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaInterleavedAnimation_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;

  enablePtrPair(transforms) NumTransforms() const {
    return this->numTransforms;
  }
  enablehkArray(transforms) NumTransforms() const {
    return this->transforms.count;
  }
  enablePtrPairRef(transforms) NumTransforms() { return this->numTransforms; }
  enablehkArrayRef(transforms) NumTransforms() {
    return this->transforms.count;
  }

  enablePtrPair(floats) NumFloats() const { return this->numFloats; }
  enablehkArray(floats) NumFloats() const { return this->floats.count; }
  enablePtrPairRef(floats) NumFloats() { return this->numFloats; }
  enablehkArrayRef(floats) NumFloats() { return this->floats.count; }

  const hkQTransform *GetTransform(size_t id) const {
    return &this->transforms[id];
  }
  float GetFloat(size_t id) const { return this->floats[id]; }

  void SwapEndian() {
    FByteswapper(NumTransforms());
    FByteswapper(NumFloats());
    const size_t numTransforms = NumTransforms() * 12;
    const size_t numFloats = NumFloats();

    float *data = reinterpret_cast<float *>(
        static_cast<hkQTransform *>(this->transforms));
    float *dataEnd = data + numTransforms;

    for (; data != dataEnd; data++)
      FByteswapper(*data);

    for (size_t i = 0; i < numFloats; i++)
      FByteswapper(this->floats[i]);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaInterleavedAnimation550_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  _ipointer<hkQTransform> transforms;
  uint32 numTransforms;
  _ipointer<float> floats;
  uint32 numFloats;
};

template <template <class C> class _ipointer>
using hkaInterleavedAnimation550_t_sharedData =
    hkaInterleavedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_t>;
template <template <class C> class _ipointer>
using hkaInterleavedAnimation550_rp_t_sharedData =
    hkaInterleavedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_rp_t>;

template <template <class C> class _ipointer>
using hkaInterleavedSkeletalAnimation550_t =
    hkaInterleavedAnimation_t_shared<_ipointer,
                                     hkaInterleavedAnimation550_t_sharedData>;
template <template <class C> class _ipointer>
using hkaInterleavedSkeletalAnimation550_rp_t =
    hkaInterleavedAnimation_t_shared<
        _ipointer, hkaInterleavedAnimation550_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation660_t
    : hkaInterleavedSkeletalAnimation550_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation660_rp_t
    : hkaInterleavedSkeletalAnimation550_rp_t<_ipointer> {};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaInterleavedAnimation710_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  hkArray<hkQTransform, _ipointer> transforms;
  hkArray<float, _ipointer> floats;
};

template <template <class C> class _ipointer>
using hkaInterleavedAnimation710_t_sharedData =
    hkaInterleavedAnimation710_tt<_ipointer, hkaAnimation710_t>;
template <template <class C> class _ipointer>
using hkaInterleavedAnimation710_rp_t_sharedData =
    hkaInterleavedAnimation710_tt<_ipointer, hkaAnimation710_rp_t>;

template <template <class C> class _ipointer>
using hkaInterleavedUncompressedAnimation710_t =
    hkaInterleavedAnimation_t_shared<_ipointer,
                                     hkaInterleavedAnimation710_t_sharedData>;
template <template <class C> class _ipointer>
using hkaInterleavedUncompressedAnimation710_rp_t =
    hkaInterleavedAnimation_t_shared<
        _ipointer, hkaInterleavedAnimation710_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2010_t
    : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2010_rp_t
    : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2011_t
    : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2011_rp_t
    : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2012_t
    : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2012_rp_t
    : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2013_t
    : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2013_rp_t
    : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2014_t
    : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2014_rp_t
    : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2015_t
    : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaInterleavedUncompressedAnimation2015_rp_t
    : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};