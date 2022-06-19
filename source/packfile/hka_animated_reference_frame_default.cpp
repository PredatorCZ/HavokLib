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

#include "hka_animated_reference_frame.hpp"
#include "internal/hka_defaultanimrefframe.hpp"
#include <span>

#include "hka_animated_reference_frame_default.inl"

struct hkaDefaultAnimatedReferenceFrameMidInterface
    : hkaAnimatedReferenceFrameMidInterface,
      hkaDefaultAnimatedReferenceFrameInternalInterface {

  clgen::hkaDefaultAnimatedReferenceFrame::Interface interface;

  hkaDefaultAnimatedReferenceFrameMidInterface(clgen::LayoutLookup rules,
                                               char *data)
      : interface {
    data, rules
  } {
  }

  clgen::hkaAnimatedReferenceFrame::Interface Base() const override {
    return interface.BasehkaAnimatedReferenceFrame();
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void Process() override {
    frameRate = static_cast<uint32>(GetNumFrames() / GetDuration());
  }

  const Vector4A16 GetUp() const override { return interface.Up(); }
  const Vector4A16 GetForward() const override { return interface.Forward(); }
  float GetDuration() const override { return interface.Duration(); }
  size_t GetNumFrames() const override {
    return interface.NumReferenceFrameSamples();
  }

  const Vector4A16 &GetRefFrame(size_t id) const override {
    auto item = interface.ReferenceFrameSamples();
    return item[id];
  }

  void SwapEndian() override {
    clgen::EndianSwap(interface);
    Vector4A16 tmp = GetUp();
    FByteswapper(tmp);
    interface.Up(tmp);
    tmp = GetForward();
    FByteswapper(tmp);
    interface.Forward(tmp);

    for (std::span<Vector4A16> refs(interface.ReferenceFrameSamples(),
                                    interface.NumReferenceFrameSamples());
         auto &i : refs) {
      FByteswapper(i);
    }
  }
};

hkVirtualClass *
hkaDefaultAnimatedReferenceFrameInternalInterface::Create(CRule rule) {
  return new hkaDefaultAnimatedReferenceFrameMidInterface{
      clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};
}
