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
#include "hkaSplineDecompressor.h"

template <class C>
struct hkaSplineCompressedAnimation_t
    : hkaSplineCompressedAnimationInternalInterface,
      hkaSkeletalAnimation_t<typename C::parentClass,
                             hkaAnimationInternalInterface> {
  typedef C value_type;
  typedef hkaSkeletalAnimation_t<typename C::parentClass,
                                 hkaAnimationInternalInterface>
      parent;
  hkClassConstructor(hkaSplineCompressedAnimation_t);
  void SwapEndian() {
    parent::SwapEndian();
    static_cast<value_type *>(this->Data)->SwapEndian();
  }
  void Process() {
    decomp.Assign(this);
    this->frameRate =
        static_cast<uint32>(this->GetNumFrames() / this->Duration());
  }
  hkaSplineDecompressor decomp;

  uint32 GetNumFrames() const {
    return static_cast<value_type *>(this->Data)->GetNumFrames();
  }
  uint32 GetNumBlocks() const {
    return static_cast<value_type *>(this->Data)->GetNumBlocks();
  }
  uint32 GetMaxFramesPerBlock() const {
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
  hkRealArray<uint32> GetBlockOffsets() const {
    return static_cast<value_type *>(this->Data)->GetBlockOffsets();
  }
  hkRealArray<uint32> GetFloatBlockOffsets() const {
    return static_cast<value_type *>(this->Data)->GetFloatBlockOffsets();
  }
  hkRealArray<uint32> GetTransformOffsets() const {
    return static_cast<value_type *>(this->Data)->GetTransformOffsets();
  }
  hkRealArray<uint32> GetFloatOffsets() const {
    return static_cast<value_type *>(this->Data)->GetFloatOffsets();
  }

  void GetValue(uni::RTSValue &output, float time,
                size_t trackID) const override {
    size_t blockID = static_cast<size_t>(time * GetBlockInverseDuration());
    float localTime = time - (static_cast<float>(blockID) * GetBlockDuration());

    decomp.blocks[blockID].GetValue(trackID, localTime, output);
  }
};

template <class C>
using hkaSplineSkeletalAnimation_t = hkaSplineCompressedAnimation_t<C>;

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaSplineCompressedSkeletalAnimation_t_shared : _parent<_ipointer> {
  uint32 GetNumFrames() const { return this->numFrames; }
  uint32 GetNumBlocks() const { return this->numBlocks; }
  uint32 GetMaxFramesPerBlock() const { return this->maxFramesPerBlock; }
  float GetBlockDuration() const { return this->blockDuration; }
  float GetBlockInverseDuration() const { return this->blockInverseDuration; }
  float GetFrameDuration() const { return this->frameDuration; }
  char *GetData() { return this->data; }
  hkRealArray<uint32> GetBlockOffsets() const {
    return {this->blockOffsets, this->blockOffsets.count};
  }
  hkRealArray<uint32> GetFloatBlockOffsets() const {
    return {this->floatBlockOffsets, this->floatBlockOffsets.count};
  }
  hkRealArray<uint32> GetTransformOffsets() const {
    return {this->transformOffsets, this->transformOffsets.count};
  }
  hkRealArray<uint32> GetFloatOffsets() const {
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

    for (uint32 i = 0; i < this->blockOffsets.count; i++)
      FByteswapper(this->blockOffsets[i]);

    for (uint32 i = 0; i < this->floatBlockOffsets.count; i++)
      FByteswapper(this->floatBlockOffsets[i]);

    for (uint32 i = 0; i < this->transformOffsets.count; i++)
      FByteswapper(this->transformOffsets[i]);

    for (uint32 i = 0; i < this->floatOffsets.count; i++)
      FByteswapper(this->floatOffsets[i]);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaSplineCompressedAnimation550_tt : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  uint32 numFrames;
  uint32 numBlocks;
  uint32 maxFramesPerBlock;
  uint32 maskAndQuantizationSize;
  float blockDuration;
  float blockInverseDuration;
  float frameDuration;
  hkArray<uint32, _ipointer> blockOffsets, floatBlockOffsets, transformOffsets,
      floatOffsets;
  hkArray<char, _ipointer> data;
  uint32 endian;
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