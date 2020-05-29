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
//#include "hkaWaveletDecompressor.h"

template <class C>
struct hkaWaveletCompressedAnimation_t
    : hkaWaveletCompressedAnimationInternalInterface,
      hkaSkeletalAnimation_t<typename C::parentClass, hkaAnimationLerpSampler> {
  typedef C value_type;
  typedef hkaSkeletalAnimation_t<typename C::parentClass,
                                 hkaAnimationLerpSampler>
      parent;
  hkClassConstructor(hkaWaveletCompressedAnimation_t);
  void SwapEndian() override {
    parent::SwapEndian();
    static_cast<value_type *>(this->Data)->SwapEndian();
  }
  void Process() override {} //{ decomp.Assign(this); }
  // hkaWaveletDecompressor decomp;
  size_t GetNumOfPoses() const {
    return static_cast<value_type *>(this->Data)->GetNumOfPoses();
  }
  size_t GetBlockSize() const {
    return static_cast<value_type *>(this->Data)->GetBlockSize();
  }
  size_t GetQuantizedDataOffset() const {
    return static_cast<value_type *>(this->Data)->GetQuantizedDataOffset();
  }
  size_t GetStaticMaskOffset() const {
    return static_cast<value_type *>(this->Data)->GetStaticMaskOffset();
  }
  size_t GetStaticDataOffset() const {
    return static_cast<value_type *>(this->Data)->GetStaticDataOffset();
  }
  const char *GetData() const {
    return static_cast<value_type *>(this->Data)->GetData();
  }
  size_t GetNumDynamicTracks() const {
    return static_cast<value_type *>(this->Data)->GetNumDynamicTracks();
  }
  size_t GetOffsetsOffset() const {
    return static_cast<value_type *>(this->Data)->GetOffsetsOffset();
  }
  size_t GetBitWidthOffset() const {
    return static_cast<value_type *>(this->Data)->GetBitWidthOffset();
  }
  size_t GetScalesOffset() const {
    return static_cast<value_type *>(this->Data)->GetScalesOffset();
  }
  size_t GetNumPreserved() const {
    return static_cast<value_type *>(this->Data)->GetNumPreserved();
  }

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override {
    //decomp.GetFrame(trackID, frame, out);
  };
};

template <class C>
using hkaWaveletCompressedSkeletalAnimation_t =
    hkaWaveletCompressedAnimation_t<C>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaWaveletCompressedSkeletalAnimation_t_shared : _parent<_ipointer> {
   uint32 GetNumOfPoses() const { return this->numberOfPoses; }
   uint32 GetBlockSize() const { return this->blockSize; }
   uint32 GetQuantizedDataOffset() const {
    return this->quantizedDataIdx;
  }
   uint32 GetStaticMaskOffset() const {
    return this->staticMaskIdx;
  }
   uint32 GetStaticDataOffset() const {
    return this->staticDOFsIdx;
  }
  const char *GetData() const {
    return reinterpret_cast<const char *>(static_cast<const uint8*>(
        this->dataBuffer.GetData()));
  }
   uint32 GetNumDynamicTracks() const {
    return this->qFormat.numD;
  }
   uint32 GetOffsetsOffset() const {
    return this->qFormat.offsetIdx;
  }
   uint32 GetBitWidthOffset() const {
    return this->qFormat.bitWidthIdx;
  }
   uint32 GetScalesOffset() const {
    return this->qFormat.scaleIdx;
  }
   uint32 GetNumPreserved() const {
    return this->qFormat.preserved;
  }

  void SwapEndian() {
    FByteswapper(this->numberOfPoses);
    FByteswapper(this->blockSize);
    FByteswapper(this->staticMaskIdx);
    FByteswapper(this->staticDOFsIdx);
    FByteswapper(this->blockIndexIdx);
    FByteswapper(this->blockIndexSize);
    FByteswapper(this->quantizedDataIdx);
    FByteswapper(this->quantizedDataSize);
    FByteswapper(this->qFormat);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaWaveletCompressedAnimation550_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  uint32 numberOfPoses;
  uint32 blockSize;
  QuantizationFormat qFormat;
  uint32 staticMaskIdx;
  uint32 staticDOFsIdx;
  uint32 blockIndexIdx;
  uint32 blockIndexSize;
  uint32 quantizedDataIdx;
  uint32 quantizedDataSize;
  _ipointer<uint8> dataBuffer;
  uint32 numDataBuffer;
};

template <template <class C> class _ipointer>
using hkaWaveletCompressedSkeletalAnimation550_t_sharedData =
    hkaWaveletCompressedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_t>;
template <template <class C> class _ipointer>
using hkaWaveletCompressedSkeletalAnimation550_rp_t_sharedData =
    hkaWaveletCompressedAnimation550_tt<_ipointer,
                                        hkaSkeletalAnimation550_rp_t>;

template <template <class C> class _ipointer>
using hkaWaveletCompressedSkeletalAnimation550_t =
    hkaWaveletCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaWaveletCompressedSkeletalAnimation550_t_sharedData>;
template <template <class C> class _ipointer>
using hkaWaveletCompressedSkeletalAnimation550_rp_t =
    hkaWaveletCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaWaveletCompressedSkeletalAnimation550_rp_t_sharedData>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaWaveletCompressedAnimation660_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  uint32 numberOfPoses;
  uint32 blockSize;
  QuantizationFormat qFormat;
  uint32 staticMaskIdx;
  uint32 staticDOFsIdx;
  uint32 numStaticTransformDOFs;
  uint32 numDynamicTransformDOFs;
  uint32 blockIndexIdx;
  uint32 blockIndexSize;
  uint32 quantizedDataIdx;
  uint32 quantizedDataSize;
  _ipointer<uint8> dataBuffer;
  uint32 numDataBuffer;
};

