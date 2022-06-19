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

#include "internal/hka_interleavedanimation.hpp"
#include <span>

#include "hka_animation.hpp"
#include "hka_animation_interleaved.inl"

struct hkaInterleavedAnimationMidInterface
    : hkaInterleavedAnimationInternalInterface,
      hkaAnimationMidInterface<hkaAnimationLerpSampler> {
  clgen::hkaInterleavedAnimation::Interface interface;

  hkaInterleavedAnimationMidInterface(clgen::LayoutLookup rules, char *data)
      : interface {
    data, rules
  } {
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  clgen::hkaAnimation::Interface Anim() const override {
    return interface.BasehkaAnimation();
  }

  void SwapEndian() override {
    hkaAnimationMidInterface<hkaAnimationLerpSampler>::SwapEndian();
    clgen::EndianSwap(interface);

    for (std::span<hkQTransform> tms(interface.Transforms(),
                                     interface.NumTransforms());
         auto &i : tms) {
      FByteswapper(i.rotation);
      FByteswapper(i.scale);
      FByteswapper(i.translation);
    }

    for (std::span<float> refs(interface.Floats(), interface.NumFloats());
         auto &i : refs) {
      FByteswapper(i);
    }
  }

  void Process() override {
    numFrames =
        static_cast<uint32>(GetNumTransforms() / GetNumOfTransformTracks());
    frameRate = static_cast<uint32>(numFrames / Duration());
  }

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override {
    out = *GetTransform(frame * GetNumOfTransformTracks() + trackID);
  }

  size_t GetNumTransforms() const override { return interface.NumTransforms(); }
  size_t GetNumFloats() const override { return interface.NumFloats(); }

  const hkQTransform *GetTransform(size_t id) const override {
    return interface.Transforms() + id;
  }
  float GetFloat(size_t id) const override { return interface.Floats()[id]; }
};

hkVirtualClass *hkaInterleavedAnimationInternalInterface::Create(CRule rule) {
  return new hkaInterleavedAnimationMidInterface{
      clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};
}
