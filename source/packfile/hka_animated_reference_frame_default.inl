// This file has been automatically generated. Do not modify.
#pragma once
#include "hka_animated_reference_frame.inl"
namespace clgen::hkaDefaultAnimatedReferenceFrame {
enum Members {
  basehkaAnimatedReferenceFrame,
  duration,
  forward,
  numReferenceFrameSamples,
  referenceFrameSamples,
  up,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2011_3, 8, 0}}, 80}, {0, 48, 32, 64, 56, 16}, {0x88}},
  {{{{HK2012_1, HK2019, 8, 0}}, 96}, {0, 64, 48, 80, 72, 32}, {0x88}},
  {{{{HK500, HK2019, 4, 0}}, 64}, {0, 48, 32, 56, 52, 16}, {0x88}},
  {{{{HK500, HK2015, 8, 1}}, 80}, {0, 48, 32, 64, 56, 16}, {0x88}},
  {{{{HK2016, HK2019, 8, 1}}, 96}, {0, 64, 48, 80, 72, 32}, {0x88}},
  {{{{HK500, HK2019, 4, 1}}, 64}, {0, 48, 32, 56, 52, 16}, {0x88}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr ,LookupFlag::Padding}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  hkaAnimatedReferenceFrame::Interface BasehkaAnimatedReferenceFrame() const {
    int16 off = m(basehkaAnimatedReferenceFrame); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Vector4A16 Up() const { return m(up) == -1 ? Vector4A16{} : *reinterpret_cast<Vector4A16*>(data + m(up)); }
  Vector4A16 Forward() const { return m(forward) == -1 ? Vector4A16{} : *reinterpret_cast<Vector4A16*>(data + m(forward)); }
  float Duration() const { return m(duration) == -1 ? float{} : *reinterpret_cast<float*>(data + m(duration)); }
  Pointer<Vector4A16> ReferenceFrameSamplesPtr() {
    int16 off = m(referenceFrameSamples); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Vector4A16 *ReferenceFrameSamples() {
    int16 off = m(referenceFrameSamples); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<Vector4A16**>(data + off);
    return *reinterpret_cast<es::PointerX86<Vector4A16>*>(data + off);
  }
  const Vector4A16 *ReferenceFrameSamples() const {
    int16 off = m(referenceFrameSamples); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<Vector4A16**>(data + off);
    return *reinterpret_cast<es::PointerX86<Vector4A16>*>(data + off);
  }
  uint32 NumReferenceFrameSamples() const { return m(numReferenceFrameSamples) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numReferenceFrameSamples)); }
  void Up(Vector4A16 value) { if (m(up) >= 0) *reinterpret_cast<Vector4A16*>(data + m(up)) = value; }
  void Forward(Vector4A16 value) { if (m(forward) >= 0) *reinterpret_cast<Vector4A16*>(data + m(forward)) = value; }
  void Duration(float value) { if (m(duration) >= 0) *reinterpret_cast<float*>(data + m(duration)) = value; }
  void NumReferenceFrameSamples(uint32 value) { if (m(numReferenceFrameSamples) >= 0) *reinterpret_cast<uint32*>(data + m(numReferenceFrameSamples)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
