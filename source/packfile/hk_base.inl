// This file has been automatically generated. Do not modify.
#include "classgen/classgen.hpp"
namespace clgen::hkVariant {
enum Members {
  classDesc,
  object,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {HK500, HK2017, 8, 0, 16, {8, 0}, {0x0}},
  {HK500, HK2017, 4, 0, 8, {4, 0}, {0x0}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  uint16 LayoutVersion() const { return lookup.version; }
  char *Object() {
    int16 off = m(object); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *Object() const {
    int16 off = m(object); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  char *ClassDesc() {
    int16 off = m(classDesc); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *ClassDesc() const {
    int16 off = m(classDesc); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  

  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkReferenceObject {
enum Members {
  memSizeAndFlags,
  referenceCount,
  unk,
  vtable,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {HK500, HK2015, 8, 0, 16, {8, 10, -1, 0}, {0x5}},
  {HK2016, HK2017, 8, 0, 24, {16, 18, 8, 0}, {0x5}},
  {HK500, HK2015, 4, 0, 8, {4, 6, -1, 0}, {0x5}},
  {HK2016, HK2017, 4, 0, 12, {8, 10, 4, 0}, {0x5}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  uint16 LayoutVersion() const { return lookup.version; }
  int32 *Vtable() {
    int16 off = m(vtable); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int32**>(data + off);
    return *reinterpret_cast<es::PointerX86<int32>*>(data + off);
  }
  const int32 *Vtable() const {
    int16 off = m(vtable); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int32**>(data + off);
    return *reinterpret_cast<es::PointerX86<int32>*>(data + off);
  }
  int16 MemSizeAndFlags() const { return m(memSizeAndFlags) == -1 ? int16{} : *reinterpret_cast<int16*>(data + m(memSizeAndFlags)); }
  int16 ReferenceCount() const { return m(referenceCount) == -1 ? int16{} : *reinterpret_cast<int16*>(data + m(referenceCount)); }
  int32 *Unk() {
    int16 off = m(unk); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int32**>(data + off);
    return *reinterpret_cast<es::PointerX86<int32>*>(data + off);
  }
  const int32 *Unk() const {
    int16 off = m(unk); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int32**>(data + off);
    return *reinterpret_cast<es::PointerX86<int32>*>(data + off);
  }
  void MemSizeAndFlags(int16 value) { if (m(memSizeAndFlags) >= 0) *reinterpret_cast<int16*>(data + m(memSizeAndFlags)) = value; }
  void ReferenceCount(int16 value) { if (m(referenceCount) >= 0) *reinterpret_cast<int16*>(data + m(referenceCount)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
