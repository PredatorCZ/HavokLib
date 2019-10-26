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
#include "hkaAnimation.h"
#include "hkaSplineDecompressor.h"

template <class C>
struct hkaSplineCompressedAnimation_t
    : hkaSplineCompressedAnimationInternalInterface,
      hkaSkeletalAnimation_t<typename C::parentClass> {
  typedef C value_type;
  typedef hkaSkeletalAnimation_t<typename C::parentClass> parent;
  hkClassConstructor(hkaSplineCompressedAnimation_t);
  void SwapEndian() {
    hkaSkeletalAnimation_t<typename C::parentClass>::SwapEndian();
    static_cast<value_type *>(this->Data)->SwapEndian();
  }
  void Process() { decomp.Assign(this); }
  hkaSplineDecompressor decomp;

  int GetNumFrames() const {
    return static_cast<value_type *>(this->Data)->GetNumFrames();
  }
  int GetNumBlocks() const {
    return static_cast<value_type *>(this->Data)->GetNumBlocks();
  }
  int GetMaxFramesPerBlock() const {
    return static_cast<value_type *>(this->Data)->GetMaxFramesPerBlock();
  }
  float GetBlockDuration() const {
    return static_cast<value_type *>(this->Data)->GetBlockDuration();
  }
  float GetBlockInverseDuration() const {
    return static_cast<value_type *>(this->Data)->GetBlockInverseDuration();
  }
  float GetFrameDuration() const {
    return static_cast<value_type *>(this->Data)->GetFrameDuration();
  }
  char *GetData() const {
    return static_cast<value_type *>(this->Data)->GetData();
  }
  hkRealArray<uint> GetBlockOffsets() const {
    return static_cast<value_type *>(this->Data)->GetBlockOffsets();
  }
  hkRealArray<uint> GetFloatBlockOffsets() const {
    return static_cast<value_type *>(this->Data)
        ->GetFloatBlockOffsets();
  }
  hkRealArray<uint> GetTransformOffsets() const {
    return static_cast<value_type *>(this->Data)
        ->GetTransformOffsets();
  }
  hkRealArray<uint> GetFloatOffsets() const {
    return static_cast<value_type *>(this->Data)->GetFloatOffsets();
  }
  bool IsDecoderSupported() const { return true; }
  ES_INLINE int GetLocalValues(int frame, float delta,
                               float &localFrame) const {
    int blockID = frame / (GetMaxFramesPerBlock() - 1);

    if (blockID >= GetNumBlocks())
      blockID = GetNumBlocks() - 1;
    else if (blockID < 0)
      blockID = 0;

    localFrame =
        static_cast<float>(frame - (blockID * (GetMaxFramesPerBlock() - 1)));
    localFrame = (delta + localFrame) * GetFrameDuration();
    localFrame =
        localFrame * GetBlockInverseDuration() * (GetMaxFramesPerBlock() - 1);

    return blockID;
  }

  void GetTrack(int trackID, int frame, float delta, TrackType type,
                Vector4A16 &out) const {
    float localFrame;
    int blockID = GetLocalValues(frame, delta, localFrame);

    decomp.blocks[blockID].GetTrack(trackID, localFrame, type, out);
  }
  void GetTransform(int trackID, int frame, float delta,
                    hkQTransform &out) const {
    float localFrame;
    int blockID = GetLocalValues(frame, delta, localFrame);

    decomp.blocks[blockID].GetTransform(trackID, localFrame, out);
  }
  bool IsTrackStatic(int trackID, TrackType type) const {
    return decomp.IsTrackStatic(trackID, type);
  }
  int GetNumInternalFrames() const { return GetNumFrames(); }
};

