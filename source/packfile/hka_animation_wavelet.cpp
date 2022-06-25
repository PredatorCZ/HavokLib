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

#include "internal/hka_waveletanimation.hpp"
#include "hka_animation.hpp"
#include "hka_decompressor.hpp"
#include "hka_animation_wavelet.inl"
//#include "hkaWaveletDecompressor.h"

struct hkaWaveletCompressedAnimationMidInterface
    : hkaWaveletCompressedAnimationInternalInterface,
      hkaAnimationMidInterface<hkaAnimationLerpSampler> {
  clgen::hkaWaveletCompressedAnimation::Interface interface;
  //hkaWaveletDecompressor decomp;

  hkaWaveletCompressedAnimationMidInterface(clgen::LayoutLookup rules, char *data)
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
    auto fmt = interface.QFormat();
    FByteswapper(fmt);
    interface.QFormat(fmt);
  }

  void Process() override {} //{ decomp.Assign(this); }

  size_t GetNumOfPoses() const { return interface.NumberOfPoses(); }
  size_t GetBlockSize() const { return interface.BlockSize(); }
  size_t GetQuantizedDataOffset() const {
    return interface.QuantizedDataIdx();
  }
  size_t GetStaticMaskOffset() const { return interface.StaticMaskIdx(); }
  size_t GetStaticDataOffset() const { return interface.StaticDOFsIdx(); }
  const char *GetData() const { return interface.DataBuffer(); }
  size_t GetNumDynamicTracks() const { return interface.QFormat().numD; }
  size_t GetOffsetsOffset() const { return interface.QFormat().offsetIdx; }
  size_t GetBitWidthOffset() const { return interface.QFormat().bitWidthIdx; }
  size_t GetScalesOffset() const { return interface.QFormat().scaleIdx; }
  size_t GetNumPreserved() const { return interface.QFormat().preserved; }

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override{
      // decomp.GetFrame(trackID, frame, out);
  };
};

IhkVirtualClass *hkaWaveletCompressedAnimationInternalInterface::Create(CRule rule) {
  return new hkaWaveletCompressedAnimationMidInterface{
       clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};

}
