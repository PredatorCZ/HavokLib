// This file has been automatically generated. Do not modify.
#include "hk_base.inl"
namespace clgen::hkNamedVariant {
enum Members {
  className,
  name,
  variant,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 32}, {8, 0, 16}, {0x0}},
  {{{{HK700, HK2017, 8, 0}}, 24}, {8, 0, 16}, {0x0}},
  {{{{HK500, HK660, 4, 0}}, 16}, {4, 0, 8}, {0x0}},
  {{{{HK700, HK2017, 4, 0}}, 12}, {4, 0, 8}, {0x0}}
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
  Pointer<char> ClassNamePtr() {
    int16 off = m(className); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *ClassName() {
    int16 off = m(className); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *ClassName() const {
    int16 off = m(className); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  hkVariant::Interface Variant() const { return {m(variant) == -1 ? nullptr : data + m(variant), lookup}; }
  Pointer<char> VariantPtr() {
    int16 off = m(variant); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *VariantHK700() {
    int16 off = m(variant); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *VariantHK700() const {
    int16 off = m(variant); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  

  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkRootLevelContainer {
enum Members {
  numVariants,
  variants,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 12}, {8, 0}, {0x2}},
  {{{{HK700, HK2017, 8, 0}}, 16}, {8, 0}, {0x2}},
  {{{{HK500, HK660, 4, 0}}, 8}, {4, 0}, {0x2}},
  {{{{HK700, HK2017, 4, 0}}, 12}, {4, 0}, {0x2}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<hkNamedVariant::Interface> VariantsPtr() {
    int16 off = m(variants); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkNamedVariant::Interface> Variants() {
    int16 off = m(variants); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkNamedVariant::Interface> Variants() const {
    int16 off = m(variants); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumVariants() const { return m(numVariants) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numVariants)); }
  void NumVariants(uint32 value) { if (m(numVariants) >= 0) *reinterpret_cast<uint32*>(data + m(numVariants)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