template <class C>
using hkaSplineSkeletalAnimation_t = hkaSplineCompressedAnimation_t<C>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaSplineCompressedSkeletalAnimation_t_shared : _parent<_ipointer> {
  ES_FORCEINLINE int GetNumFrames() const { return this->numFrames; }
  ES_FORCEINLINE int GetNumBlocks() const { return this->numBlocks; }
  ES_FORCEINLINE int GetMaxFramesPerBlock() const {
    return this->maxFramesPerBlock;
  }
  ES_FORCEINLINE float GetBlockDuration() const { return this->blockDuration; }
  ES_FORCEINLINE float GetBlockInverseDuration() const {
    return this->blockInverseDuration;
  }
  ES_FORCEINLINE float GetFrameDuration() const { return this->frameDuration; }
  ES_FORCEINLINE char *GetData() {
    return this->data;
  }
  ES_FORCEINLINE hkRealArray<uint> GetBlockOffsets() const {
    return {this->blockOffsets, this->blockOffsets.count};
  }
  ES_FORCEINLINE hkRealArray<uint>
  GetFloatBlockOffsets() const {
    return {this->floatBlockOffsets,
            this->floatBlockOffsets.count};
  }
  ES_FORCEINLINE hkRealArray<uint>
  GetTransformOffsets() const {
    return {this->transformOffsets,
            this->transformOffsets.count};
  }
  ES_FORCEINLINE hkRealArray<uint> GetFloatOffsets() const {
    return {this->floatOffsets, this->floatOffsets.count};
  }

  void SwapEndian() {
    FByteswapper(this->numFrames);
    FByteswapper(this->numBlocks);
    FByteswapper(this->maxFramesPerBlock);
    FByteswapper(this->maskAndQuantizationSize);
    FByteswapper(this->blockDuration);
    FByteswapper(this->blockInverseDuration);
    FByteswapper(this->frameDuration);
    FByteswapper(this->endian);
    FByteswapper(this->blockOffsets.count);
    FByteswapper(this->floatBlockOffsets.count);
    FByteswapper(this->transformOffsets.count);
    FByteswapper(this->floatOffsets.count);

    for (int i = 0; i < this->blockOffsets.count; i++)
      FByteswapper(this->blockOffsets[i]);

    for (int i = 0; i < this->floatBlockOffsets.count; i++)
      FByteswapper(this->floatBlockOffsets[i]);

    for (int i = 0; i < this->transformOffsets.count; i++)
      FByteswapper(this->transformOffsets[i]);

    for (int i = 0; i < this->floatOffsets.count; i++)
      FByteswapper(this->floatOffsets[i]);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaSplineCompressedAnimation550_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  int numFrames;
  int numBlocks;
  int maxFramesPerBlock;
  int maskAndQuantizationSize;
  float blockDuration;
  float blockInverseDuration;
  float frameDuration;
  hkArray<uint, _ipointer> blockOffsets, floatBlockOffsets, transformOffsets,
      floatOffsets;
  hkArray<char, _ipointer> data;
  int endian;
};

template <template <class C> class _ipointer>
using hkaSplineCompressedSkeletalAnimation550_t_sharedData =
    hkaSplineCompressedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_t>;
template <template <class C> class _ipointer>
using hkaSplineCompressedSkeletalAnimation550_rp_t_sharedData =
    hkaSplineCompressedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_rp_t>;

template <template <class C> class _ipointer>
using hkaSplineSkeletalAnimation550_t =
    hkaSplineCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaSplineCompressedSkeletalAnimation550_t_sharedData>;
template <template <class C> class _ipointer>
using hkaSplineSkeletalAnimation550_rp_t =
    hkaSplineCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaSplineCompressedSkeletalAnimation550_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation660_t
    : hkaSplineSkeletalAnimation550_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation660_rp_t
    : hkaSplineSkeletalAnimation550_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
using hkaSplineCompressedAnimation710_t_sharedData =
    hkaSplineCompressedAnimation550_tt<_ipointer, hkaAnimation710_t>;
template <template <class C> class _ipointer>
using hkaSplineCompressedAnimation710_rp_t_sharedData =
    hkaSplineCompressedAnimation550_tt<_ipointer, hkaAnimation710_rp_t>;

template <template <class C> class _ipointer>
using hkaSplineCompressedAnimation710_t =
    hkaSplineCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaSplineCompressedAnimation710_t_sharedData>;
template <template <class C> class _ipointer>
using hkaSplineCompressedAnimation710_rp_t =
    hkaSplineCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaSplineCompressedAnimation710_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2010_t
    : hkaSplineCompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2010_rp_t
    : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2011_t
    : hkaSplineCompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2011_rp_t
    : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2012_t
    : hkaSplineCompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2012_rp_t
    : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2013_t
    : hkaSplineCompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2013_rp_t
    : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2014_t
    : hkaSplineCompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2014_rp_t
    : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2015_t
    : hkaSplineCompressedAnimation710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2015_rp_t
    : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
using hkaSplineCompressedAnimation2016_t_sharedData =
    hkaSplineCompressedAnimation550_tt<_ipointer, hkaAnimation2016_t>;
template <template <class C> class _ipointer>
using hkaSplineCompressedAnimation2016_rp_t_sharedData =
    hkaSplineCompressedAnimation550_tt<_ipointer, hkaAnimation2016_rp_t>;

template <template <class C> class _ipointer>
using hkaSplineCompressedAnimation2016_t =
    hkaSplineCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaSplineCompressedAnimation2016_t_sharedData>;
template <template <class C> class _ipointer>
using hkaSplineCompressedAnimation2016_rp_t =
    hkaSplineCompressedSkeletalAnimation_t_shared<
        _ipointer, hkaSplineCompressedAnimation2016_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2017_t
    : hkaSplineCompressedAnimation2016_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaSplineCompressedAnimation2017_rp_t
    : hkaSplineCompressedAnimation2016_rp_t<_ipointer> {};