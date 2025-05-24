// This file has been automatically generated. Do not modify.
#include "hka_animation.inl"
namespace clgen::hkaSplineCompressedAnimation {
enum Members {
  basehkaAnimation,
  blockDuration,
  blockInverseDuration,
  blockOffsets,
  dataBuffer,
  floatBlockOffsets,
  floatOffsets,
  frameDuration,
  maskAndQuantizationSize,
  maxFramesPerBlock,
  numBlockOffsets,
  numBlocks,
  numDataBuffer,
  numFloatBlockOffsets,
  numFloatOffsets,
  numFrames,
  numTransformBlockOffsets,
  transformBlockOffsets,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2015, 8, 0}}, 168}, {0, 72, 76, 88, 152, 104, 136, 80, 68, 64, 96, 60, 160, 112, 144, 56, 128, 120}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK2016, HK2017, 8, 0}}, 176}, {0, 80, 84, 96, 160, 112, 144, 88, 76, 72, 104, 68, 168, 120, 152, 64, 136, 128}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK500, HK510, 4, 0}}, 120}, {0, 48, 52, 60, 108, 72, 96, 56, 44, 40, 64, 36, 112, 76, 100, 32, 88, 84}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK550, HK660, 4, 0}}, 124}, {0, 52, 56, 64, 112, 76, 100, 60, 48, 44, 68, 40, 116, 80, 104, 36, 92, 88}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK700, HK2015, 4, 0}}, 128}, {0, 56, 60, 68, 116, 80, 104, 64, 52, 48, 72, 44, 120, 84, 108, 40, 96, 92}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK2016, HK2017, 4, 0}}, 132}, {0, 60, 64, 72, 120, 84, 108, 68, 56, 52, 76, 48, 124, 88, 112, 44, 100, 96}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK500, HK510, 8, 1}}, 152}, {0, 60, 64, 72, 136, 88, 120, 68, 56, 52, 80, 48, 144, 96, 128, 44, 112, 104}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK550, HK660, 8, 1}}, 160}, {0, 68, 72, 80, 144, 96, 128, 76, 64, 60, 88, 56, 152, 104, 136, 52, 120, 112}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK700, HK2015, 8, 1}}, 168}, {0, 72, 76, 88, 152, 104, 136, 80, 68, 64, 96, 60, 160, 112, 144, 56, 128, 120}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK2016, HK2017, 8, 1}}, 176}, {0, 80, 84, 96, 160, 112, 144, 88, 76, 72, 104, 68, 168, 120, 152, 64, 136, 128}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK500, HK510, 4, 1}}, 120}, {0, 48, 52, 60, 108, 72, 96, 56, 44, 40, 64, 36, 112, 76, 100, 32, 88, 84}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK550, HK660, 4, 1}}, 124}, {0, 52, 56, 64, 112, 76, 100, 60, 48, 44, 68, 40, 116, 80, 104, 36, 92, 88}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK700, HK2015, 4, 1}}, 128}, {0, 56, 60, 68, 116, 80, 104, 64, 52, 48, 72, 44, 120, 84, 108, 40, 96, 92}, {0x8028, 0xaaaa, 0x2}},
  {{{{HK2016, HK2017, 4, 1}}, 132}, {0, 60, 64, 72, 120, 84, 108, 68, 56, 52, 76, 48, 124, 88, 112, 44, 100, 96}, {0x8028, 0xaaaa, 0x2}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr ,LookupFlag::Padding}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  hkaAnimation::Interface BasehkaAnimation() const {
    int16 off = m(basehkaAnimation); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 NumFrames() const { return m(numFrames) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numFrames)); }
  uint32 NumBlocks() const { return m(numBlocks) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numBlocks)); }
  uint32 MaxFramesPerBlock() const { return m(maxFramesPerBlock) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(maxFramesPerBlock)); }
  uint32 MaskAndQuantizationSize() const { return m(maskAndQuantizationSize) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(maskAndQuantizationSize)); }
  float BlockDuration() const { return m(blockDuration) == -1 ? float{} : *reinterpret_cast<float*>(data + m(blockDuration)); }
  float BlockInverseDuration() const { return m(blockInverseDuration) == -1 ? float{} : *reinterpret_cast<float*>(data + m(blockInverseDuration)); }
  float FrameDuration() const { return m(frameDuration) == -1 ? float{} : *reinterpret_cast<float*>(data + m(frameDuration)); }
  Pointer<uint32> BlockOffsetsPtr() {
    int16 off = m(blockOffsets); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 *BlockOffsets() {
    int16 off = m(blockOffsets); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  const uint32 *BlockOffsets() const {
    int16 off = m(blockOffsets); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  uint32 NumBlockOffsets() const { return m(numBlockOffsets) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numBlockOffsets)); }
  Pointer<uint32> FloatBlockOffsetsPtr() {
    int16 off = m(floatBlockOffsets); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 *FloatBlockOffsets() {
    int16 off = m(floatBlockOffsets); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  const uint32 *FloatBlockOffsets() const {
    int16 off = m(floatBlockOffsets); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  uint32 NumFloatBlockOffsets() const { return m(numFloatBlockOffsets) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numFloatBlockOffsets)); }
  Pointer<uint32> TransformBlockOffsetsPtr() {
    int16 off = m(transformBlockOffsets); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 *TransformBlockOffsets() {
    int16 off = m(transformBlockOffsets); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  const uint32 *TransformBlockOffsets() const {
    int16 off = m(transformBlockOffsets); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  uint32 NumTransformBlockOffsets() const { return m(numTransformBlockOffsets) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numTransformBlockOffsets)); }
  Pointer<uint32> FloatOffsetsPtr() {
    int16 off = m(floatOffsets); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 *FloatOffsets() {
    int16 off = m(floatOffsets); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  const uint32 *FloatOffsets() const {
    int16 off = m(floatOffsets); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  uint32 NumFloatOffsets() const { return m(numFloatOffsets) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numFloatOffsets)); }
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
  void NumFrames(uint32 value) { if (m(numFrames) >= 0) *reinterpret_cast<uint32*>(data + m(numFrames)) = value; }
  void NumBlocks(uint32 value) { if (m(numBlocks) >= 0) *reinterpret_cast<uint32*>(data + m(numBlocks)) = value; }
  void MaxFramesPerBlock(uint32 value) { if (m(maxFramesPerBlock) >= 0) *reinterpret_cast<uint32*>(data + m(maxFramesPerBlock)) = value; }
  void MaskAndQuantizationSize(uint32 value) { if (m(maskAndQuantizationSize) >= 0) *reinterpret_cast<uint32*>(data + m(maskAndQuantizationSize)) = value; }
  void BlockDuration(float value) { if (m(blockDuration) >= 0) *reinterpret_cast<float*>(data + m(blockDuration)) = value; }
  void BlockInverseDuration(float value) { if (m(blockInverseDuration) >= 0) *reinterpret_cast<float*>(data + m(blockInverseDuration)) = value; }
  void FrameDuration(float value) { if (m(frameDuration) >= 0) *reinterpret_cast<float*>(data + m(frameDuration)) = value; }
  void NumBlockOffsets(uint32 value) { if (m(numBlockOffsets) >= 0) *reinterpret_cast<uint32*>(data + m(numBlockOffsets)) = value; }
  void NumFloatBlockOffsets(uint32 value) { if (m(numFloatBlockOffsets) >= 0) *reinterpret_cast<uint32*>(data + m(numFloatBlockOffsets)) = value; }
  void NumTransformBlockOffsets(uint32 value) { if (m(numTransformBlockOffsets) >= 0) *reinterpret_cast<uint32*>(data + m(numTransformBlockOffsets)) = value; }
  void NumFloatOffsets(uint32 value) { if (m(numFloatOffsets) >= 0) *reinterpret_cast<uint32*>(data + m(numFloatOffsets)) = value; }
  void NumDataBuffer(uint32 value) { if (m(numDataBuffer) >= 0) *reinterpret_cast<uint32*>(data + m(numDataBuffer)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
