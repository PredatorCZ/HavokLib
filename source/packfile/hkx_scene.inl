// This file has been automatically generated. Do not modify.
#include "hk_base.inl"
namespace clgen::hkxSpline {
enum Members {
  basehkReferenceObject,
  controlPoints,
  isClosed,
  numControlPoints,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2015, 8, 0}}, 40}, {0, 16, 32, 24}, {0x80}},
  {{{{HK2016, HK2019, 8, 0}}, 48}, {0, 24, 40, 32}, {0x80}},
  {{{{HK500, HK2015, 4, 0}}, 24}, {0, 8, 20, 12}, {0x80}},
  {{{{HK2016, HK2019, 4, 0}}, 28}, {0, 12, 24, 16}, {0x80}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Pointer<::hkxSplineControlPoint> ControlPointsPtr() {
    int16 off = m(controlPoints); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  ::hkxSplineControlPoint *ControlPoints() {
    int16 off = m(controlPoints); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<::hkxSplineControlPoint**>(data + off);
    return *reinterpret_cast<es::PointerX86<::hkxSplineControlPoint>*>(data + off);
  }
  const ::hkxSplineControlPoint *ControlPoints() const {
    int16 off = m(controlPoints); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<::hkxSplineControlPoint**>(data + off);
    return *reinterpret_cast<es::PointerX86<::hkxSplineControlPoint>*>(data + off);
  }
  uint32 NumControlPoints() const { return m(numControlPoints) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numControlPoints)); }
  bool IsClosed() const { return m(isClosed) == -1 ? bool{} : *reinterpret_cast<bool*>(data + m(isClosed)); }
  void NumControlPoints(uint32 value) { if (m(numControlPoints) >= 0) *reinterpret_cast<uint32*>(data + m(numControlPoints)) = value; }
  void IsClosed(bool value) { if (m(isClosed) >= 0) *reinterpret_cast<bool*>(data + m(isClosed)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxNodeAnnotationData {
enum Members {
  description,
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
  Pointer<char> DescriptionPtr() {
    int16 off = m(description); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *Description() {
    int16 off = m(description); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *Description() const {
    int16 off = m(description); if (off == -1) return nullptr;
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
namespace clgen::hkxNode {
enum Members {
  annotations,
  basehkxAttributeHolder,
  bone,
  children,
  keyFrames,
  linearKeyFrameHints,
  name,
  numAnnotations,
  numBone,
  numChildren,
  numKeyFrames,
  numLinearKeyFrameHints,
  object,
  selected,
  userProperties,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 104}, {72, 0, -1, 56, 40, -1, 16, 80, -1, 64, 48, -1, 24, 96, 88}, {0x8000, 0x28}},
  {{{{HK700, HK2011_3, 8, 0}}, 112}, {80, 0, -1, 64, 48, -1, 32, 88, -1, 72, 56, -1, 40, 104, 96}, {0x8000, 0x28}},
  {{{{HK2012_1, HK2015, 8, 0}}, 144}, {80, 0, 128, 64, 48, 96, 32, 88, 136, 72, 56, 104, 40, 120, 112}, {0x8000, 0xaa}},
  {{{{HK2016, HK2019, 8, 0}}, 152}, {88, 0, 136, 72, 56, 104, 40, 96, 144, 80, 64, 112, 48, 128, 120}, {0x8000, 0xaa}},
  {{{{HK500, HK660, 4, 0}}, 52}, {36, 0, -1, 28, 20, -1, 8, 40, -1, 32, 24, -1, 12, 48, 44}, {0x8000, 0x28}},
  {{{{HK700, HK2011_3, 4, 0}}, 72}, {52, 0, -1, 40, 28, -1, 20, 56, -1, 44, 32, -1, 24, 68, 64}, {0x8000, 0x28}},
  {{{{HK2012_1, HK2015, 4, 0}}, 96}, {52, 0, 84, 40, 28, 64, 20, 56, 88, 44, 32, 68, 24, 80, 76}, {0x8000, 0xaa}},
  {{{{HK2016, HK2019, 4, 0}}, 100}, {56, 0, 88, 44, 32, 68, 24, 60, 92, 48, 36, 72, 28, 84, 80}, {0x8000, 0xaa}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  hkxAttributeHolder::Interface BasehkxAttributeHolder() const {
    int16 off = m(basehkxAttributeHolder); if (off == -1) return {nullptr, lookup};
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
  hkVariant::Interface Object() const { return {m(object) == -1 ? nullptr : data + m(object), lookup}; }
  Pointer<hkReferenceObject::Interface> ObjectPtr() {
    int16 off = m(object); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkReferenceObject::Interface> ObjectHK700() {
    int16 off = m(object); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkReferenceObject::Interface> ObjectHK700() const {
    int16 off = m(object); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Pointer<es::Matrix44> KeyFramesPtr() {
    int16 off = m(keyFrames); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  es::Matrix44 *KeyFrames() {
    int16 off = m(keyFrames); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<es::Matrix44**>(data + off);
    return *reinterpret_cast<es::PointerX86<es::Matrix44>*>(data + off);
  }
  const es::Matrix44 *KeyFrames() const {
    int16 off = m(keyFrames); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<es::Matrix44**>(data + off);
    return *reinterpret_cast<es::PointerX86<es::Matrix44>*>(data + off);
  }
  uint32 NumKeyFrames() const { return m(numKeyFrames) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numKeyFrames)); }
  Pointer<Pointer<hkxNode::Interface>> ChildrenPtr() {
    int16 off = m(children); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxNode::Interface>> Children() {
    int16 off = m(children); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxNode::Interface>> Children() const {
    int16 off = m(children); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumChildren() const { return m(numChildren) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numChildren)); }
  Pointer<hkxNodeAnnotationData::Interface> AnnotationsPtr() {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkxNodeAnnotationData::Interface> Annotations() {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxNodeAnnotationData::Interface> Annotations() const {
    int16 off = m(annotations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumAnnotations() const { return m(numAnnotations) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numAnnotations)); }
  Pointer<char> UserPropertiesPtr() {
    int16 off = m(userProperties); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *UserProperties() {
    int16 off = m(userProperties); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *UserProperties() const {
    int16 off = m(userProperties); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  bool Selected() const { return m(selected) == -1 ? bool{} : *reinterpret_cast<bool*>(data + m(selected)); }
  Pointer<float> LinearKeyFrameHintsPtr() {
    int16 off = m(linearKeyFrameHints); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  float *LinearKeyFrameHints() {
    int16 off = m(linearKeyFrameHints); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  const float *LinearKeyFrameHints() const {
    int16 off = m(linearKeyFrameHints); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  uint32 NumLinearKeyFrameHints() const { return m(numLinearKeyFrameHints) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numLinearKeyFrameHints)); }
  Pointer<bool> BonePtr() {
    int16 off = m(bone); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  bool *Bone() {
    int16 off = m(bone); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<bool**>(data + off);
    return *reinterpret_cast<es::PointerX86<bool>*>(data + off);
  }
  const bool *Bone() const {
    int16 off = m(bone); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<bool**>(data + off);
    return *reinterpret_cast<es::PointerX86<bool>*>(data + off);
  }
  uint32 NumBone() const { return m(numBone) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numBone)); }
  void NumKeyFrames(uint32 value) { if (m(numKeyFrames) >= 0) *reinterpret_cast<uint32*>(data + m(numKeyFrames)) = value; }
  void NumChildren(uint32 value) { if (m(numChildren) >= 0) *reinterpret_cast<uint32*>(data + m(numChildren)) = value; }
  void NumAnnotations(uint32 value) { if (m(numAnnotations) >= 0) *reinterpret_cast<uint32*>(data + m(numAnnotations)) = value; }
  void Selected(bool value) { if (m(selected) >= 0) *reinterpret_cast<bool*>(data + m(selected)) = value; }
  void NumLinearKeyFrameHints(uint32 value) { if (m(numLinearKeyFrameHints) >= 0) *reinterpret_cast<uint32*>(data + m(numLinearKeyFrameHints)) = value; }
  void NumBone(uint32 value) { if (m(numBone) >= 0) *reinterpret_cast<uint32*>(data + m(numBone)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxNodeSelectionSet {
enum Members {
  basehkxAttributeHolder,
  name,
  numSelectedNodes,
  selectedNodes,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 40}, {0, 32, 24, 16}, {0x20}},
  {{{{HK700, HK2015, 8, 0}}, 56}, {0, 48, 40, 32}, {0x20}},
  {{{{HK2016, HK2019, 8, 0}}, 64}, {0, 56, 48, 40}, {0x20}},
  {{{{HK500, HK660, 4, 0}}, 20}, {0, 16, 12, 8}, {0x20}},
  {{{{HK700, HK2015, 4, 0}}, 36}, {0, 32, 24, 20}, {0x20}},
  {{{{HK2016, HK2019, 4, 0}}, 40}, {0, 36, 28, 24}, {0x20}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  hkxAttributeHolder::Interface BasehkxAttributeHolder() const {
    int16 off = m(basehkxAttributeHolder); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Pointer<Pointer<hkxNode::Interface>> SelectedNodesPtr() {
    int16 off = m(selectedNodes); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxNode::Interface>> SelectedNodes() {
    int16 off = m(selectedNodes); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxNode::Interface>> SelectedNodes() const {
    int16 off = m(selectedNodes); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumSelectedNodes() const { return m(numSelectedNodes) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numSelectedNodes)); }
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
  void NumSelectedNodes(uint32 value) { if (m(numSelectedNodes) >= 0) *reinterpret_cast<uint32*>(data + m(numSelectedNodes)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxCamera {
enum Members {
  basehkReferenceObject,
  data_,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 64}, {-1, 0}, {0x0}},
  {{{{HK700, HK2015, 8, 0}}, 80}, {0, 16}, {0x0}},
  {{{{HK2016, HK2019, 8, 0}}, 96}, {0, 32}, {0x0}},
  {{{{HK500, HK660, 4, 0}}, 64}, {-1, 0}, {0x0}},
  {{{{HK700, HK2019, 4, 0}}, 80}, {0, 16}, {0x0}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  std::span<::hkxCamera> Data_() const { return m(data_) == -1 ? std::span<::hkxCamera>{} : std::span<::hkxCamera>{reinterpret_cast<::hkxCamera*>(data + m(data_)), 1}; }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  

  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxLight {
enum Members {
  angle,
  basehkReferenceObject,
  color,
  direction,
  fadeEnd,
  fadeStart,
  intensity,
  position,
  range,
  shadowCaster,
  type,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 64}, {52, -1, 48, 32, -1, -1, -1, 16, -1, -1, 0}, {0x22, 0x0}},
  {{{{HK700, HK2011_3, 8, 0}}, 80}, {68, 0, 64, 48, -1, -1, -1, 32, -1, -1, 16}, {0x22, 0x0}},
  {{{{HK2012_1, HK2015, 8, 0}}, 96}, {68, 0, 64, 48, 80, 76, 84, 32, 72, 88, 16}, {0x2a22, 0x2}},
  {{{{HK2016, HK2019, 8, 0}}, 96}, {68, 0, 64, 48, 80, 76, 84, 32, 72, 88, 24}, {0x2a22, 0x2}},
  {{{{HK500, HK660, 4, 0}}, 64}, {52, -1, 48, 32, -1, -1, -1, 16, -1, -1, 0}, {0x22, 0x0}},
  {{{{HK700, HK2011_3, 4, 0}}, 64}, {52, 0, 48, 32, -1, -1, -1, 16, -1, -1, 8}, {0x22, 0x0}},
  {{{{HK2012_1, HK2015, 4, 0}}, 80}, {52, 0, 48, 32, 64, 60, 68, 16, 56, 72, 8}, {0x2a22, 0x2}},
  {{{{HK2016, HK2019, 4, 0}}, 80}, {52, 0, 48, 32, 64, 60, 68, 16, 56, 72, 12}, {0x2a22, 0x2}},
  {{{{HK500, HK660, 8, 1}}, 64}, {52, -1, 48, 32, -1, -1, -1, 16, -1, -1, 0}, {0x22, 0x0}},
  {{{{HK700, HK2011_3, 8, 1}}, 64}, {52, 0, 48, 32, -1, -1, -1, 16, -1, -1, 12}, {0x22, 0x0}},
  {{{{HK2012_1, HK2015, 8, 1}}, 80}, {52, 0, 48, 32, 64, 60, 68, 16, 56, 72, 12}, {0x2a22, 0x2}},
  {{{{HK2016, HK2019, 8, 1}}, 96}, {68, 0, 64, 48, 80, 76, 84, 32, 72, 88, 20}, {0x2a22, 0x2}},
  {{{{HK500, HK660, 4, 1}}, 64}, {52, -1, 48, 32, -1, -1, -1, 16, -1, -1, 0}, {0x22, 0x0}},
  {{{{HK700, HK2011_3, 4, 1}}, 64}, {52, 0, 48, 32, -1, -1, -1, 16, -1, -1, 8}, {0x22, 0x0}},
  {{{{HK2012_1, HK2015, 4, 1}}, 80}, {52, 0, 48, 32, 64, 60, 68, 16, 56, 72, 8}, {0x2a22, 0x2}},
  {{{{HK2016, HK2019, 4, 1}}, 80}, {52, 0, 48, 32, 64, 60, 68, 16, 56, 72, 12}, {0x2a22, 0x2}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr ,LookupFlag::Padding}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  ::LightType Type() const { return m(type) == -1 ? ::LightType{} : *reinterpret_cast<::LightType*>(data + m(type)); }
  Vector4A16 Position() const { return m(position) == -1 ? Vector4A16{} : *reinterpret_cast<Vector4A16*>(data + m(position)); }
  Vector4A16 Direction() const { return m(direction) == -1 ? Vector4A16{} : *reinterpret_cast<Vector4A16*>(data + m(direction)); }
  uint32 Color() const { return m(color) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(color)); }
  float Angle() const { return m(angle) == -1 ? float{} : *reinterpret_cast<float*>(data + m(angle)); }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  float Range() const { return m(range) == -1 ? float{} : *reinterpret_cast<float*>(data + m(range)); }
  float FadeStart() const { return m(fadeStart) == -1 ? float{} : *reinterpret_cast<float*>(data + m(fadeStart)); }
  float FadeEnd() const { return m(fadeEnd) == -1 ? float{} : *reinterpret_cast<float*>(data + m(fadeEnd)); }
  float Intensity() const { return m(intensity) == -1 ? float{} : *reinterpret_cast<float*>(data + m(intensity)); }
  bool ShadowCaster() const { return m(shadowCaster) == -1 ? bool{} : *reinterpret_cast<bool*>(data + m(shadowCaster)); }
  void Type(::LightType value) { if (m(type) >= 0) *reinterpret_cast<::LightType*>(data + m(type)) = value; }
  void Position(Vector4A16 value) { if (m(position) >= 0) *reinterpret_cast<Vector4A16*>(data + m(position)) = value; }
  void Direction(Vector4A16 value) { if (m(direction) >= 0) *reinterpret_cast<Vector4A16*>(data + m(direction)) = value; }
  void Color(uint32 value) { if (m(color) >= 0) *reinterpret_cast<uint32*>(data + m(color)) = value; }
  void Angle(float value) { if (m(angle) >= 0) *reinterpret_cast<float*>(data + m(angle)) = value; }
  void Range(float value) { if (m(range) >= 0) *reinterpret_cast<float*>(data + m(range)) = value; }
  void FadeStart(float value) { if (m(fadeStart) >= 0) *reinterpret_cast<float*>(data + m(fadeStart)) = value; }
  void FadeEnd(float value) { if (m(fadeEnd) >= 0) *reinterpret_cast<float*>(data + m(fadeEnd)) = value; }
  void Intensity(float value) { if (m(intensity) >= 0) *reinterpret_cast<float*>(data + m(intensity)) = value; }
  void ShadowCaster(bool value) { if (m(shadowCaster) >= 0) *reinterpret_cast<bool*>(data + m(shadowCaster)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxTextureInplace {
enum Members {
  basehkReferenceObject,
  data_,
  fileType,
  name,
  numData_,
  originalFilename,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 40}, {-1, 8, 0, 24, 16, 32}, {0x200}},
  {{{{HK700, HK2015, 8, 0}}, 56}, {0, 24, 16, 40, 32, 48}, {0x200}},
  {{{{HK2016, HK2019, 8, 0}}, 64}, {0, 32, 24, 48, 40, 56}, {0x200}},
  {{{{HK500, HK660, 4, 0}}, 20}, {-1, 4, 0, 12, 8, 16}, {0x200}},
  {{{{HK700, HK2015, 4, 0}}, 32}, {0, 12, 8, 24, 16, 28}, {0x200}},
  {{{{HK2016, HK2019, 4, 0}}, 36}, {0, 16, 12, 28, 20, 32}, {0x200}},
  {{{{HK500, HK660, 8, 1}}, 40}, {-1, 8, 0, 24, 16, 32}, {0x200}},
  {{{{HK700, HK2015, 8, 1}}, 48}, {0, 16, 12, 32, 24, 40}, {0x200}},
  {{{{HK2016, HK2019, 8, 1}}, 56}, {0, 24, 20, 40, 32, 48}, {0x200}},
  {{{{HK500, HK660, 4, 1}}, 20}, {-1, 4, 0, 12, 8, 16}, {0x200}},
  {{{{HK700, HK2015, 4, 1}}, 32}, {0, 12, 8, 24, 16, 28}, {0x200}},
  {{{{HK2016, HK2019, 4, 1}}, 36}, {0, 16, 12, 28, 20, 32}, {0x200}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr ,LookupFlag::Padding}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  std::span<char> FileType() const { return m(fileType) == -1 ? std::span<char>{} : std::span<char>{reinterpret_cast<char*>(data + m(fileType)), 4}; }
  Pointer<uint8> Data_Ptr() {
    int16 off = m(data_); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint8 *Data_() {
    int16 off = m(data_); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint8**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint8>*>(data + off);
  }
  const uint8 *Data_() const {
    int16 off = m(data_); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint8**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint8>*>(data + off);
  }
  uint32 NumData_() const { return m(numData_) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numData_)); }
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
  Pointer<char> OriginalFilenamePtr() {
    int16 off = m(originalFilename); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *OriginalFilename() {
    int16 off = m(originalFilename); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *OriginalFilename() const {
    int16 off = m(originalFilename); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  void NumData_(uint32 value) { if (m(numData_) >= 0) *reinterpret_cast<uint32*>(data + m(numData_)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxTextureFile {
enum Members {
  basehkReferenceObject,
  fileName,
  name,
  originalFilename,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 24}, {-1, 0, 8, 16}, {0x0}},
  {{{{HK700, HK2015, 8, 0}}, 40}, {0, 16, 24, 32}, {0x0}},
  {{{{HK2016, HK2019, 8, 0}}, 48}, {0, 24, 32, 40}, {0x0}},
  {{{{HK500, HK660, 4, 0}}, 12}, {-1, 0, 4, 8}, {0x0}},
  {{{{HK700, HK2015, 4, 0}}, 20}, {0, 8, 12, 16}, {0x0}},
  {{{{HK2016, HK2019, 4, 0}}, 24}, {0, 12, 16, 20}, {0x0}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<char> FileNamePtr() {
    int16 off = m(fileName); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *FileName() {
    int16 off = m(fileName); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *FileName() const {
    int16 off = m(fileName); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
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
  Pointer<char> OriginalFilenamePtr() {
    int16 off = m(originalFilename); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *OriginalFilename() {
    int16 off = m(originalFilename); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *OriginalFilename() const {
    int16 off = m(originalFilename); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  

  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxSkinBinding {
enum Members {
  basehkReferenceObject,
  bindPose,
  initSkinTransform,
  mapping,
  mesh,
  nodeNames,
  numBindPose,
  numMapping,
  numNodeNames,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 112}, {-1, 24, 48, 8, 0, -1, 32, 16, -1}, {0xa000, 0x0}},
  {{{{HK700, HK700, 8, 0}}, 128}, {0, 40, 64, 24, 16, -1, 48, 32, -1}, {0xa000, 0x0}},
  {{{{HK710, HK2015, 8, 0}}, 128}, {0, 40, 64, -1, 16, 24, 48, -1, 32}, {0x2000, 0x2}},
  {{{{HK2016, HK2019, 8, 0}}, 128}, {0, 48, 64, -1, 24, 32, 56, -1, 40}, {0x2000, 0x2}},
  {{{{HK500, HK660, 4, 0}}, 96}, {-1, 12, 32, 4, 0, -1, 16, 8, -1}, {0xa000, 0x0}},
  {{{{HK700, HK700, 4, 0}}, 112}, {0, 24, 48, 12, 8, -1, 28, 16, -1}, {0xa000, 0x0}},
  {{{{HK710, HK2015, 4, 0}}, 112}, {0, 24, 48, -1, 8, 12, 28, -1, 16}, {0x2000, 0x2}},
  {{{{HK2016, HK2019, 4, 0}}, 112}, {0, 28, 48, -1, 12, 16, 32, -1, 20}, {0x2000, 0x2}}
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
  Pointer<Pointer<hkxNode::Interface>> MappingPtr() {
    int16 off = m(mapping); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxNode::Interface>> Mapping() {
    int16 off = m(mapping); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxNode::Interface>> Mapping() const {
    int16 off = m(mapping); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumMapping() const { return m(numMapping) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numMapping)); }
  Pointer<es::Matrix44> BindPosePtr() {
    int16 off = m(bindPose); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  es::Matrix44 *BindPose() {
    int16 off = m(bindPose); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<es::Matrix44**>(data + off);
    return *reinterpret_cast<es::PointerX86<es::Matrix44>*>(data + off);
  }
  const es::Matrix44 *BindPose() const {
    int16 off = m(bindPose); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<es::Matrix44**>(data + off);
    return *reinterpret_cast<es::PointerX86<es::Matrix44>*>(data + off);
  }
  uint32 NumBindPose() const { return m(numBindPose) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numBindPose)); }
  es::Matrix44 InitSkinTransform() const { return m(initSkinTransform) == -1 ? es::Matrix44{} : *reinterpret_cast<es::Matrix44*>(data + m(initSkinTransform)); }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Pointer<Pointer<char>> NodeNamesPtr() {
    int16 off = m(nodeNames); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<char>> NodeNames() {
    int16 off = m(nodeNames); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<char>> NodeNames() const {
    int16 off = m(nodeNames); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumNodeNames() const { return m(numNodeNames) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numNodeNames)); }
  void NumMapping(uint32 value) { if (m(numMapping) >= 0) *reinterpret_cast<uint32*>(data + m(numMapping)) = value; }
  void NumBindPose(uint32 value) { if (m(numBindPose) >= 0) *reinterpret_cast<uint32*>(data + m(numBindPose)) = value; }
  void InitSkinTransform(es::Matrix44 value) { if (m(initSkinTransform) >= 0) *reinterpret_cast<es::Matrix44*>(data + m(initSkinTransform)) = value; }
  void NumNodeNames(uint32 value) { if (m(numNodeNames) >= 0) *reinterpret_cast<uint32*>(data + m(numNodeNames)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxScene {
enum Members {
  appliedTransform,
  asset,
  basehkReferenceObject,
  cameras,
  externalTextures,
  inplaceTextures,
  lights,
  materials,
  meshes,
  modeller,
  numCameras,
  numExternalTextures,
  numFrames,
  numInplaceTextures,
  numLights,
  numMaterials,
  numMeshes,
  numSelectionSets,
  numSkinBindings,
  numSplines,
  rootNode,
  sceneLength,
  selectionSets,
  skinBindings,
  splines,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 208}, {160, 8, -1, 48, 128, 112, 64, 96, 80, 0, 56, 136, -1, 120, 72, 104, 88, 40, 152, -1, 24, 16, 32, 144, -1}, {0x0, 0xa8a0, 0x82a, 0x0}},
  {{{{HK700, HK2011_3, 8, 0}}, 224}, {176, 24, 0, 64, 144, 128, 80, 112, 96, 16, 72, 152, -1, 136, 88, 120, 104, 56, 168, -1, 40, 32, 48, 160, -1}, {0x0, 0xa8a0, 0x82a, 0x0}},
  {{{{HK2012_1, HK2015, 8, 0}}, 240}, {192, 24, 0, 64, 144, 128, 80, 112, 96, 16, 72, 152, 36, 136, 88, 120, 104, 56, 168, 184, 40, 32, 48, 160, 176}, {0x0, 0xaaa0, 0x8aa, 0x0}},
  {{{{HK2016, HK2019, 8, 0}}, 256}, {208, 32, 0, 72, 152, 136, 88, 120, 104, 24, 80, 160, 44, 144, 96, 128, 112, 64, 176, 192, 48, 40, 56, 168, 184}, {0x0, 0xaaa0, 0x8aa, 0x0}},
  {{{{HK500, HK660, 4, 0}}, 128}, {80, 4, -1, 24, 64, 56, 32, 48, 40, 0, 28, 68, -1, 60, 36, 52, 44, 20, 76, -1, 12, 8, 16, 72, -1}, {0x0, 0xa8a0, 0x82a, 0x0}},
  {{{{HK700, HK2011_3, 4, 0}}, 176}, {128, 12, 0, 36, 96, 84, 48, 72, 60, 8, 40, 100, -1, 88, 52, 76, 64, 28, 112, -1, 20, 16, 24, 108, -1}, {0x0, 0xa8a0, 0x82a, 0x0}},
  {{{{HK2012_1, HK2015, 4, 0}}, 192}, {144, 12, 0, 40, 100, 88, 52, 76, 64, 8, 44, 104, 20, 92, 56, 80, 68, 32, 116, 128, 24, 16, 28, 112, 124}, {0x0, 0xaaa0, 0x8aa, 0x0}},
  {{{{HK2016, HK2019, 4, 0}}, 192}, {144, 16, 0, 44, 104, 92, 56, 80, 68, 12, 48, 108, 24, 96, 60, 84, 72, 36, 120, 132, 28, 20, 32, 116, 128}, {0x0, 0xaaa0, 0x8aa, 0x0}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<char> ModellerPtr() {
    int16 off = m(modeller); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *Modeller() {
    int16 off = m(modeller); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *Modeller() const {
    int16 off = m(modeller); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  Pointer<char> AssetPtr() {
    int16 off = m(asset); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *Asset() {
    int16 off = m(asset); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *Asset() const {
    int16 off = m(asset); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  float SceneLength() const { return m(sceneLength) == -1 ? float{} : *reinterpret_cast<float*>(data + m(sceneLength)); }
  Pointer<hkxNode::Interface> RootNodePtr() {
    int16 off = m(rootNode); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkxNode::Interface> RootNode() {
    int16 off = m(rootNode); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxNode::Interface> RootNode() const {
    int16 off = m(rootNode); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Pointer<Pointer<hkxNodeSelectionSet::Interface>> SelectionSetsPtr() {
    int16 off = m(selectionSets); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxNodeSelectionSet::Interface>> SelectionSets() {
    int16 off = m(selectionSets); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxNodeSelectionSet::Interface>> SelectionSets() const {
    int16 off = m(selectionSets); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumSelectionSets() const { return m(numSelectionSets) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numSelectionSets)); }
  Pointer<Pointer<hkxCamera::Interface>> CamerasPtr() {
    int16 off = m(cameras); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxCamera::Interface>> Cameras() {
    int16 off = m(cameras); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxCamera::Interface>> Cameras() const {
    int16 off = m(cameras); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumCameras() const { return m(numCameras) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numCameras)); }
  Pointer<Pointer<hkxLight::Interface>> LightsPtr() {
    int16 off = m(lights); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxLight::Interface>> Lights() {
    int16 off = m(lights); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxLight::Interface>> Lights() const {
    int16 off = m(lights); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumLights() const { return m(numLights) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numLights)); }
  Pointer<Pointer<hkReferenceObject::Interface>> MeshesPtr() {
    int16 off = m(meshes); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkReferenceObject::Interface>> Meshes() {
    int16 off = m(meshes); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkReferenceObject::Interface>> Meshes() const {
    int16 off = m(meshes); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumMeshes() const { return m(numMeshes) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numMeshes)); }
  Pointer<Pointer<hkReferenceObject::Interface>> MaterialsPtr() {
    int16 off = m(materials); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkReferenceObject::Interface>> Materials() {
    int16 off = m(materials); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkReferenceObject::Interface>> Materials() const {
    int16 off = m(materials); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumMaterials() const { return m(numMaterials) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numMaterials)); }
  Pointer<Pointer<hkxTextureInplace::Interface>> InplaceTexturesPtr() {
    int16 off = m(inplaceTextures); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxTextureInplace::Interface>> InplaceTextures() {
    int16 off = m(inplaceTextures); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxTextureInplace::Interface>> InplaceTextures() const {
    int16 off = m(inplaceTextures); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumInplaceTextures() const { return m(numInplaceTextures) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numInplaceTextures)); }
  Pointer<Pointer<hkxTextureFile::Interface>> ExternalTexturesPtr() {
    int16 off = m(externalTextures); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxTextureFile::Interface>> ExternalTextures() {
    int16 off = m(externalTextures); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxTextureFile::Interface>> ExternalTextures() const {
    int16 off = m(externalTextures); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumExternalTextures() const { return m(numExternalTextures) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numExternalTextures)); }
  Pointer<Pointer<hkxSkinBinding::Interface>> SkinBindingsPtr() {
    int16 off = m(skinBindings); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxSkinBinding::Interface>> SkinBindings() {
    int16 off = m(skinBindings); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxSkinBinding::Interface>> SkinBindings() const {
    int16 off = m(skinBindings); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumSkinBindings() const { return m(numSkinBindings) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numSkinBindings)); }
  hkMatrix3 AppliedTransform() const { return m(appliedTransform) == -1 ? hkMatrix3{} : *reinterpret_cast<hkMatrix3*>(data + m(appliedTransform)); }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 NumFrames() const { return m(numFrames) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numFrames)); }
  Pointer<Pointer<hkxSpline::Interface>> SplinesPtr() {
    int16 off = m(splines); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxSpline::Interface>> Splines() {
    int16 off = m(splines); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxSpline::Interface>> Splines() const {
    int16 off = m(splines); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumSplines() const { return m(numSplines) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numSplines)); }
  void SceneLength(float value) { if (m(sceneLength) >= 0) *reinterpret_cast<float*>(data + m(sceneLength)) = value; }
  void NumSelectionSets(uint32 value) { if (m(numSelectionSets) >= 0) *reinterpret_cast<uint32*>(data + m(numSelectionSets)) = value; }
  void NumCameras(uint32 value) { if (m(numCameras) >= 0) *reinterpret_cast<uint32*>(data + m(numCameras)) = value; }
  void NumLights(uint32 value) { if (m(numLights) >= 0) *reinterpret_cast<uint32*>(data + m(numLights)) = value; }
  void NumMeshes(uint32 value) { if (m(numMeshes) >= 0) *reinterpret_cast<uint32*>(data + m(numMeshes)) = value; }
  void NumMaterials(uint32 value) { if (m(numMaterials) >= 0) *reinterpret_cast<uint32*>(data + m(numMaterials)) = value; }
  void NumInplaceTextures(uint32 value) { if (m(numInplaceTextures) >= 0) *reinterpret_cast<uint32*>(data + m(numInplaceTextures)) = value; }
  void NumExternalTextures(uint32 value) { if (m(numExternalTextures) >= 0) *reinterpret_cast<uint32*>(data + m(numExternalTextures)) = value; }
  void NumSkinBindings(uint32 value) { if (m(numSkinBindings) >= 0) *reinterpret_cast<uint32*>(data + m(numSkinBindings)) = value; }
  void AppliedTransform(hkMatrix3 value) { if (m(appliedTransform) >= 0) *reinterpret_cast<hkMatrix3*>(data + m(appliedTransform)) = value; }
  void NumFrames(uint32 value) { if (m(numFrames) >= 0) *reinterpret_cast<uint32*>(data + m(numFrames)) = value; }
  void NumSplines(uint32 value) { if (m(numSplines) >= 0) *reinterpret_cast<uint32*>(data + m(numSplines)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
