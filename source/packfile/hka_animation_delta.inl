// This file has been automatically generated. Do not modify.
#include "hka_animation.inl"
namespace clgen::hkaDeltaCompressedAnimation {
enum Members {
  basehkaAnimation,
  blockSize,
  dataBuffer,
  lastBlockSize,
  maskDOFsIdx,
  maskDOFsSize,
  numDataBuffer,
  numDynamicTransformDOFs,
  numStaticTransformDOFs,
  numberOfPoses,
  qFormat,
  quantizedDataIdx,
  quantizedDataSize,
  staticMaskIdx,
  staticMaskSize,
  totalBlockSize,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK550, 8, 0}}, 136}, {0, 60, 120, 112, 100, 104, 128, -1, -1, 56, 64, 84, 88, 92, 96, 108}, {0x2a88, 0xaa88}},
  {{{{HK600, HK2010_2, 8, 0}}, 144}, {0, 60, 128, 120, 100, 104, 136, 112, 108, 56, 64, 84, 88, 92, 96, 116}, {0xaa88, 0xaa8a}},
  {{{{HK500, HK510, 4, 0}}, 100}, {0, 36, 92, 88, 76, 80, 96, -1, -1, 32, 40, 60, 64, 68, 72, 84}, {0x2a88, 0xaa88}},
  {{{{HK550, HK550, 4, 0}}, 104}, {0, 40, 96, 92, 80, 84, 100, -1, -1, 36, 44, 64, 68, 72, 76, 88}, {0x2a88, 0xaa88}},
  {{{{HK600, HK660, 4, 0}}, 112}, {0, 40, 104, 100, 80, 84, 108, 92, 88, 36, 44, 64, 68, 72, 76, 96}, {0xaa88, 0xaa8a}},
  {{{{HK700, HK2010_2, 4, 0}}, 120}, {0, 44, 108, 104, 84, 88, 112, 96, 92, 40, 48, 68, 72, 76, 80, 100}, {0xaa88, 0xaa8a}},
  {{{{HK500, HK510, 8, 1}}, 120}, {0, 48, 104, 100, 88, 92, 112, -1, -1, 44, 52, 72, 76, 80, 84, 96}, {0x2a88, 0xaa88}},
  {{{{HK550, HK550, 8, 1}}, 128}, {0, 56, 112, 108, 96, 100, 120, -1, -1, 52, 60, 80, 84, 88, 92, 104}, {0x2a88, 0xaa88}},
  {{{{HK600, HK660, 8, 1}}, 136}, {0, 56, 120, 116, 96, 100, 128, 108, 104, 52, 60, 80, 84, 88, 92, 112}, {0xaa88, 0xaa8a}},
  {{{{HK700, HK2010_2, 8, 1}}, 144}, {0, 60, 128, 120, 100, 104, 136, 112, 108, 56, 64, 84, 88, 92, 96, 116}, {0xaa88, 0xaa8a}},
  {{{{HK500, HK510, 4, 1}}, 100}, {0, 36, 92, 88, 76, 80, 96, -1, -1, 32, 40, 60, 64, 68, 72, 84}, {0x2a88, 0xaa88}},
  {{{{HK550, HK550, 4, 1}}, 104}, {0, 40, 96, 92, 80, 84, 100, -1, -1, 36, 44, 64, 68, 72, 76, 88}, {0x2a88, 0xaa88}},
  {{{{HK600, HK660, 4, 1}}, 112}, {0, 40, 104, 100, 80, 84, 108, 92, 88, 36, 44, 64, 68, 72, 76, 96}, {0xaa88, 0xaa8a}},
  {{{{HK700, HK2010_2, 4, 1}}, 120}, {0, 44, 108, 104, 84, 88, 112, 96, 92, 40, 48, 68, 72, 76, 80, 100}, {0xaa88, 0xaa8a}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Padding ,LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  hkaAnimation::Interface BasehkaAnimation() const {
    int16 off = m(basehkaAnimation); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 NumberOfPoses() const { return m(numberOfPoses) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numberOfPoses)); }
  uint32 BlockSize() const { return m(blockSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(blockSize)); }
  QuantizationFormat QFormat() const { return m(qFormat) == -1 ? QuantizationFormat{} : *reinterpret_cast<QuantizationFormat*>(data + m(qFormat)); }
  uint32 QuantizedDataIdx() const { return m(quantizedDataIdx) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(quantizedDataIdx)); }
  uint32 QuantizedDataSize() const { return m(quantizedDataSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(quantizedDataSize)); }
  uint32 StaticMaskIdx() const { return m(staticMaskIdx) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(staticMaskIdx)); }
  uint32 StaticMaskSize() const { return m(staticMaskSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(staticMaskSize)); }
  uint32 MaskDOFsIdx() const { return m(maskDOFsIdx) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(maskDOFsIdx)); }
  uint32 MaskDOFsSize() const { return m(maskDOFsSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(maskDOFsSize)); }
  uint32 TotalBlockSize() const { return m(totalBlockSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(totalBlockSize)); }
  uint32 LastBlockSize() const { return m(lastBlockSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(lastBlockSize)); }
  Pointer<char> DataBufferPtr() {
    int16 off = m(dataBuffer); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *DataBuffer() {
    int16 off = m(dataBuffer); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *DataBuffer() const {
    int16 off = m(dataBuffer); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  uint32 NumDataBuffer() const { return m(numDataBuffer) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numDataBuffer)); }
  uint32 NumStaticTransformDOFs() const { return m(numStaticTransformDOFs) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numStaticTransformDOFs)); }
  uint32 NumDynamicTransformDOFs() const { return m(numDynamicTransformDOFs) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numDynamicTransformDOFs)); }
  void NumberOfPoses(uint32 value) { if (m(numberOfPoses) >= 0) *reinterpret_cast<uint32*>(data + m(numberOfPoses)) = value; }
  void BlockSize(uint32 value) { if (m(blockSize) >= 0) *reinterpret_cast<uint32*>(data + m(blockSize)) = value; }
  void QFormat(QuantizationFormat value) { if (m(qFormat) >= 0) *reinterpret_cast<QuantizationFormat*>(data + m(qFormat)) = value; }
  void QuantizedDataIdx(uint32 value) { if (m(quantizedDataIdx) >= 0) *reinterpret_cast<uint32*>(data + m(quantizedDataIdx)) = value; }
  void QuantizedDataSize(uint32 value) { if (m(quantizedDataSize) >= 0) *reinterpret_cast<uint32*>(data + m(quantizedDataSize)) = value; }
  void StaticMaskIdx(uint32 value) { if (m(staticMaskIdx) >= 0) *reinterpret_cast<uint32*>(data + m(staticMaskIdx)) = value; }
  void StaticMaskSize(uint32 value) { if (m(staticMaskSize) >= 0) *reinterpret_cast<uint32*>(data + m(staticMaskSize)) = value; }
  void MaskDOFsIdx(uint32 value) { if (m(maskDOFsIdx) >= 0) *reinterpret_cast<uint32*>(data + m(maskDOFsIdx)) = value; }
  void MaskDOFsSize(uint32 value) { if (m(maskDOFsSize) >= 0) *reinterpret_cast<uint32*>(data + m(maskDOFsSize)) = value; }
  void TotalBlockSize(uint32 value) { if (m(totalBlockSize) >= 0) *reinterpret_cast<uint32*>(data + m(totalBlockSize)) = value; }
  void LastBlockSize(uint32 value) { if (m(lastBlockSize) >= 0) *reinterpret_cast<uint32*>(data + m(lastBlockSize)) = value; }
  void NumDataBuffer(uint32 value) { if (m(numDataBuffer) >= 0) *reinterpret_cast<uint32*>(data + m(numDataBuffer)) = value; }
  void NumStaticTransformDOFs(uint32 value) { if (m(numStaticTransformDOFs) >= 0) *reinterpret_cast<uint32*>(data + m(numStaticTransformDOFs)) = value; }
  void NumDynamicTransformDOFs(uint32 value) { if (m(numDynamicTransformDOFs) >= 0) *reinterpret_cast<uint32*>(data + m(numDynamicTransformDOFs)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
