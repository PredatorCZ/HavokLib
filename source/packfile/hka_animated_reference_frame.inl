// This file has been automatically generated. Do not modify.
#pragma once
#include "hk_base.inl"
namespace clgen::hkaAnimatedReferenceFrame {
enum Members {
  basehkReferenceObject,
  frameType,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2011_3, 8, 0}}, 16}, {0, -1}, {0x0}},
  {{{{HK2012_1, HK2015, 8, 0}}, 24}, {0, 16}, {0x0}},
  {{{{HK2016, HK2017, 8, 0}}, 32}, {0, 24}, {0x0}},
  {{{{HK500, HK2011_3, 4, 0}}, 8}, {0, -1}, {0x0}},
  {{{{HK2012_1, HK2015, 4, 0}}, 12}, {0, 8}, {0x0}},
  {{{{HK2016, HK2017, 4, 0}}, 16}, {0, 12}, {0x0}},
  {{{{HK500, HK2011_3, 8, 1}}, 16}, {0, -1}, {0x0}},
  {{{{HK2012_1, HK2015, 8, 1}}, 16}, {0, 12}, {0x0}},
  {{{{HK2016, HK2017, 8, 1}}, 24}, {0, 20}, {0x0}},
  {{{{HK500, HK2011_3, 4, 1}}, 8}, {0, -1}, {0x0}},
  {{{{HK2012_1, HK2015, 4, 1}}, 12}, {0, 8}, {0x0}},
  {{{{HK2016, HK2017, 4, 1}}, 16}, {0, 12}, {0x0}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr ,LookupFlag::Padding}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  hkaAnimatedReferenceFrameType FrameType() const { return m(frameType) == -1 ? hkaAnimatedReferenceFrameType{} : *reinterpret_cast<hkaAnimatedReferenceFrameType*>(data + m(frameType)); }
  void FrameType(hkaAnimatedReferenceFrameType value) { if (m(frameType) >= 0) *reinterpret_cast<hkaAnimatedReferenceFrameType*>(data + m(frameType)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
