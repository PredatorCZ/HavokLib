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

#include "hkaAnimation.h"
#include "hkaDeltaDecompressor.h"

namespace {

template <class C>
struct hkaDeltaCompressedAnimation_t
    : hkaDeltaCompressedAnimationInternalInterface,
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
  void Process() override {
    decomp.Assign(this);
    this->numFrames = GetNumOfPoses();
    this->frameRate = static_cast<uint32>(this->numFrames / this->Duration());
  }
  hkaDeltaDecompressor decomp;

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override {
    decomp.GetFrame(trackID, frame, out);
  };
  size_t GetNumOfPoses() const override { return Data()->GetNumOfPoses(); }
  size_t GetBlockSize() const override { return Data()->GetBlockSize(); }
  size_t GetQuantizedDataOffset() const override {
    return Data()->GetQuantizedDataOffset();
  }
  size_t GetStaticMaskOffset() const override {
    return Data()->GetStaticMaskOffset();
  }
  size_t GetStaticDataOffset() const override {
    return Data()->GetStaticDataOffset();
  }
  const char *GetData() const override { return Data()->GetData(); }
  size_t GetNumDynamicTracks() const override {
    return Data()->GetNumDynamicTracks();
  }
  size_t GetOffsetsOffset() const override {
    return Data()->GetOffsetsOffset();
  }
  size_t GetBitWidthOffset() const override {
    return Data()->GetBitWidthOffset();
  }
  size_t GetScalesOffset() const override { return Data()->GetScalesOffset(); }
  size_t GetNumPreserved() const override { return Data()->GetNumPreserved(); }
};

template <template <class C> class _ipointer, class _parent>
struct hkaDeltaCompressedAnimation_t_shared : _parent {
  size_t GetNumOfPoses() const { return this->numberOfPoses; }
  size_t GetBlockSize() const { return this->blockSize; }
  size_t GetQuantizedDataOffset() const { return this->quantizedDataIdx; }
  size_t GetStaticMaskOffset() const { return this->staticMaskIdx; }
  size_t GetStaticDataOffset() const { return this->maskDOFsIdx; }
  const char *GetData() const {
    return reinterpret_cast<const char *>(
        static_cast<const uint8 *>(this->dataBuffer));
  }
  size_t GetNumDynamicTracks() const { return this->qFormat.numD; }
  size_t GetOffsetsOffset() const { return this->qFormat.offsetIdx; }
  size_t GetBitWidthOffset() const { return this->qFormat.bitWidthIdx; }
  size_t GetScalesOffset() const { return this->qFormat.scaleIdx; }
  size_t GetNumPreserved() const { return this->qFormat.preserved; }

  void SwapEndian() {
    FByteswapper(this->numberOfPoses);
    FByteswapper(this->blockSize);
    FByteswapper(this->quantizedDataIdx);
    FByteswapper(this->quantizedDataSize);
    FByteswapper(this->staticMaskIdx);
    FByteswapper(this->staticMaskSize);
    FByteswapper(this->maskDOFsIdx);
    FByteswapper(this->maskDOFsSize);
    FByteswapper(this->totalBlockSize);
    FByteswapper(this->lastBlockSize);
    FByteswapper(this->qFormat);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaDeltaCompressedAnimation500_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
  uint32 numberOfPoses;
  uint32 blockSize;
  QuantizationFormat qFormat;
  uint32 quantizedDataIdx;
  uint32 quantizedDataSize;
  uint32 staticMaskIdx;
  uint32 staticMaskSize;
  uint32 maskDOFsIdx;
  uint32 maskDOFsSize;
  uint32 totalBlockSize;
  uint32 lastBlockSize;
  _ipointer<uint8> dataBuffer;
  uint32 numDataBuffer;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaDeltaCompressedAnimation600_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
  uint32 numberOfPoses;
  uint32 blockSize;
  QuantizationFormat qFormat;
  uint32 quantizedDataIdx;
  uint32 quantizedDataSize;
  uint32 staticMaskIdx;
  uint32 staticMaskSize;
  uint32 maskDOFsIdx;
  uint32 maskDOFsSize;
  uint32 numStaticTransformDOFs;
  uint32 numDynamicTransformDOFs;
  uint32 totalBlockSize;
  uint32 lastBlockSize;
  _ipointer<uint8> dataBuffer;
  uint32 numDataBuffer;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaDeltaCompressedAnimation700_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
  uint32 numberOfPoses;
  uint32 blockSize;
  QuantizationFormat qFormat;
  uint32 quantizedDataIdx;
  uint32 quantizedDataSize;
  uint32 staticMaskIdx;
  uint32 staticMaskSize;
  uint32 maskDOFsIdx;
  uint32 maskDOFsSize;
  uint32 numStaticTransformDOFs;
  uint32 numDynamicTransformDOFs;
  uint32 totalBlockSize;
  uint32 lastBlockSize;
  hkArray<uint8, _ipointer> dataBuffer;
};

template <template <class C> class _ipointer, bool rp>
using hkaDeltaCompressedAnimation500 = hkaDeltaCompressedAnimation_t_shared<
    _ipointer,
    hkaDeltaCompressedAnimation500_t_data<_ipointer, hkaAnimation500, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaDeltaCompressedAnimation550 = hkaDeltaCompressedAnimation_t_shared<
    _ipointer,
    hkaDeltaCompressedAnimation500_t_data<_ipointer, hkaAnimation550, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaDeltaCompressedAnimation600 = hkaDeltaCompressedAnimation_t_shared<
    _ipointer,
    hkaDeltaCompressedAnimation600_t_data<_ipointer, hkaAnimation550, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaDeltaCompressedAnimation700 = hkaDeltaCompressedAnimation_t_shared<
    _ipointer,
    hkaDeltaCompressedAnimation700_t_data<_ipointer, hkaAnimation700, rp>>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkaDeltaCompressedAnimationTier
    : hkaDeltaCompressedAnimation700<_ipointer, rp> {};

hkClassTier1Alias(hkaDeltaCompressedAnimation, 500, 500);
hkClassTier1Alias(hkaDeltaCompressedAnimation, 500, 510);
hkClassTier1Alias(hkaDeltaCompressedAnimation, 550, 550);
hkClassTier1Alias(hkaDeltaCompressedAnimation, 600, 600);
hkClassTier1Alias(hkaDeltaCompressedAnimation, 600, 610);
hkClassTier1Alias(hkaDeltaCompressedAnimation, 600, 650);
hkClassTier1Alias(hkaDeltaCompressedAnimation, 600, 660);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterVersionedClasses(hkaDeltaCompressedAnimation, 500, 510, 550, 600,
                               610, 650, 660, 700, 710, 2010_1, 2010_2)};
} // namespace

hkVirtualClass *
hkaDeltaCompressedAnimationInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}