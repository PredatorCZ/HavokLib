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
#include "hkObjectBase.h"
#include "hkaAnimation.h"
#include "hkaDeltaDecompressor.h"

template <class C>
struct hkaDeltaCompressedAnimation_t
    : hkaDeltaCompressedAnimationInternalInterface,
      hkaSkeletalAnimation_t<typename C::parentClass, hkaAnimationLerpSampler> {
  typedef C value_type;
  typedef hkaSkeletalAnimation_t<typename C::parentClass,
                                 hkaAnimationLerpSampler>
      parent;
  hkClassConstructor(hkaDeltaCompressedAnimation_t<C>);
  void SwapEndian() override {
    parent::SwapEndian();
    static_cast<value_type *>(this->Data)->SwapEndian();
  }
  void Process() override {
    decomp.Assign(this);
    this->numFrames = GetNumOfPoses();
    this->frameRate = static_cast<uint32>(this->numFrames / this->Duration());
  }
  hkaDeltaDecompressor decomp;

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override {
    decomp.GetFrame(trackID, frame, out);
  };
  size_t GetNumOfPoses() const override {
    return static_cast<value_type *>(this->Data)->GetNumOfPoses();
  }
  size_t GetBlockSize() const override {
    return static_cast<value_type *>(this->Data)->GetBlockSize();
  }
  size_t GetQuantizedDataOffset() const override {
    return static_cast<value_type *>(this->Data)->GetQuantizedDataOffset();
  }
  size_t GetStaticMaskOffset() const override {
    return static_cast<value_type *>(this->Data)->GetStaticMaskOffset();
  }
  size_t GetStaticDataOffset() const override {
    return static_cast<value_type *>(this->Data)->GetStaticDataOffset();
  }
  const char *GetData() const override {
    return static_cast<value_type *>(this->Data)->GetData();
  }
  size_t GetNumDynamicTracks() const override {
    return static_cast<value_type *>(this->Data)->GetNumDynamicTracks();
  }
  size_t GetOffsetsOffset() const override {
    return static_cast<value_type *>(this->Data)->GetOffsetsOffset();
  }
  size_t GetBitWidthOffset() const override {
    return static_cast<value_type *>(this->Data)->GetBitWidthOffset();
  }
  size_t GetScalesOffset() const override {
    return static_cast<value_type *>(this->Data)->GetScalesOffset();
  }
  size_t GetNumPreserved() const override {
    return static_cast<value_type *>(this->Data)->GetNumPreserved();
  }
};

