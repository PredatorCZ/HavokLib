// This file has been automatically generated. Do not modify.
#include "hk_base.inl"
namespace clgen::hkaAnimationContainer {
enum Members {
  animations,
  attachments,
  basehkReferenceObject,
  bindings,
  numAnimations,
  numAttachments,
  numBindings,
  numSkeletons,
  numSkins,
  skeletons,
  skins,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 76}, {16, 48, -1, 32, 24, 56, 40, 8, 72, 0, 64}, {0xaa00, 0x2}},
  {{{{HK700, HK2015, 8, 0}}, 96}, {32, 64, 0, 48, 40, 72, 56, 24, 88, 16, 80}, {0xaa00, 0x2}},
  {{{{HK2016, HK2017, 8, 0}}, 104}, {40, 72, 0, 56, 48, 80, 64, 32, 96, 24, 88}, {0xaa00, 0x2}},
  {{{{HK500, HK660, 4, 0}}, 40}, {8, 24, -1, 16, 12, 28, 20, 4, 36, 0, 32}, {0xaa00, 0x2}},
  {{{{HK700, HK2015, 4, 0}}, 68}, {20, 44, 0, 32, 24, 48, 36, 12, 60, 8, 56}, {0xaa00, 0x2}},
  {{{{HK2016, HK2017, 4, 0}}, 72}, {24, 48, 0, 36, 28, 52, 40, 16, 64, 12, 60}, {0xaa00, 0x2}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  uint16 LayoutVersion() const { return lookup.version; }
  Iterator<Pointer<char>> Skeletons() {
    int16 off = m(skeletons); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<char>> Skeletons() const {
    int16 off = m(skeletons); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumSkeletons() const { return m(numSkeletons) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numSkeletons)); }
  Iterator<Pointer<char>> Animations() {
    int16 off = m(animations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<char>> Animations() const {
    int16 off = m(animations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumAnimations() const { return m(numAnimations) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numAnimations)); }
  Iterator<Pointer<char>> Bindings() {
    int16 off = m(bindings); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<char>> Bindings() const {
    int16 off = m(bindings); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumBindings() const { return m(numBindings) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numBindings)); }
  Iterator<Pointer<char>> Attachments() {
    int16 off = m(attachments); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<char>> Attachments() const {
    int16 off = m(attachments); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumAttachments() const { return m(numAttachments) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numAttachments)); }
  Iterator<Pointer<char>> Skins() {
    int16 off = m(skins); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<char>> Skins() const {
    int16 off = m(skins); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumSkins() const { return m(numSkins) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numSkins)); }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  void NumSkeletons(uint32 value) { if (m(numSkeletons) >= 0) *reinterpret_cast<uint32*>(data + m(numSkeletons)) = value; }
  void NumAnimations(uint32 value) { if (m(numAnimations) >= 0) *reinterpret_cast<uint32*>(data + m(numAnimations)) = value; }
  void NumBindings(uint32 value) { if (m(numBindings) >= 0) *reinterpret_cast<uint32*>(data + m(numBindings)) = value; }
  void NumAttachments(uint32 value) { if (m(numAttachments) >= 0) *reinterpret_cast<uint32*>(data + m(numAttachments)) = value; }
  void NumSkins(uint32 value) { if (m(numSkins) >= 0) *reinterpret_cast<uint32*>(data + m(numSkins)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
