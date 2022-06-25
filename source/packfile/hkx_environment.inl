// This file has been automatically generated. Do not modify.
#include "hk_base.inl"
namespace clgen::hkxEnvironmentVariable {
enum Members {
  name,
  value,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2017, 8, 0}}, 16}, {0, 8}, {0x0}},
  {{{{HK500, HK2017, 4, 0}}, 8}, {0, 4}, {0x0}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  uint16 LayoutVersion() const { return lookup.version; }
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
  char *Value() {
    int16 off = m(value); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *Value() const {
    int16 off = m(value); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  

  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxEnvironment {
enum Members {
  basehkReferenceObject,
  numVariables,
  variables,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 16}, {-1, 8, 0}, {0x8}},
  {{{{HK700, HK2015, 8, 0}}, 32}, {0, 24, 16}, {0x8}},
  {{{{HK2016, HK2017, 8, 0}}, 40}, {0, 32, 24}, {0x8}},
  {{{{HK500, HK660, 4, 0}}, 12}, {-1, 4, 0}, {0x8}},
  {{{{HK700, HK2015, 4, 0}}, 20}, {0, 12, 8}, {0x8}},
  {{{{HK2016, HK2017, 4, 0}}, 24}, {0, 16, 12}, {0x8}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  uint16 LayoutVersion() const { return lookup.version; }
  Iterator<hkxEnvironmentVariable::Interface> Variables() {
    int16 off = m(variables); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxEnvironmentVariable::Interface> Variables() const {
    int16 off = m(variables); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumVariables() const { return m(numVariables) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numVariables)); }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  void NumVariables(uint32 value) { if (m(numVariables) >= 0) *reinterpret_cast<uint32*>(data + m(numVariables)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
