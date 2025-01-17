/*  Havok Format Library
    Copyright(C) 2016-2022 Lukas Cone

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

#include "hka_animation_spline.inl"

struct hkaSplineCompressedAnimationMidInterface
    : hkaSplineCompressedAnimationInternalInterface,
      hkaAnimationMidInterface<hkaAnimationInternalInterface> {
  clgen::hkaSplineCompressedAnimation::Interface interface;
  hkaSplineDecompressor decomp;

  hkaSplineCompressedAnimationMidInterface(clgen::LayoutLookup rules,
                                           char *data)
      : interface{data, rules} {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  clgen::hkaAnimation::Interface Anim() const override {
    return interface.BasehkaAnimation();
  }

  void SwapEndian() override {
    hkaAnimationMidInterface<hkaAnimationInternalInterface>::SwapEndian();
    clgen::EndianSwap(interface);

    for (std::span<uint32> tms(interface.FloatBlockOffsets(),
                               interface.NumFloatBlockOffsets());
         auto &i : tms) {
      FByteswapper(i);
    }

    for (std::span<uint32> tms(interface.BlockOffsets(),
                               interface.NumBlockOffsets());
         auto &i : tms) {
      FByteswapper(i);
    }

    for (std::span<uint32> tms(interface.FloatOffsets(),
                               interface.NumFloatOffsets());
         auto &i : tms) {
      FByteswapper(i);
    }

    for (std::span<uint32> tms(interface.TransformBlockOffsets(),
                               interface.NumTransformBlockOffsets());
         auto &i : tms) {
      FByteswapper(i);
    }
  }

  void Process() override {
    decomp.Assign(this);
    this->frameRate =
        static_cast<uint32>(this->GetNumFrames() / this->Duration());
  }

  uint32 GetNumFrames() const override { return interface.NumFrames(); }
  uint32 GetNumBlocks() const override { return interface.NumBlocks(); }
  uint32 GetMaxFramesPerBlock() const override {
    return interface.MaxFramesPerBlock();
  }
  float GetBlockDuration() const override { return interface.BlockDuration(); }
  float GetBlockInverseDuration() const override {
    return interface.BlockInverseDuration();
  }
  float GetFrameDuration() const override { return interface.FrameDuration(); }
  char *GetData() const override {
    return const_cast<char *>(interface.DataBuffer());
  }
  std::span<const uint32> GetBlockOffsets() const override {
    return {interface.BlockOffsets(), interface.NumBlockOffsets()};
  }
  std::span<const uint32> GetFloatBlockOffsets() const override {
    return {interface.FloatBlockOffsets(), interface.NumFloatBlockOffsets()};
  }
  std::span<const uint32> GetTransformOffsets() const override {
    return {interface.TransformBlockOffsets(),
            interface.NumTransformBlockOffsets()};
  }
  std::span<const uint32> GetFloatOffsets() const override {
    return {interface.FloatOffsets(), interface.NumFloatOffsets()};
  }

  void GetValue(uni::RTSValue &output, float time,
                size_t trackID) const override {
    const float blockInverseDuration = GetBlockInverseDuration();
    const float blockDuration = GetBlockDuration();
    size_t blockID = static_cast<size_t>(time * blockInverseDuration);

    if (blockID >= GetNumBlocks()) [[unlikely]] {
      blockID = GetNumBlocks() - 1;
    }

    float localTime = time - (static_cast<float>(blockID) * blockDuration);

    if (localTime < 0.f) {
      localTime = 0.f;
    }

    decomp.blocks[blockID].GetValue(trackID, localTime * frameRate, output);
  }
};

IhkVirtualClass *
hkaSplineCompressedAnimationInternalInterface::Create(CRule rule) {
  return new hkaSplineCompressedAnimationMidInterface{
      clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};
}
