/*  Havok Format Library
    Copyright(C) 2016-2019 Lukas Cone

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
      hkaSkeletalAnimation_t<typename C::parentClass> {
  typedef C value_type;
  hkClassConstructor(hkaWaveletCompressedAnimation_t<C>);
  void SwapEndian() {
    hkaSkeletalAnimation_t<typename C::parentClass>::SwapEndian();
    static_cast<value_type *>(this->Data)->SwapEndian();
  }
  void Process() {} //{ decomp.Assign(this); }
  // hkaWaveletDecompressor decomp;
  const int GetNumOfPoses() const {
    return static_cast<value_type *>(this->Data)->GetNumOfPoses();
  }
  const int GetBlockSize() const {
    return static_cast<value_type *>(this->Data)->GetBlockSize();
  }
  const int GetQuantizedDataOffset() const {
    return static_cast<value_type *>(this->Data)->GetQuantizedDataOffset();
  }
  const int GetStaticMaskOffset() const {
    return static_cast<value_type *>(this->Data)->GetStaticMaskOffset();
  }
  const int GetStaticDataOffset() const {
    return static_cast<value_type *>(this->Data)->GetStaticDataOffset();
  }
  const char *GetData() const {
    return static_cast<value_type *>(this->Data)->GetData();
  }
  const int GetNumDynamicTracks() const {
    return static_cast<value_type *>(this->Data)->GetNumDynamicTracks();
  }
  const int GetOffsetsOffset() const {
    return static_cast<value_type *>(this->Data)->GetOffsetsOffset();
  }
  const int GetBitWidthOffset() const {
    return static_cast<value_type *>(this->Data)->GetBitWidthOffset();
  }
  const int GetScalesOffset() const {
    return static_cast<value_type *>(this->Data)->GetScalesOffset();
  }
  const int GetNumPreserved() const {
    return static_cast<value_type *>(this->Data)->GetNumPreserved();
  }
  bool IsTrackStatic(int trackID, TrackType type) const { return false; }
  void GetTrack(int trackID, int frame, float delta, TrackType type,
                Vector4A16 &out) const {}
  void GetTransform(int trackID, int frame, float delta,
                    hkQTransform &out) const {}
  int GetNumInternalFrames() const { return 0; }
};

template <class C>
using hkaWaveletCompressedSkeletalAnimation_t =
    hkaWaveletCompressedAnimation_t<C>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaWaveletCompressedSkeletalAnimation_t_shared : _parent<_ipointer> {
  ES_FORCEINLINE const int GetNumOfPoses() const { return this->numberOfPoses; }
  ES_FORCEINLINE const int GetBlockSize() const { return this->blockSize; }
  ES_FORCEINLINE const int GetQuantizedDataOffset() const {
    return this->quantizedDataIdx;
  }
  ES_FORCEINLINE const int GetStaticMaskOffset() const {
    return this->staticMaskIdx;
  }
  ES_FORCEINLINE const int GetStaticDataOffset() const {
    return this->staticDOFsIdx;
  }
  ES_FORCEINLINE const char *GetData() const {
    return reinterpret_cast<const char *>(static_cast<const uchar*>(
        this->dataBuffer.GetData()));
  }
  ES_FORCEINLINE const int GetNumDynamicTracks() const {
    return this->qFormat.numD;
  }
  ES_FORCEINLINE const int GetOffsetsOffset() const {
    return this->qFormat.offsetIdx;
  }
  ES_FORCEINLINE const int GetBitWidthOffset() const {
    return this->qFormat.bitWidthIdx;
  }
  ES_FORCEINLINE const int GetScalesOffset() const {
    return this->qFormat.scaleIdx;
  }
  ES_FORCEINLINE const int GetNumPreserved() const {
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
  int numberOfPoses;
  int blockSize;
  QuantizationFormat qFormat;
  int staticMaskIdx;
  int staticDOFsIdx;
  int blockIndexIdx;
  int blockIndexSize;
  int quantizedDataIdx;
  int quantizedDataSize;
  _ipointer<unsigned char> dataBuffer;
  int numDataBuffer;
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
  int numberOfPoses;
  int blockSize;
  QuantizationFormat qFormat;
  int staticMaskIdx;
  int staticDOFsIdx;
  int numStaticTransformDOFs;
  int numDynamicTransformDOFs;
  int blockIndexIdx;
  int blockIndexSize;
  int quantizedDataIdx;
  int quantizedDataSize;
  _ipointer<unsigned char> dataBuffer;
  int numDataBuffer;
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
  int numberOfPoses;
  int blockSize;
  QuantizationFormat qFormat;
  int staticMaskIdx;
  int staticDOFsIdx;
  int numStaticTransformDOFs;
  int numDynamicTransformDOFs;
  int blockIndexIdx;
  int blockIndexSize;
  int quantizedDataIdx;
  int quantizedDataSize;
  hkArray<unsigned char, _ipointer> dataBuffer;
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