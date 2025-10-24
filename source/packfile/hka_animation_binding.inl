// This file has been automatically generated. Do not modify.
#include "hk_base.inl"
namespace clgen::hkaAnimationBinding {
enum Members {
  animation,
  basehkReferenceObject,
  blendHint,
  floatTrackToFloatSlotIndices,
  numFloatTrackToFloatSlotIndices,
  numPartitionIndices,
  numTransformTrackToBoneIndices,
  partitionIndices,
  skeletonName,
  transformTrackToBoneIndices,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK510, 8, 0}}, 24}, {0, -1, 20, -1, -1, -1, 16, -1, -1, 8}, {0x2000, 0x0}},
  {{{{HK550, HK550, 8, 0}}, 40}, {0, -1, 36, 24, 32, -1, 16, -1, -1, 8}, {0x2200, 0x0}},
  {{{{HK600, HK660, 8, 0}}, 48}, {8, -1, 44, 32, 40, -1, 24, -1, 0, 16}, {0x2200, 0x0}},
  {{{{HK700, HK2011_3, 8, 0}}, 72}, {24, 0, 64, 48, 56, -1, 40, -1, 16, 32}, {0x2200, 0x0}},
  {{{{HK2012_1, HK2015, 8, 0}}, 88}, {24, 0, 80, 48, 56, 72, 40, 64, 16, 32}, {0x2a00, 0x0}},
  {{{{HK2016, HK2019, 8, 0}}, 96}, {32, 0, 88, 56, 64, 80, 48, 72, 24, 40}, {0x2a00, 0x0}},
  {{{{HK500, HK510, 4, 0}}, 16}, {0, -1, 12, -1, -1, -1, 8, -1, -1, 4}, {0x2000, 0x0}},
  {{{{HK550, HK550, 4, 0}}, 24}, {0, -1, 20, 12, 16, -1, 8, -1, -1, 4}, {0x2200, 0x0}},
  {{{{HK600, HK660, 4, 0}}, 28}, {4, -1, 24, 16, 20, -1, 12, -1, 0, 8}, {0x2200, 0x0}},
  {{{{HK700, HK2011_3, 4, 0}}, 44}, {12, 0, 40, 28, 32, -1, 20, -1, 8, 16}, {0x2200, 0x0}},
  {{{{HK2012_1, HK2015, 4, 0}}, 56}, {12, 0, 52, 28, 32, 44, 20, 40, 8, 16}, {0x2a00, 0x0}},
  {{{{HK2016, HK2019, 4, 0}}, 60}, {16, 0, 56, 32, 36, 48, 24, 44, 12, 20}, {0x2a00, 0x0}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<hkaAnimation> AnimationPtr() {
    int16 off = m(animation); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  hkaAnimation *Animation() {
    int16 off = m(animation); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<hkaAnimation**>(data + off);
    return *reinterpret_cast<es::PointerX86<hkaAnimation>*>(data + off);
  }
  const hkaAnimation *Animation() const {
    int16 off = m(animation); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<hkaAnimation**>(data + off);
    return *reinterpret_cast<es::PointerX86<hkaAnimation>*>(data + off);
  }
  Pointer<int16> TransformTrackToBoneIndicesPtr() {
    int16 off = m(transformTrackToBoneIndices); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  int16 *TransformTrackToBoneIndices() {
    int16 off = m(transformTrackToBoneIndices); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  const int16 *TransformTrackToBoneIndices() const {
    int16 off = m(transformTrackToBoneIndices); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  uint32 NumTransformTrackToBoneIndices() const { return m(numTransformTrackToBoneIndices) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numTransformTrackToBoneIndices)); }
  ::BlendHint BlendHint() const { return m(blendHint) == -1 ? ::BlendHint{} : *reinterpret_cast<::BlendHint*>(data + m(blendHint)); }
  Pointer<int16> FloatTrackToFloatSlotIndicesPtr() {
    int16 off = m(floatTrackToFloatSlotIndices); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  int16 *FloatTrackToFloatSlotIndices() {
    int16 off = m(floatTrackToFloatSlotIndices); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  const int16 *FloatTrackToFloatSlotIndices() const {
    int16 off = m(floatTrackToFloatSlotIndices); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  uint32 NumFloatTrackToFloatSlotIndices() const { return m(numFloatTrackToFloatSlotIndices) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numFloatTrackToFloatSlotIndices)); }
  Pointer<char> SkeletonNamePtr() {
    int16 off = m(skeletonName); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *SkeletonName() {
    int16 off = m(skeletonName); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *SkeletonName() const {
    int16 off = m(skeletonName); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Pointer<int16> PartitionIndicesPtr() {
    int16 off = m(partitionIndices); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  int16 *PartitionIndices() {
    int16 off = m(partitionIndices); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  const int16 *PartitionIndices() const {
    int16 off = m(partitionIndices); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  uint32 NumPartitionIndices() const { return m(numPartitionIndices) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numPartitionIndices)); }
  void NumTransformTrackToBoneIndices(uint32 value) { if (m(numTransformTrackToBoneIndices) >= 0) *reinterpret_cast<uint32*>(data + m(numTransformTrackToBoneIndices)) = value; }
  void BlendHint(::BlendHint value) { if (m(blendHint) >= 0) *reinterpret_cast<::BlendHint*>(data + m(blendHint)) = value; }
  void NumFloatTrackToFloatSlotIndices(uint32 value) { if (m(numFloatTrackToFloatSlotIndices) >= 0) *reinterpret_cast<uint32*>(data + m(numFloatTrackToFloatSlotIndices)) = value; }
  void NumPartitionIndices(uint32 value) { if (m(numPartitionIndices) >= 0) *reinterpret_cast<uint32*>(data + m(numPartitionIndices)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
