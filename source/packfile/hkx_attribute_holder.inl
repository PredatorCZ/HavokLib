// This file has been automatically generated. Do not modify.
#pragma once
#include "hk_base.inl"
namespace clgen::hkxAttribute {
enum Members {
  name,
  value,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 24}, {0, 8}, {0x0}},
  {{{{HK700, HK2019, 8, 0}}, 16}, {0, 8}, {0x0}},
  {{{{HK500, HK660, 4, 0}}, 12}, {0, 4}, {0x0}},
  {{{{HK700, HK2019, 4, 0}}, 8}, {0, 4}, {0x0}}
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
  hkVariant::Interface Value() const { return {m(value) == -1 ? nullptr : data + m(value), lookup}; }
  Pointer<hkReferenceObject::Interface> ValuePtr() {
    int16 off = m(value); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkReferenceObject::Interface> ValueHK700() {
    int16 off = m(value); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkReferenceObject::Interface> ValueHK700() const {
    int16 off = m(value); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  

  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxAttributeGroup {
enum Members {
  attributes,
  name,
  numAttributes,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2019, 8, 0}}, 24}, {8, 0, 16}, {0x20}},
  {{{{HK500, HK660, 4, 0}}, 12}, {4, 0, 8}, {0x20}},
  {{{{HK700, HK2019, 4, 0}}, 16}, {4, 0, 8}, {0x20}}
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
  Pointer<hkxAttribute::Interface> AttributesPtr() {
    int16 off = m(attributes); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkxAttribute::Interface> Attributes() {
    int16 off = m(attributes); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxAttribute::Interface> Attributes() const {
    int16 off = m(attributes); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumAttributes() const { return m(numAttributes) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numAttributes)); }
  void NumAttributes(uint32 value) { if (m(numAttributes) >= 0) *reinterpret_cast<uint32*>(data + m(numAttributes)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxAttributeHolder {
enum Members {
  attributeGroups,
  basehkReferenceObject,
  numAttributeGroups,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 16}, {0, -1, 8}, {0x20}},
  {{{{HK700, HK2015, 8, 0}}, 32}, {16, 0, 24}, {0x20}},
  {{{{HK2016, HK2019, 8, 0}}, 40}, {24, 0, 32}, {0x20}},
  {{{{HK500, HK660, 4, 0}}, 8}, {0, -1, 4}, {0x20}},
  {{{{HK700, HK2015, 4, 0}}, 20}, {8, 0, 12}, {0x20}},
  {{{{HK2016, HK2019, 4, 0}}, 24}, {12, 0, 16}, {0x20}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<hkxAttributeGroup::Interface> AttributeGroupsPtr() {
    int16 off = m(attributeGroups); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkxAttributeGroup::Interface> AttributeGroups() {
    int16 off = m(attributeGroups); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxAttributeGroup::Interface> AttributeGroups() const {
    int16 off = m(attributeGroups); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumAttributeGroups() const { return m(numAttributeGroups) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numAttributeGroups)); }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  void NumAttributeGroups(uint32 value) { if (m(numAttributeGroups) >= 0) *reinterpret_cast<uint32*>(data + m(numAttributeGroups)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