template <template <class C> class _ipointer>
using hkaWaveletCompressedAnimation660_t_sharedData =
    hkaWaveletCompressedAnimation660_tt<_ipointer, hkaAnimation660_t>;
template <template <class C> class _ipointer>
using hkaWaveletCompressedAnimation660_rp_t_sharedData =
    hkaWaveletCompressedAnimation660_tt<_ipointer, hkaAnimation660_rp_t>;

template <template <class C> class _ipointer>
using hkaWaveletCompressedAnimation660_t =
    hkaWaveletCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaWaveletCompressedAnimation660_t_sharedData>;
template <template <class C> class _ipointer>
using hkaWaveletCompressedAnimation660_rp_t =
    hkaWaveletCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaWaveletCompressedAnimation660_rp_t_sharedData>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaWaveletCompressedAnimation710_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  uint32 numberOfPoses;
  uint32 blockSize;
  QuantizationFormat qFormat;
  uint32 staticMaskIdx;
  uint32 staticDOFsIdx;
  uint32 numStaticTransformDOFs;
  uint32 numDynamicTransformDOFs;
  uint32 blockIndexIdx;
  uint32 blockIndexSize;
  uint32 quantizedDataIdx;
  uint32 quantizedDataSize;
  hkArray<uint8, _ipointer> dataBuffer;
};

template <template <class C> class _ipointer>
using hkaWaveletCompressedAnimation710_t_sharedData =
    hkaWaveletCompressedAnimation710_tt<_ipointer, hkaAnimation710_t>;
template <template <class C> class _ipointer>
using hkaWaveletCompressedAnimation710_rp_t_sharedData =
    hkaWaveletCompressedAnimation710_tt<_ipointer, hkaAnimation710_rp_t>;

template <template <class C> class _ipointer>
using hkaWaveletCompressedAnimation710_t =
    hkaWaveletCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaWaveletCompressedAnimation710_t_sharedData>;
template <template <class C> class _ipointer>
using hkaWaveletCompressedAnimation710_rp_t =
    hkaWaveletCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaWaveletCompressedAnimation710_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaWaveletCompressedAnimation2010_t
    : hkaWaveletCompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaWaveletCompressedAnimation2010_rp_t
    : hkaWaveletCompressedAnimation710_rp_t<_ipointer> {};