// This file has been automatically generated. Do not modify.
#pragma once
#include "hka_animated_reference_frame.inl"
namespace clgen::hkaAnnotation {
enum Members {
  text,
  time,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2019, 8, 0}}, 16}, {8, 0}, {0x8}},
  {{{{HK500, HK2019, 4, 0}}, 8}, {4, 0}, {0x8}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  float Time() const { return m(time) == -1 ? float{} : *reinterpret_cast<float*>(data + m(time)); }
  Pointer<char> TextPtr() {
    int16 off = m(text); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *Text() {
    int16 off = m(text); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *Text() const {
    int16 off = m(text); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  void Time(float value) { if (m(time) >= 0) *reinterpret_cast<float*>(data + m(time)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkaAnnotationTrack {
enum Members {
  annotations,
  name,
  numAnnotations,
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
  Pointer<hkaAnnotation::Interface> AnnotationsPtr() {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkaAnnotation::Interface> Annotations() {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkaAnnotation::Interface> Annotations() const {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumAnnotations() const { return m(numAnnotations) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numAnnotations)); }
  void NumAnnotations(uint32 value) { if (m(numAnnotations) >= 0) *reinterpret_cast<uint32*>(data + m(numAnnotations)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkaAnimation {
enum Members {
  animationType,
  annotations,
  basehkReferenceObject,
  duration,
  extractedMotion,
  numAnnotations,
  numOfFloatTracks,
  numOfTransformTracks,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK510, 8, 0}}, 56}, {16, 40, 0, 20, 32, 48, -1, 24}, {0x8882}},
  {{{{HK550, HK2015, 8, 0}}, 56}, {16, 40, 0, 20, 32, 48, 28, 24}, {0xa882}},
  {{{{HK2016, HK2019, 8, 0}}, 64}, {24, 48, 0, 28, 40, 56, 36, 32}, {0xa882}},
  {{{{HK500, HK510, 4, 0}}, 32}, {8, 24, 0, 12, 20, 28, -1, 16}, {0x8882}},
  {{{{HK550, HK660, 4, 0}}, 36}, {8, 28, 0, 12, 24, 32, 20, 16}, {0xa882}},
  {{{{HK700, HK2015, 4, 0}}, 40}, {8, 28, 0, 12, 24, 32, 20, 16}, {0xa882}},
  {{{{HK2016, HK2019, 4, 0}}, 44}, {12, 32, 0, 16, 28, 36, 24, 20}, {0xa882}},
  {{{{HK500, HK510, 8, 1}}, 48}, {12, 32, 0, 16, 24, 40, -1, 20}, {0x8882}},
  {{{{HK550, HK2015, 8, 1}}, 56}, {12, 40, 0, 16, 32, 48, 24, 20}, {0xa882}},
  {{{{HK2016, HK2019, 8, 1}}, 64}, {20, 48, 0, 24, 40, 56, 32, 28}, {0xa882}},
  {{{{HK500, HK510, 4, 1}}, 32}, {8, 24, 0, 12, 20, 28, -1, 16}, {0x8882}},
  {{{{HK550, HK660, 4, 1}}, 36}, {8, 28, 0, 12, 24, 32, 20, 16}, {0xa882}},
  {{{{HK700, HK2015, 4, 1}}, 40}, {8, 28, 0, 12, 24, 32, 20, 16}, {0xa882}},
  {{{{HK2016, HK2019, 4, 1}}, 44}, {12, 32, 0, 16, 28, 36, 24, 20}, {0xa882}}
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
  uint32 AnimationType() const { return m(animationType) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(animationType)); }
  float Duration() const { return m(duration) == -1 ? float{} : *reinterpret_cast<float*>(data + m(duration)); }
  uint32 NumOfTransformTracks() const { return m(numOfTransformTracks) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numOfTransformTracks)); }
  Pointer<hkaAnimatedReferenceFrame::Interface> ExtractedMotionPtr() {
    int16 off = m(extractedMotion); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkaAnimatedReferenceFrame::Interface> ExtractedMotion() {
    int16 off = m(extractedMotion); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkaAnimatedReferenceFrame::Interface> ExtractedMotion() const {
    int16 off = m(extractedMotion); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Pointer<Pointer<hkaAnnotationTrack::Interface>> AnnotationsPtr() {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkaAnnotationTrack::Interface>> Annotations() {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkaAnnotationTrack::Interface>> Annotations() const {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumAnnotations() const { return m(numAnnotations) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numAnnotations)); }
  Pointer<hkaAnnotationTrack::Interface> AnnotationsPtrHK700() {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkaAnnotationTrack::Interface> AnnotationsHK700() {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkaAnnotationTrack::Interface> AnnotationsHK700() const {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumOfFloatTracks() const { return m(numOfFloatTracks) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numOfFloatTracks)); }
  void AnimationType(uint32 value) { if (m(animationType) >= 0) *reinterpret_cast<uint32*>(data + m(animationType)) = value; }
  void Duration(float value) { if (m(duration) >= 0) *reinterpret_cast<float*>(data + m(duration)) = value; }
  void NumOfTransformTracks(uint32 value) { if (m(numOfTransformTracks) >= 0) *reinterpret_cast<uint32*>(data + m(numOfTransformTracks)) = value; }
  void NumAnnotations(uint32 value) { if (m(numAnnotations) >= 0) *reinterpret_cast<uint32*>(data + m(numAnnotations)) = value; }
  void NumOfFloatTracks(uint32 value) { if (m(numOfFloatTracks) >= 0) *reinterpret_cast<uint32*>(data + m(numOfFloatTracks)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
