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

#include "hka_animation.hpp"
#include "hka_spline_decompressor.hpp"

namespace {

template <class C>
struct hkaSplineCompressedAnimation_t
    : hkaSplineCompressedAnimationInternalInterface,
      hkaAnimation_t<typename C::parentClass, hkaAnimationInternalInterface> {
  typedef C value_type;
  typedef hkaAnimation_t<typename C::parentClass, hkaAnimationInternalInterface>
      parent;

  value_type *Data() { return static_cast<value_type *>(this->data.get()); }
  const value_type *Data() const {
    return static_cast<const value_type *>(this->data.get());
  }

  void SwapEndian() override {
    parent::SwapEndian();
    Data()->SwapEndian();
  }
  void Process() override {
    decomp.Assign(this);
    this->frameRate =
        static_cast<uint32>(this->GetNumFrames() / this->Duration());
  }
  hkaSplineDecompressor decomp;

  uint32 GetNumFrames() const override { return Data()->GetNumFrames(); }
  uint32 GetNumBlocks() const override { return Data()->GetNumBlocks(); }
  uint32 GetMaxFramesPerBlock() const override {
    return Data()->GetMaxFramesPerBlock();
  }
  float GetBlockDuration() const override { return Data()->GetBlockDuration(); }
  float GetBlockInverseDuration() const override {
    return Data()->GetBlockInverseDuration();
  }
  float GetFrameDuration() const override { return Data()->GetFrameDuration(); }
  char *GetData() const override {
    return const_cast<char *>(Data()->GetData());
  }
  hkRealArray<uint32> GetBlockOffsets() const override {
    return Data()->GetBlockOffsets();
  }
  hkRealArray<uint32> GetFloatBlockOffsets() const override {
    return Data()->GetFloatBlockOffsets();
  }
  hkRealArray<uint32> GetTransformOffsets() const override {
    return Data()->GetTransformOffsets();
  }
  hkRealArray<uint32> GetFloatOffsets() const override {
    return Data()->GetFloatOffsets();
  }

  void GetValue(uni::RTSValue &output, float time,
                size_t trackID) const override {
    size_t blockID = static_cast<size_t>(time * GetBlockInverseDuration());
    float localTime = time - (static_cast<float>(blockID) * GetBlockDuration());

    if (localTime < 0.f) {
      localTime = 0.f;
    }

    decomp.blocks[blockID].GetValue(trackID, localTime * frameRate, output);
  }
};

template <template <class C> class _ipointer, class _parent>
struct hkaSplineCompressedAnimation_t_shared : _parent {
  uint32 GetNumFrames() const { return this->numFrames; }
  uint32 GetNumBlocks() const { return this->numBlocks; }
  uint32 GetMaxFramesPerBlock() const { return this->maxFramesPerBlock; }
  float GetBlockDuration() const { return this->blockDuration; }
  float GetBlockInverseDuration() const { return this->blockInverseDuration; }
  float GetFrameDuration() const { return this->frameDuration; }
  const char *GetData() const { return this->data; }
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
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaSplineCompressedAnimation550_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
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

template <template <class C> class _ipointer, bool rp>
using hkaSplineCompressedAnimation550 = hkaSplineCompressedAnimation_t_shared<
    _ipointer,
    hkaSplineCompressedAnimation550_t_data<_ipointer, hkaAnimation550, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaSplineCompressedAnimation700 = hkaSplineCompressedAnimation_t_shared<
    _ipointer,
    hkaSplineCompressedAnimation550_t_data<_ipointer, hkaAnimation700, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaSplineCompressedAnimation2011 = hkaSplineCompressedAnimation_t_shared<
    _ipointer,
    hkaSplineCompressedAnimation550_t_data<_ipointer, hkaAnimation2011, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaSplineCompressedAnimation2016 = hkaSplineCompressedAnimation_t_shared<
    _ipointer,
    hkaSplineCompressedAnimation550_t_data<_ipointer, hkaAnimation2016, rp>>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkaSplineCompressedAnimationTier
    : hkaSplineCompressedAnimation2011<_ipointer, rp> {};

hkClassTier1Alias(hkaSplineCompressedAnimation, 550, 550);
hkClassTier1Alias(hkaSplineCompressedAnimation, 550, 600);
hkClassTier1Alias(hkaSplineCompressedAnimation, 550, 610);
hkClassTier1Alias(hkaSplineCompressedAnimation, 550, 650);
hkClassTier1Alias(hkaSplineCompressedAnimation, 550, 660);
hkClassTier1Alias(hkaSplineCompressedAnimation, 700, 700);
hkClassTier1Alias(hkaSplineCompressedAnimation, 700, 710);
hkClassTier1Alias(hkaSplineCompressedAnimation, 700, 2010_1);
hkClassTier1Alias(hkaSplineCompressedAnimation, 700, 2010_2);
hkClassTier1Alias(hkaSplineCompressedAnimation, 2016, 2016);
hkClassTier1Alias(hkaSplineCompressedAnimation, 2016, 2017);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterVersionedClasses(hkaSplineCompressedAnimation, 550, 600, 610, 650,
                               660, 700, 710, 2010_1, 2010_2, 2011_1, 2011_2,
                               2011_3, 2012_1, 2012_2, 2013, 2014, 2015, 2016,
                               2017)};
} // namespace

hkVirtualClass *
hkaSplineCompressedAnimationInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