template <class C>
using hkaDeltaCompressedSkeletalAnimation_t = hkaDeltaCompressedAnimation_t<C>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaDeltaCompressedSkeletalAnimation_t_shared : _parent<_ipointer> {
  size_t GetNumOfPoses() const { return this->numberOfPoses; }
  size_t GetBlockSize() const { return this->blockSize; }
  size_t GetQuantizedDataOffset() const { return this->quantizedDataIdx; }
  size_t GetStaticMaskOffset() const { return this->staticMaskIdx; }
  size_t GetStaticDataOffset() const { return this->maskDOFsIdx; }
  const char *GetData() const {
    return reinterpret_cast<const char *>(
        static_cast<const uint8 *>(this->dataBuffer));
  }
  size_t GetNumDynamicTracks() const { return this->qFormat.numD; }
  size_t GetOffsetsOffset() const { return this->qFormat.offsetIdx; }
  size_t GetBitWidthOffset() const { return this->qFormat.bitWidthIdx; }
  size_t GetScalesOffset() const { return this->qFormat.scaleIdx; }
  size_t GetNumPreserved() const { return this->qFormat.preserved; }

  // ADD_DISABLERS(_parent<_ipointer>, version1);

  void SwapEndian() {
    FByteswapper(this->numberOfPoses);
    FByteswapper(this->blockSize);
    FByteswapper(this->quantizedDataIdx);
    FByteswapper(this->quantizedDataSize);
    FByteswapper(this->staticMaskIdx);
    FByteswapper(this->staticMaskSize);
    FByteswapper(this->maskDOFsIdx);
    FByteswapper(this->maskDOFsSize);
    FByteswapper(this->totalBlockSize);
    FByteswapper(this->lastBlockSize);
    FByteswapper(this->qFormat);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaDeltaCompressedAnimation550_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  uint32 numberOfPoses;
  uint32 blockSize;
  QuantizationFormat qFormat;
  uint32 quantizedDataIdx;
  uint32 quantizedDataSize;
  uint32 staticMaskIdx;
  uint32 staticMaskSize;
  uint32 maskDOFsIdx;
  uint32 maskDOFsSize;
  uint32 totalBlockSize;
  uint32 lastBlockSize;
  _ipointer<uint8> dataBuffer;
  uint32 numDataBuffer;

  void version1();
};

template <template <class C> class _ipointer>
using hkaDeltaCompressedSkeletalAnimation550_t_sharedData =
    hkaDeltaCompressedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_t>;
template <template <class C> class _ipointer>
using hkaDeltaCompressedSkeletalAnimation550_rp_t_sharedData =
    hkaDeltaCompressedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_rp_t>;

template <template <class C> class _ipointer>
using hkaDeltaCompressedSkeletalAnimation550_t =
    hkaDeltaCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaDeltaCompressedSkeletalAnimation550_t_sharedData>;
template <template <class C> class _ipointer>
using hkaDeltaCompressedSkeletalAnimation550_rp_t =
    hkaDeltaCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaDeltaCompressedSkeletalAnimation550_rp_t_sharedData>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaDeltaCompressedAnimation660_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  uint32 numberOfPoses;
  uint32 blockSize;
  QuantizationFormat qFormat;
  uint32 quantizedDataIdx;
  uint32 quantizedDataSize;
  uint32 staticMaskIdx;
  uint32 staticMaskSize;
  uint32 maskDOFsIdx;
  uint32 maskDOFsSize;
  uint32 numStaticTransformDOFs;
  uint32 numDynamicTransformDOFs;
  uint32 totalBlockSize;
  uint32 lastBlockSize;
  _ipointer<uint8> dataBuffer;
  uint32 numDataBuffer;
};

template <template <class C> class _ipointer>
using hkaDeltaCompressedAnimation660_t_sharedData =
    hkaDeltaCompressedAnimation660_tt<_ipointer, hkaAnimation660_t>;
template <template <class C> class _ipointer>
using hkaDeltaCompressedAnimation660_rp_t_sharedData =
    hkaDeltaCompressedAnimation660_tt<_ipointer, hkaAnimation660_rp_t>;

template <template <class C> class _ipointer>
using hkaDeltaCompressedAnimation660_t =
    hkaDeltaCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaDeltaCompressedAnimation660_t_sharedData>;
template <template <class C> class _ipointer>
using hkaDeltaCompressedAnimation660_rp_t =
    hkaDeltaCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaDeltaCompressedAnimation660_rp_t_sharedData>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaDeltaCompressedAnimation710_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  uint32 numberOfPoses;
  uint32 blockSize;
  QuantizationFormat qFormat;
  uint32 quantizedDataIdx;
  uint32 quantizedDataSize;
  uint32 staticMaskIdx;
  uint32 staticMaskSize;
  uint32 maskDOFsIdx;
  uint32 maskDOFsSize;
  uint32 numStaticTransformDOFs;
  uint32 numDynamicTransformDOFs;
  uint32 totalBlockSize;
  uint32 lastBlockSize;
  hkArray<uint8, _ipointer> dataBuffer;
};

template <template <class C> class _ipointer>
using hkaDeltaCompressedAnimation710_t_sharedData =
    hkaDeltaCompressedAnimation710_tt<_ipointer, hkaAnimation710_t>;
template <template <class C> class _ipointer>
using hkaDeltaCompressedAnimation710_rp_t_sharedData =
    hkaDeltaCompressedAnimation710_tt<_ipointer, hkaAnimation710_rp_t>;

template <template <class C> class _ipointer>
using hkaDeltaCompressedAnimation710_t =
    hkaDeltaCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaDeltaCompressedAnimation710_t_sharedData>;
template <template <class C> class _ipointer>
using hkaDeltaCompressedAnimation710_rp_t =
    hkaDeltaCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaDeltaCompressedAnimation710_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaDeltaCompressedAnimation2010_t
    : hkaDeltaCompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaDeltaCompressedAnimation2010_rp_t
    : hkaDeltaCompressedAnimation710_rp_t<_ipointer> {};