// This file has been automatically generated. Do not modify.
#include "hk_base.inl"
namespace clgen::hkaBone {
enum Members {
  lockTranslation,
  name,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2019, 8, 0}}, 16}, {8, 0}, {0x2}},
  {{{{HK500, HK2019, 4, 0}}, 8}, {4, 0}, {0x2}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<char> NamePtr() {
    int16 off = m(name); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *Name() {
    int16 off = m(name); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *Name() const {
    int16 off = m(name); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  int32 LockTranslation() const { return m(lockTranslation) == -1 ? int32{} : *reinterpret_cast<int32*>(data + m(lockTranslation)); }
  void LockTranslation(int32 value) { if (m(lockTranslation) >= 0) *reinterpret_cast<int32*>(data + m(lockTranslation)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkLocalFrameOnBone {
enum Members {
  boneIndex,
  localFrame,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2010_2, 8, 0}}, 16}, {8, 0}, {0x2}},
  {{{{HK2011_1, HK2019, 8, 0}}, 16}, {8, 0}, {0x1}},
  {{{{HK500, HK2010_2, 4, 0}}, 8}, {4, 0}, {0x2}},
  {{{{HK2011_1, HK2019, 4, 0}}, 8}, {4, 0}, {0x1}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<hkLocalFrame> LocalFramePtr() {
    int16 off = m(localFrame); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  hkLocalFrame *LocalFrame() {
    int16 off = m(localFrame); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<hkLocalFrame**>(data + off);
    return *reinterpret_cast<es::PointerX86<hkLocalFrame>*>(data + off);
  }
  const hkLocalFrame *LocalFrame() const {
    int16 off = m(localFrame); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<hkLocalFrame**>(data + off);
    return *reinterpret_cast<es::PointerX86<hkLocalFrame>*>(data + off);
  }
  int32 BoneIndex() const { return m(boneIndex) == -1 ? int32{} : *reinterpret_cast<int32*>(data + m(boneIndex)); }
  int16 BoneIndexHK2011_1() const { return m(boneIndex) == -1 ? int16{} : *reinterpret_cast<int16*>(data + m(boneIndex)); }
  void BoneIndex(int32 value) { if (m(boneIndex) >= 0) *reinterpret_cast<int32*>(data + m(boneIndex)) = value; }
  void BoneIndexHK2011_1(int16 value) { if (m(boneIndex) >= 0) *reinterpret_cast<int16*>(data + m(boneIndex)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkaPartition {
enum Members {
  name,
  numBones,
  startBoneIndex,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2019, 8, 0}}, 16}, {0, 10, 8}, {0x14}},
  {{{{HK500, HK2019, 4, 0}}, 8}, {0, 6, 4}, {0x14}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<char> NamePtr() {
    int16 off = m(name); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *Name() {
    int16 off = m(name); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *Name() const {
    int16 off = m(name); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  int16 StartBoneIndex() const { return m(startBoneIndex) == -1 ? int16{} : *reinterpret_cast<int16*>(data + m(startBoneIndex)); }
  uint16 NumBones() const { return m(numBones) == -1 ? uint16{} : *reinterpret_cast<uint16*>(data + m(numBones)); }
  void StartBoneIndex(int16 value) { if (m(startBoneIndex) >= 0) *reinterpret_cast<int16*>(data + m(startBoneIndex)) = value; }
  void NumBones(uint16 value) { if (m(numBones) >= 0) *reinterpret_cast<uint16*>(data + m(numBones)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkaSkeleton {
enum Members {
  basehkReferenceObject,
  bones,
  floatSlots,
  localFrames,
  name,
  numBones,
  numFloatSlots,
  numLocalFrames,
  numParentIndices,
  numPartitions,
  numReferenceFloats,
  numTransforms,
  parentIndices,
  partitions,
  referenceFloats,
  transforms,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK510, 8, 0}}, 56}, {-1, 24, -1, -1, 0, 32, -1, -1, 16, -1, -1, 48, 8, -1, -1, 40}, {0x800, 0x82}},
  {{{{HK550, HK550, 8, 0}}, 72}, {-1, 24, 56, -1, 0, 32, 64, -1, 16, -1, -1, 48, 8, -1, -1, 40}, {0x2800, 0x82}},
  {{{{HK600, HK660, 8, 0}}, 88}, {-1, 24, 56, 72, 0, 32, 64, 80, 16, -1, -1, 48, 8, -1, -1, 40}, {0xa800, 0x82}},
  {{{{HK700, HK710, 8, 0}}, 104}, {0, 40, 72, 88, 16, 48, 80, 96, 32, -1, -1, 64, 24, -1, -1, 56}, {0xa800, 0x82}},
  {{{{HK2010_1, HK2011_3, 8, 0}}, 120}, {0, 40, 88, 104, 16, 48, 96, 112, 32, -1, 80, 64, 24, -1, 72, 56}, {0xa800, 0xa2}},
  {{{{HK2012_1, HK2015, 8, 0}}, 136}, {0, 40, 88, 104, 16, 48, 96, 112, 32, 128, 80, 64, 24, 120, 72, 56}, {0xa800, 0xaa}},
  {{{{HK2016, HK2019, 8, 0}}, 144}, {0, 48, 96, 112, 24, 56, 104, 120, 40, 136, 88, 72, 32, 128, 80, 64}, {0xa800, 0xaa}},
  {{{{HK500, HK510, 4, 0}}, 28}, {-1, 12, -1, -1, 0, 16, -1, -1, 8, -1, -1, 24, 4, -1, -1, 20}, {0x800, 0x82}},
  {{{{HK550, HK550, 4, 0}}, 36}, {-1, 12, 28, -1, 0, 16, 32, -1, 8, -1, -1, 24, 4, -1, -1, 20}, {0x2800, 0x82}},
  {{{{HK600, HK660, 4, 0}}, 44}, {-1, 12, 28, 36, 0, 16, 32, 40, 8, -1, -1, 24, 4, -1, -1, 20}, {0xa800, 0x82}},
  {{{{HK700, HK710, 4, 0}}, 72}, {0, 24, 48, 60, 8, 28, 52, 64, 16, -1, -1, 40, 12, -1, -1, 36}, {0xa800, 0x82}},
  {{{{HK2010_1, HK2011_3, 4, 0}}, 84}, {0, 24, 60, 72, 8, 28, 64, 76, 16, -1, 52, 40, 12, -1, 48, 36}, {0xa800, 0xa2}},
  {{{{HK2012_1, HK2015, 4, 0}}, 96}, {0, 24, 60, 72, 8, 28, 64, 76, 16, 88, 52, 40, 12, 84, 48, 36}, {0xa800, 0xaa}},
  {{{{HK2016, HK2019, 4, 0}}, 100}, {0, 28, 64, 76, 12, 32, 68, 80, 20, 92, 56, 44, 16, 88, 52, 40}, {0xa800, 0xaa}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<char> NamePtr() {
    int16 off = m(name); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *Name() {
    int16 off = m(name); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *Name() const {
    int16 off = m(name); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  Pointer<int16> ParentIndicesPtr() {
    int16 off = m(parentIndices); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  int16 *ParentIndices() {
    int16 off = m(parentIndices); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  const int16 *ParentIndices() const {
    int16 off = m(parentIndices); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  uint32 NumParentIndices() const { return m(numParentIndices) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numParentIndices)); }
  Pointer<Pointer<hkaBone::Interface>> BonesPtr() {
    int16 off = m(bones); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkaBone::Interface>> Bones() {
    int16 off = m(bones); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkaBone::Interface>> Bones() const {
    int16 off = m(bones); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumBones() const { return m(numBones) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numBones)); }
  Pointer<hkaBone::Interface> BonesPtrHK700() {
    int16 off = m(bones); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkaBone::Interface> BonesHK700() {
    int16 off = m(bones); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkaBone::Interface> BonesHK700() const {
    int16 off = m(bones); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Pointer<hkQTransform> TransformsPtr() {
    int16 off = m(transforms); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  hkQTransform *Transforms() {
    int16 off = m(transforms); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<hkQTransform**>(data + off);
    return *reinterpret_cast<es::PointerX86<hkQTransform>*>(data + off);
  }
  const hkQTransform *Transforms() const {
    int16 off = m(transforms); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<hkQTransform**>(data + off);
    return *reinterpret_cast<es::PointerX86<hkQTransform>*>(data + off);
  }
  uint32 NumTransforms() const { return m(numTransforms) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numTransforms)); }
  Pointer<Pointer<char>> FloatSlotsPtr() {
    int16 off = m(floatSlots); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<char>> FloatSlots() {
    int16 off = m(floatSlots); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<char>> FloatSlots() const {
    int16 off = m(floatSlots); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumFloatSlots() const { return m(numFloatSlots) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numFloatSlots)); }
  Pointer<hkLocalFrameOnBone::Interface> LocalFramesPtr() {
    int16 off = m(localFrames); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkLocalFrameOnBone::Interface> LocalFrames() {
    int16 off = m(localFrames); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkLocalFrameOnBone::Interface> LocalFrames() const {
    int16 off = m(localFrames); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumLocalFrames() const { return m(numLocalFrames) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numLocalFrames)); }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Pointer<float> ReferenceFloatsPtr() {
    int16 off = m(referenceFloats); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  float *ReferenceFloats() {
    int16 off = m(referenceFloats); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  const float *ReferenceFloats() const {
    int16 off = m(referenceFloats); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  uint32 NumReferenceFloats() const { return m(numReferenceFloats) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numReferenceFloats)); }
  Pointer<hkaPartition::Interface> PartitionsPtr() {
    int16 off = m(partitions); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkaPartition::Interface> Partitions() {
    int16 off = m(partitions); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkaPartition::Interface> Partitions() const {
    int16 off = m(partitions); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumPartitions() const { return m(numPartitions) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numPartitions)); }
  void NumParentIndices(uint32 value) { if (m(numParentIndices) >= 0) *reinterpret_cast<uint32*>(data + m(numParentIndices)) = value; }
  void NumBones(uint32 value) { if (m(numBones) >= 0) *reinterpret_cast<uint32*>(data + m(numBones)) = value; }
  void NumTransforms(uint32 value) { if (m(numTransforms) >= 0) *reinterpret_cast<uint32*>(data + m(numTransforms)) = value; }
  void NumFloatSlots(uint32 value) { if (m(numFloatSlots) >= 0) *reinterpret_cast<uint32*>(data + m(numFloatSlots)) = value; }
  void NumLocalFrames(uint32 value) { if (m(numLocalFrames) >= 0) *reinterpret_cast<uint32*>(data + m(numLocalFrames)) = value; }
  void NumReferenceFloats(uint32 value) { if (m(numReferenceFloats) >= 0) *reinterpret_cast<uint32*>(data + m(numReferenceFloats)) = value; }
  void NumPartitions(uint32 value) { if (m(numPartitions) >= 0) *reinterpret_cast<uint32*>(data + m(numPartitions)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
