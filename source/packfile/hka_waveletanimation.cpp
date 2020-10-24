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

#include "internal/hka_waveletanimation.hpp"
#include "hka_animation.hpp"
#include "hka_decompressor.hpp"
//#include "hkaWaveletDecompressor.h"

namespace {

template <class C>
struct hkaWaveletCompressedAnimation_t
    : hkaWaveletCompressedAnimationInternalInterface,
      hkaAnimation_t<typename C::parentClass, hkaAnimationLerpSampler> {
  typedef C value_type;
  typedef hkaAnimation_t<typename C::parentClass, hkaAnimationLerpSampler>
      parent;

  value_type *Data() { return static_cast<value_type *>(this->data.get()); }
  const value_type *Data() const {
    return static_cast<const value_type *>(this->data.get());
  }

  void SwapEndian() override {
    parent::SwapEndian();
    Data()->SwapEndian();
  }
  void Process() override {} //{ decomp.Assign(this); }
  // hkaWaveletDecompressor decomp;
  size_t GetNumOfPoses() const { return Data()->GetNumOfPoses(); }
  size_t GetBlockSize() const { return Data()->GetBlockSize(); }
  size_t GetQuantizedDataOffset() const {
    return Data()->GetQuantizedDataOffset();
  }
  size_t GetStaticMaskOffset() const { return Data()->GetStaticMaskOffset(); }
  size_t GetStaticDataOffset() const { return Data()->GetStaticDataOffset(); }
  const char *GetData() const { return Data()->GetData(); }
  size_t GetNumDynamicTracks() const { return Data()->GetNumDynamicTracks(); }
  size_t GetOffsetsOffset() const { return Data()->GetOffsetsOffset(); }
  size_t GetBitWidthOffset() const { return Data()->GetBitWidthOffset(); }
  size_t GetScalesOffset() const { return Data()->GetScalesOffset(); }
  size_t GetNumPreserved() const { return Data()->GetNumPreserved(); }

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override{
      // decomp.GetFrame(trackID, frame, out);
  };
};

template <template <class C> class _ipointer, class _parent>
struct hkaWaveletCompressedAnimation_t_shared : _parent {
  uint32 GetNumOfPoses() const { return this->numberOfPoses; }
  uint32 GetBlockSize() const { return this->blockSize; }
  uint32 GetQuantizedDataOffset() const { return this->quantizedDataIdx; }
  uint32 GetStaticMaskOffset() const { return this->staticMaskIdx; }
  uint32 GetStaticDataOffset() const { return this->staticDOFsIdx; }
  const char *GetData() const {
    return reinterpret_cast<const char *>(
        static_cast<const uint8 *>(this->dataBuffer.GetData()));
  }
  uint32 GetNumDynamicTracks() const { return this->qFormat.numD; }
  uint32 GetOffsetsOffset() const { return this->qFormat.offsetIdx; }
  uint32 GetBitWidthOffset() const { return this->qFormat.bitWidthIdx; }
  uint32 GetScalesOffset() const { return this->qFormat.scaleIdx; }
  uint32 GetNumPreserved() const { return this->qFormat.preserved; }

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
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaWaveletCompressedAnimation500_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
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

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaWaveletCompressedAnimation600_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
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

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaWaveletCompressedAnimation700_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
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

template <template <class C> class _ipointer, bool rp>
using hkaWaveletCompressedAnimation500 = hkaWaveletCompressedAnimation_t_shared<
    _ipointer,
    hkaWaveletCompressedAnimation500_t_data<_ipointer, hkaAnimation500, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaWaveletCompressedAnimation550 = hkaWaveletCompressedAnimation_t_shared<
    _ipointer,
    hkaWaveletCompressedAnimation500_t_data<_ipointer, hkaAnimation550, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaWaveletCompressedAnimation600 = hkaWaveletCompressedAnimation_t_shared<
    _ipointer,
    hkaWaveletCompressedAnimation600_t_data<_ipointer, hkaAnimation550, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaWaveletCompressedAnimation700 = hkaWaveletCompressedAnimation_t_shared<
    _ipointer,
    hkaWaveletCompressedAnimation700_t_data<_ipointer, hkaAnimation700, rp>>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkaWaveletCompressedAnimationTier
    : hkaWaveletCompressedAnimation700<_ipointer, rp> {};

hkClassTier1Alias(hkaWaveletCompressedAnimation, 500, 500);
hkClassTier1Alias(hkaWaveletCompressedAnimation, 500, 510);
hkClassTier1Alias(hkaWaveletCompressedAnimation, 550, 550);
hkClassTier1Alias(hkaWaveletCompressedAnimation, 600, 600);
hkClassTier1Alias(hkaWaveletCompressedAnimation, 600, 610);
hkClassTier1Alias(hkaWaveletCompressedAnimation, 600, 650);
hkClassTier1Alias(hkaWaveletCompressedAnimation, 600, 660);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterVersionedClasses(hkaWaveletCompressedAnimation, 500, 510, 550,
                               600, 610, 650, 660, 700, 710, 2010_1, 2010_2)};
} // namespace

hkVirtualClass *
hkaWaveletCompressedAnimationInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
