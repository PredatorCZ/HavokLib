// This file has been automatically generated. Do not modify.
#include "hka_animation.inl"
namespace clgen::hkaInterleavedAnimation {
enum Members {
  basehkaAnimation,
  floats,
  numFloats,
  numTransforms,
  transforms,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK510, 8, 0}}, 72}, {0, -1, -1, 64, 56}, {0x80}},
  {{{{HK550, HK2015, 8, 0}}, 88}, {0, 72, 80, 64, 56}, {0xa0}},
  {{{{HK2016, HK2017, 8, 0}}, 96}, {0, 80, 88, 72, 64}, {0xa0}},
  {{{{HK500, HK510, 4, 0}}, 40}, {0, -1, -1, 36, 32}, {0x80}},
  {{{{HK550, HK660, 4, 0}}, 52}, {0, 44, 48, 40, 36}, {0xa0}},
  {{{{HK700, HK2015, 4, 0}}, 64}, {0, 52, 56, 44, 40}, {0xa0}},
  {{{{HK2016, HK2017, 4, 0}}, 68}, {0, 56, 60, 48, 44}, {0xa0}},
  {{{{HK500, HK510, 8, 1}}, 64}, {0, -1, -1, 56, 48}, {0x80}},
  {{{{HK550, HK2015, 8, 1}}, 88}, {0, 72, 80, 64, 56}, {0xa0}},
  {{{{HK2016, HK2017, 8, 1}}, 96}, {0, 80, 88, 72, 64}, {0xa0}},
  {{{{HK500, HK510, 4, 1}}, 40}, {0, -1, -1, 36, 32}, {0x80}},
  {{{{HK550, HK660, 4, 1}}, 52}, {0, 44, 48, 40, 36}, {0xa0}},
  {{{{HK700, HK2015, 4, 1}}, 64}, {0, 52, 56, 44, 40}, {0xa0}},
  {{{{HK2016, HK2017, 4, 1}}, 68}, {0, 56, 60, 48, 44}, {0xa0}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Padding ,LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  hkaAnimation::Interface BasehkaAnimation() const {
    int16 off = m(basehkaAnimation); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
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
  Pointer<float> FloatsPtr() {
    int16 off = m(floats); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  float *Floats() {
    int16 off = m(floats); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  const float *Floats() const {
    int16 off = m(floats); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  uint32 NumFloats() const { return m(numFloats) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numFloats)); }
  void NumTransforms(uint32 value) { if (m(numTransforms) >= 0) *reinterpret_cast<uint32*>(data + m(numTransforms)) = value; }
  void NumFloats(uint32 value) { if (m(numFloats) >= 0) *reinterpret_cast<uint32*>(data + m(numFloats)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
