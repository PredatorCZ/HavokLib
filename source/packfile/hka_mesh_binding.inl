// This file has been automatically generated. Do not modify.
#include "hk_base.inl"
namespace clgen::hkaMeshBindingMapping {
enum Members {
  mapping,
  numMapping,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2019, 8, 0}}, 16}, {0, 8}, {0x8}},
  {{{{HK500, HK660, 4, 0}}, 8}, {0, 4}, {0x8}},
  {{{{HK700, HK2019, 4, 0}}, 12}, {0, 4}, {0x8}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<int16> MappingPtr() {
    int16 off = m(mapping); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  int16 *Mapping() {
    int16 off = m(mapping); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  const int16 *Mapping() const {
    int16 off = m(mapping); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  uint32 NumMapping() const { return m(numMapping) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numMapping)); }
  void NumMapping(uint32 value) { if (m(numMapping) >= 0) *reinterpret_cast<uint32*>(data + m(numMapping)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkaMeshBinding {
enum Members {
  basehkReferenceObject,
  boneFromSkinMeshTransforms,
  mappings,
  mesh,
  name,
  numBoneFromSkinMeshTransforms,
  numMappings,
  skeleton,
  skeletonName,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK550, 8, 0}}, 48}, {-1, 32, 16, 0, -1, 40, 24, 8, -1}, {0x2800, 0x0}},
  {{{{HK600, HK660, 8, 0}}, 56}, {-1, 40, 24, 0, -1, 48, 32, 16, 8}, {0x2800, 0x0}},
  {{{{HK700, HK2010_2, 8, 0}}, 72}, {0, 56, 40, 16, -1, 64, 48, 32, 24}, {0x2800, 0x0}},
  {{{{HK2011_1, HK2015, 8, 0}}, 80}, {0, 64, 48, 16, 32, 72, 56, 40, 24}, {0x2800, 0x0}},
  {{{{HK2016, HK2019, 8, 0}}, 88}, {0, 72, 56, 24, 40, 80, 64, 48, 32}, {0x2800, 0x0}},
  {{{{HK500, HK550, 4, 0}}, 24}, {-1, 16, 8, 0, -1, 20, 12, 4, -1}, {0x2800, 0x0}},
  {{{{HK600, HK660, 4, 0}}, 28}, {-1, 20, 12, 0, -1, 24, 16, 8, 4}, {0x2800, 0x0}},
  {{{{HK700, HK2010_2, 4, 0}}, 44}, {0, 32, 20, 8, -1, 36, 24, 16, 12}, {0x2800, 0x0}},
  {{{{HK2011_1, HK2015, 4, 0}}, 48}, {0, 36, 24, 8, 16, 40, 28, 20, 12}, {0x2800, 0x0}},
  {{{{HK2016, HK2019, 4, 0}}, 52}, {0, 40, 28, 12, 20, 44, 32, 24, 16}, {0x2800, 0x0}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<hkReferenceObject::Interface> MeshPtr() {
    int16 off = m(mesh); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkReferenceObject::Interface> Mesh() {
    int16 off = m(mesh); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkReferenceObject::Interface> Mesh() const {
    int16 off = m(mesh); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Pointer<hkReferenceObject::Interface> SkeletonPtr() {
    int16 off = m(skeleton); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkReferenceObject::Interface> Skeleton() {
    int16 off = m(skeleton); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkReferenceObject::Interface> Skeleton() const {
    int16 off = m(skeleton); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Pointer<hkaMeshBindingMapping::Interface> MappingsPtr() {
    int16 off = m(mappings); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkaMeshBindingMapping::Interface> Mappings() {
    int16 off = m(mappings); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkaMeshBindingMapping::Interface> Mappings() const {
    int16 off = m(mappings); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumMappings() const { return m(numMappings) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numMappings)); }
  Pointer<es::Matrix44> BoneFromSkinMeshTransformsPtr() {
    int16 off = m(boneFromSkinMeshTransforms); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  es::Matrix44 *BoneFromSkinMeshTransforms() {
    int16 off = m(boneFromSkinMeshTransforms); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<es::Matrix44**>(data + off);
    return *reinterpret_cast<es::PointerX86<es::Matrix44>*>(data + off);
  }
  const es::Matrix44 *BoneFromSkinMeshTransforms() const {
    int16 off = m(boneFromSkinMeshTransforms); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<es::Matrix44**>(data + off);
    return *reinterpret_cast<es::PointerX86<es::Matrix44>*>(data + off);
  }
  uint32 NumBoneFromSkinMeshTransforms() const { return m(numBoneFromSkinMeshTransforms) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numBoneFromSkinMeshTransforms)); }
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
  void NumMappings(uint32 value) { if (m(numMappings) >= 0) *reinterpret_cast<uint32*>(data + m(numMappings)) = value; }
  void NumBoneFromSkinMeshTransforms(uint32 value) { if (m(numBoneFromSkinMeshTransforms) >= 0) *reinterpret_cast<uint32*>(data + m(numBoneFromSkinMeshTransforms)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
