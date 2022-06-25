// This file has been automatically generated. Do not modify.
#include "hka_animation.inl"
namespace clgen::hkaWaveletCompressedAnimation {
enum Members {
  basehkaAnimation,
  blockIndexIdx,
  blockIndexSize,
  blockSize,
  dataBuffer,
  numDataBuffer,
  numDynamicTransformDOFs,
  numStaticTransformDOFs,
  numberOfPoses,
  qFormat,
  quantizedDataIdx,
  quantizedDataSize,
  staticDOFsIdx,
  staticMaskIdx,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK550, 8, 0}}, 128}, {0, 92, 96, 60, 112, 120, -1, -1, 56, 64, 100, 104, 88, 84}, {0x8a8, 0xaa2}},
  {{{{HK600, HK2010_2, 8, 0}}, 136}, {0, 100, 104, 60, 120, 128, 96, 92, 56, 64, 108, 112, 88, 84}, {0xa8a8, 0xaa2}},
  {{{{HK500, HK510, 4, 0}}, 92}, {0, 68, 72, 36, 84, 88, -1, -1, 32, 40, 76, 80, 64, 60}, {0x8a8, 0xaa2}},
  {{{{HK550, HK550, 4, 0}}, 96}, {0, 72, 76, 40, 88, 92, -1, -1, 36, 44, 80, 84, 68, 64}, {0x8a8, 0xaa2}},
  {{{{HK600, HK660, 4, 0}}, 104}, {0, 80, 84, 40, 96, 100, 76, 72, 36, 44, 88, 92, 68, 64}, {0xa8a8, 0xaa2}},
  {{{{HK700, HK2010_2, 4, 0}}, 112}, {0, 84, 88, 44, 100, 104, 80, 76, 40, 48, 92, 96, 72, 68}, {0xa8a8, 0xaa2}},
  {{{{HK500, HK510, 8, 1}}, 112}, {0, 80, 84, 48, 96, 104, -1, -1, 44, 52, 88, 92, 76, 72}, {0x8a8, 0xaa2}},
  {{{{HK550, HK550, 8, 1}}, 120}, {0, 88, 92, 56, 104, 112, -1, -1, 52, 60, 96, 100, 84, 80}, {0x8a8, 0xaa2}},
  {{{{HK600, HK660, 8, 1}}, 128}, {0, 96, 100, 56, 112, 120, 92, 88, 52, 60, 104, 108, 84, 80}, {0xa8a8, 0xaa2}},
  {{{{HK700, HK2010_2, 8, 1}}, 136}, {0, 100, 104, 60, 120, 128, 96, 92, 56, 64, 108, 112, 88, 84}, {0xa8a8, 0xaa2}},
  {{{{HK500, HK510, 4, 1}}, 92}, {0, 68, 72, 36, 84, 88, -1, -1, 32, 40, 76, 80, 64, 60}, {0x8a8, 0xaa2}},
  {{{{HK550, HK550, 4, 1}}, 96}, {0, 72, 76, 40, 88, 92, -1, -1, 36, 44, 80, 84, 68, 64}, {0x8a8, 0xaa2}},
  {{{{HK600, HK660, 4, 1}}, 104}, {0, 80, 84, 40, 96, 100, 76, 72, 36, 44, 88, 92, 68, 64}, {0xa8a8, 0xaa2}},
  {{{{HK700, HK2010_2, 4, 1}}, 112}, {0, 84, 88, 44, 100, 104, 80, 76, 40, 48, 92, 96, 72, 68}, {0xa8a8, 0xaa2}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Padding ,LookupFlag::Ptr}})}, lookup{layout_} {}
  uint16 LayoutVersion() const { return lookup.version; }
  hkaAnimation::Interface BasehkaAnimation() const {
    int16 off = m(basehkaAnimation); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 NumberOfPoses() const { return m(numberOfPoses) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numberOfPoses)); }
  uint32 BlockSize() const { return m(blockSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(blockSize)); }
  QuantizationFormat QFormat() const { return m(qFormat) == -1 ? QuantizationFormat{} : *reinterpret_cast<QuantizationFormat*>(data + m(qFormat)); }
  uint32 StaticMaskIdx() const { return m(staticMaskIdx) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(staticMaskIdx)); }
  uint32 StaticDOFsIdx() const { return m(staticDOFsIdx) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(staticDOFsIdx)); }
  uint32 BlockIndexIdx() const { return m(blockIndexIdx) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(blockIndexIdx)); }
  uint32 BlockIndexSize() const { return m(blockIndexSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(blockIndexSize)); }
  uint32 QuantizedDataIdx() const { return m(quantizedDataIdx) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(quantizedDataIdx)); }
  uint32 QuantizedDataSize() const { return m(quantizedDataSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(quantizedDataSize)); }
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
  void StaticMaskIdx(uint32 value) { if (m(staticMaskIdx) >= 0) *reinterpret_cast<uint32*>(data + m(staticMaskIdx)) = value; }
  void StaticDOFsIdx(uint32 value) { if (m(staticDOFsIdx) >= 0) *reinterpret_cast<uint32*>(data + m(staticDOFsIdx)) = value; }
  void BlockIndexIdx(uint32 value) { if (m(blockIndexIdx) >= 0) *reinterpret_cast<uint32*>(data + m(blockIndexIdx)) = value; }
  void BlockIndexSize(uint32 value) { if (m(blockIndexSize) >= 0) *reinterpret_cast<uint32*>(data + m(blockIndexSize)) = value; }
  void QuantizedDataIdx(uint32 value) { if (m(quantizedDataIdx) >= 0) *reinterpret_cast<uint32*>(data + m(quantizedDataIdx)) = value; }
  void QuantizedDataSize(uint32 value) { if (m(quantizedDataSize) >= 0) *reinterpret_cast<uint32*>(data + m(quantizedDataSize)) = value; }
  void NumDataBuffer(uint32 value) { if (m(numDataBuffer) >= 0) *reinterpret_cast<uint32*>(data + m(numDataBuffer)) = value; }
  void NumStaticTransformDOFs(uint32 value) { if (m(numStaticTransformDOFs) >= 0) *reinterpret_cast<uint32*>(data + m(numStaticTransformDOFs)) = value; }
  void NumDynamicTransformDOFs(uint32 value) { if (m(numDynamicTransformDOFs) >= 0) *reinterpret_cast<uint32*>(data + m(numDynamicTransformDOFs)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
