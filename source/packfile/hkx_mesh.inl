// This file has been automatically generated. Do not modify.
#include "hkx_vertexformat.hpp"
#include "hkx_attribute_holder.inl"
namespace clgen::hkxVertexDescriptionElementDecl {
enum Members {
  byteOffset,
  byteStride,
  channelID,
  hint,
  numElements,
  type,
  usage,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 8}, {0, -1, -1, -1, -1, 4, 6}, {0x1402}},
  {{{{HK700, HK2010_1, 8, 0}}, 12}, {0, 8, -1, -1, -1, 4, 6}, {0x140a}},
  {{{{HK2010_2, HK2012_1, 8, 0}}, 16}, {0, 8, -1, -1, 12, 4, 6}, {0x140a}},
  {{{{HK2012_2, HK2013_3, 8, 0}}, 16}, {0, 8, -1, 14, 12, 4, 6}, {0x144a}},
  {{{{HK2014, HK2019, 8, 0}}, 24}, {0, 8, 16, -1, 12, 4, 6}, {0x140a}},
  {{{{HK500, HK660, 4, 0}}, 8}, {0, -1, -1, -1, -1, 4, 6}, {0x1402}},
  {{{{HK700, HK2010_1, 4, 0}}, 12}, {0, 8, -1, -1, -1, 4, 6}, {0x140a}},
  {{{{HK2010_2, HK2012_1, 4, 0}}, 16}, {0, 8, -1, -1, 12, 4, 6}, {0x140a}},
  {{{{HK2012_2, HK2013_3, 4, 0}}, 16}, {0, 8, -1, 14, 12, 4, 6}, {0x144a}},
  {{{{HK2014, HK2019, 4, 0}}, 20}, {0, 8, 16, -1, 12, 4, 6}, {0x140a}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  uint32 ByteOffset() const { return m(byteOffset) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(byteOffset)); }
  ::DataType Type() const { return m(type) == -1 ? ::DataType{} : *reinterpret_cast<::DataType*>(data + m(type)); }
  ::DataUsage Usage() const { return m(usage) == -1 ? ::DataUsage{} : *reinterpret_cast<::DataUsage*>(data + m(usage)); }
  uint32 ByteStride() const { return m(byteStride) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(byteStride)); }
  uint8 NumElements() const { return m(numElements) == -1 ? uint8{} : *reinterpret_cast<uint8*>(data + m(numElements)); }
  ::hkxVertexDescriptionDataHint Hint() const { return m(hint) == -1 ? ::hkxVertexDescriptionDataHint{} : *reinterpret_cast<::hkxVertexDescriptionDataHint*>(data + m(hint)); }
  Pointer<char> ChannelIDPtr() {
    int16 off = m(channelID); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  char *ChannelID() {
    int16 off = m(channelID); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  const char *ChannelID() const {
    int16 off = m(channelID); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<char**>(data + off);
    return *reinterpret_cast<es::PointerX86<char>*>(data + off);
  }
  void ByteOffset(uint32 value) { if (m(byteOffset) >= 0) *reinterpret_cast<uint32*>(data + m(byteOffset)) = value; }
  void Type(::DataType value) { if (m(type) >= 0) *reinterpret_cast<::DataType*>(data + m(type)) = value; }
  void Usage(::DataUsage value) { if (m(usage) >= 0) *reinterpret_cast<::DataUsage*>(data + m(usage)) = value; }
  void ByteStride(uint32 value) { if (m(byteStride) >= 0) *reinterpret_cast<uint32*>(data + m(byteStride)) = value; }
  void NumElements(uint8 value) { if (m(numElements) >= 0) *reinterpret_cast<uint8*>(data + m(numElements)) = value; }
  void Hint(::hkxVertexDescriptionDataHint value) { if (m(hint) >= 0) *reinterpret_cast<::hkxVertexDescriptionDataHint*>(data + m(hint)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxVertexDescription {
enum Members {
  decls,
  numDecls,
  stride,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 24}, {8, 16, 0}, {0x28}},
  {{{{HK700, HK2019, 8, 0}}, 16}, {0, 8, -1}, {0x8}},
  {{{{HK500, HK660, 4, 0}}, 16}, {4, 8, 0}, {0x28}},
  {{{{HK700, HK2019, 4, 0}}, 12}, {0, 4, -1}, {0x8}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  uint32 Stride() const { return m(stride) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(stride)); }
  Pointer<hkxVertexDescriptionElementDecl::Interface> DeclsPtr() {
    int16 off = m(decls); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkxVertexDescriptionElementDecl::Interface> Decls() {
    int16 off = m(decls); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxVertexDescriptionElementDecl::Interface> Decls() const {
    int16 off = m(decls); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumDecls() const { return m(numDecls) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numDecls)); }
  void Stride(uint32 value) { if (m(stride) >= 0) *reinterpret_cast<uint32*>(data + m(stride)) = value; }
  void NumDecls(uint32 value) { if (m(numDecls) >= 0) *reinterpret_cast<uint32*>(data + m(numDecls)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxVertexBufferVertexData {
enum Members {
  floatData,
  floatStride,
  numFloatData,
  numUint16Data,
  numUint32Data,
  numUint8Data,
  numVectorData,
  numVerts,
  uint16Data,
  uint16Stride,
  uint32Data,
  uint32Stride,
  uint8Data,
  uint8Stride,
  vectorData,
  vectorStride,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2019, 8, 0}}, 104}, {16, 88, 24, 56, 40, 72, 8, 80, 48, 96, 32, 92, 64, 100, 0, 84}, {0xaaa8, 0x8888}},
  {{{{HK500, HK2019, 4, 0}}, 84}, {12, 68, 16, 40, 28, 52, 4, 60, 36, 76, 24, 72, 48, 80, 0, 64}, {0xaaa8, 0x8888}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<Vector4> VectorDataPtr() {
    int16 off = m(vectorData); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Vector4 *VectorData() {
    int16 off = m(vectorData); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<Vector4**>(data + off);
    return *reinterpret_cast<es::PointerX86<Vector4>*>(data + off);
  }
  const Vector4 *VectorData() const {
    int16 off = m(vectorData); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<Vector4**>(data + off);
    return *reinterpret_cast<es::PointerX86<Vector4>*>(data + off);
  }
  uint32 NumVectorData() const { return m(numVectorData) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numVectorData)); }
  Pointer<float> VectorDataPtrHK2013_1() {
    int16 off = m(vectorData); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  float *VectorDataHK2013_1() {
    int16 off = m(vectorData); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  const float *VectorDataHK2013_1() const {
    int16 off = m(vectorData); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  Pointer<float> FloatDataPtr() {
    int16 off = m(floatData); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  float *FloatData() {
    int16 off = m(floatData); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  const float *FloatData() const {
    int16 off = m(floatData); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<float**>(data + off);
    return *reinterpret_cast<es::PointerX86<float>*>(data + off);
  }
  uint32 NumFloatData() const { return m(numFloatData) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numFloatData)); }
  Pointer<uint32> Uint32DataPtr() {
    int16 off = m(uint32Data); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 *Uint32Data() {
    int16 off = m(uint32Data); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  const uint32 *Uint32Data() const {
    int16 off = m(uint32Data); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  uint32 NumUint32Data() const { return m(numUint32Data) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numUint32Data)); }
  Pointer<uint16> Uint16DataPtr() {
    int16 off = m(uint16Data); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint16 *Uint16Data() {
    int16 off = m(uint16Data); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint16**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint16>*>(data + off);
  }
  const uint16 *Uint16Data() const {
    int16 off = m(uint16Data); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint16**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint16>*>(data + off);
  }
  uint32 NumUint16Data() const { return m(numUint16Data) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numUint16Data)); }
  Pointer<uint8> Uint8DataPtr() {
    int16 off = m(uint8Data); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint8 *Uint8Data() {
    int16 off = m(uint8Data); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint8**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint8>*>(data + off);
  }
  const uint8 *Uint8Data() const {
    int16 off = m(uint8Data); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint8**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint8>*>(data + off);
  }
  uint32 NumUint8Data() const { return m(numUint8Data) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numUint8Data)); }
  uint32 NumVerts() const { return m(numVerts) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numVerts)); }
  uint32 VectorStride() const { return m(vectorStride) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(vectorStride)); }
  uint32 FloatStride() const { return m(floatStride) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(floatStride)); }
  uint32 Uint32Stride() const { return m(uint32Stride) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(uint32Stride)); }
  uint32 Uint16Stride() const { return m(uint16Stride) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(uint16Stride)); }
  uint32 Uint8Stride() const { return m(uint8Stride) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(uint8Stride)); }
  void NumVectorData(uint32 value) { if (m(numVectorData) >= 0) *reinterpret_cast<uint32*>(data + m(numVectorData)) = value; }
  void NumFloatData(uint32 value) { if (m(numFloatData) >= 0) *reinterpret_cast<uint32*>(data + m(numFloatData)) = value; }
  void NumUint32Data(uint32 value) { if (m(numUint32Data) >= 0) *reinterpret_cast<uint32*>(data + m(numUint32Data)) = value; }
  void NumUint16Data(uint32 value) { if (m(numUint16Data) >= 0) *reinterpret_cast<uint32*>(data + m(numUint16Data)) = value; }
  void NumUint8Data(uint32 value) { if (m(numUint8Data) >= 0) *reinterpret_cast<uint32*>(data + m(numUint8Data)) = value; }
  void NumVerts(uint32 value) { if (m(numVerts) >= 0) *reinterpret_cast<uint32*>(data + m(numVerts)) = value; }
  void VectorStride(uint32 value) { if (m(vectorStride) >= 0) *reinterpret_cast<uint32*>(data + m(vectorStride)) = value; }
  void FloatStride(uint32 value) { if (m(floatStride) >= 0) *reinterpret_cast<uint32*>(data + m(floatStride)) = value; }
  void Uint32Stride(uint32 value) { if (m(uint32Stride) >= 0) *reinterpret_cast<uint32*>(data + m(uint32Stride)) = value; }
  void Uint16Stride(uint32 value) { if (m(uint16Stride) >= 0) *reinterpret_cast<uint32*>(data + m(uint16Stride)) = value; }
  void Uint8Stride(uint32 value) { if (m(uint8Stride) >= 0) *reinterpret_cast<uint32*>(data + m(uint8Stride)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxVertexBuffer {
enum Members {
  basehkReferenceObject,
  data_,
  desc,
  format,
  numVertexData,
  vertexData,
  vertexDataClass,
  vertexDesc,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK550, 8, 0}}, 32}, {-1, -1, -1, 24, 16, 8, 0, -1}, {0x200}},
  {{{{HK600, HK660, 8, 0}}, 32}, {-1, -1, -1, -1, 16, 8, 0, 24}, {0x200}},
  {{{{HK700, HK2015, 8, 0}}, 136}, {0, 16, 120, -1, -1, -1, -1, -1}, {}},
  {{{{HK2016, HK2019, 8, 0}}, 144}, {0, 24, 128, -1, -1, -1, -1, -1}, {}},
  {{{{HK500, HK550, 4, 0}}, 16}, {-1, -1, -1, 12, 8, 4, 0, -1}, {0x200}},
  {{{{HK600, HK660, 4, 0}}, 16}, {-1, -1, -1, -1, 8, 4, 0, 12}, {0x200}},
  {{{{HK700, HK2015, 4, 0}}, 104}, {0, 8, 92, -1, -1, -1, -1, -1}, {}},
  {{{{HK2016, HK2019, 4, 0}}, 108}, {0, 12, 96, -1, -1, -1, -1, -1}, {}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<int8> VertexDataClassPtr() {
    int16 off = m(vertexDataClass); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  int8 *VertexDataClass() {
    int16 off = m(vertexDataClass); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int8**>(data + off);
    return *reinterpret_cast<es::PointerX86<int8>*>(data + off);
  }
  const int8 *VertexDataClass() const {
    int16 off = m(vertexDataClass); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int8**>(data + off);
    return *reinterpret_cast<es::PointerX86<int8>*>(data + off);
  }
  Pointer<uint8> VertexDataPtr() {
    int16 off = m(vertexData); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint8 *VertexData() {
    int16 off = m(vertexData); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint8**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint8>*>(data + off);
  }
  const uint8 *VertexData() const {
    int16 off = m(vertexData); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint8**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint8>*>(data + off);
  }
  uint32 NumVertexData() const { return m(numVertexData) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numVertexData)); }
  Pointer<::hkxVertexFormat> FormatPtr() {
    int16 off = m(format); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  ::hkxVertexFormat *Format() {
    int16 off = m(format); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<::hkxVertexFormat**>(data + off);
    return *reinterpret_cast<es::PointerX86<::hkxVertexFormat>*>(data + off);
  }
  const ::hkxVertexFormat *Format() const {
    int16 off = m(format); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<::hkxVertexFormat**>(data + off);
    return *reinterpret_cast<es::PointerX86<::hkxVertexFormat>*>(data + off);
  }
  Pointer<hkxVertexDescription::Interface> VertexDescPtr() {
    int16 off = m(vertexDesc); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkxVertexDescription::Interface> VertexDesc() {
    int16 off = m(vertexDesc); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxVertexDescription::Interface> VertexDesc() const {
    int16 off = m(vertexDesc); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  hkxVertexBufferVertexData::Interface Data_() const { return {m(data_) == -1 ? nullptr : data + m(data_), lookup}; }
  hkxVertexDescription::Interface Desc() const { return {m(desc) == -1 ? nullptr : data + m(desc), lookup}; }
  void NumVertexData(uint32 value) { if (m(numVertexData) >= 0) *reinterpret_cast<uint32*>(data + m(numVertexData)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxIndexBuffer {
enum Members {
  basehkReferenceObject,
  indexType,
  indices16,
  indices32,
  length,
  numIndices16,
  numIndices32,
  vertexBaseOffset,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 48}, {-1, 0, 8, 24, 40, 16, 32, 36}, {0xaa00}},
  {{{{HK700, HK2015, 8, 0}}, 64}, {0, 16, 24, 40, 60, 32, 48, 56}, {0xaa00}},
  {{{{HK2016, HK2019, 8, 0}}, 72}, {0, 24, 32, 48, 68, 40, 56, 64}, {0xaa00}},
  {{{{HK500, HK660, 4, 0}}, 28}, {-1, 0, 4, 12, 24, 8, 16, 20}, {0xaa00}},
  {{{{HK700, HK2015, 4, 0}}, 44}, {0, 8, 12, 24, 40, 16, 28, 36}, {0xaa00}},
  {{{{HK2016, HK2019, 4, 0}}, 48}, {0, 12, 16, 28, 44, 20, 32, 40}, {0xaa00}},
  {{{{HK500, HK660, 8, 1}}, 48}, {-1, 0, 8, 24, 40, 16, 32, 36}, {0xaa00}},
  {{{{HK700, HK2015, 8, 1}}, 56}, {0, 12, 16, 32, 52, 24, 40, 48}, {0xaa00}},
  {{{{HK2016, HK2019, 8, 1}}, 64}, {0, 20, 24, 40, 60, 32, 48, 56}, {0xaa00}},
  {{{{HK500, HK660, 4, 1}}, 28}, {-1, 0, 4, 12, 24, 8, 16, 20}, {0xaa00}},
  {{{{HK700, HK2015, 4, 1}}, 44}, {0, 8, 12, 24, 40, 16, 28, 36}, {0xaa00}},
  {{{{HK2016, HK2019, 4, 1}}, 48}, {0, 12, 16, 28, 44, 20, 32, 40}, {0xaa00}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr ,LookupFlag::Padding}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  ::IndexType IndexType() const { return m(indexType) == -1 ? ::IndexType{} : *reinterpret_cast<::IndexType*>(data + m(indexType)); }
  Pointer<uint16> Indices16Ptr() {
    int16 off = m(indices16); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint16 *Indices16() {
    int16 off = m(indices16); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint16**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint16>*>(data + off);
  }
  const uint16 *Indices16() const {
    int16 off = m(indices16); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint16**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint16>*>(data + off);
  }
  uint32 NumIndices16() const { return m(numIndices16) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numIndices16)); }
  Pointer<uint32> Indices32Ptr() {
    int16 off = m(indices32); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  uint32 *Indices32() {
    int16 off = m(indices32); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  const uint32 *Indices32() const {
    int16 off = m(indices32); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<uint32**>(data + off);
    return *reinterpret_cast<es::PointerX86<uint32>*>(data + off);
  }
  uint32 NumIndices32() const { return m(numIndices32) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numIndices32)); }
  uint32 VertexBaseOffset() const { return m(vertexBaseOffset) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(vertexBaseOffset)); }
  uint32 Length() const { return m(length) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(length)); }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  void IndexType(::IndexType value) { if (m(indexType) >= 0) *reinterpret_cast<::IndexType*>(data + m(indexType)) = value; }
  void NumIndices16(uint32 value) { if (m(numIndices16) >= 0) *reinterpret_cast<uint32*>(data + m(numIndices16)) = value; }
  void NumIndices32(uint32 value) { if (m(numIndices32) >= 0) *reinterpret_cast<uint32*>(data + m(numIndices32)) = value; }
  void VertexBaseOffset(uint32 value) { if (m(vertexBaseOffset) >= 0) *reinterpret_cast<uint32*>(data + m(vertexBaseOffset)) = value; }
  void Length(uint32 value) { if (m(length) >= 0) *reinterpret_cast<uint32*>(data + m(length)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxVertexAnimation {
enum Members {
  basehkReferenceObject,
  componentMap,
  numComponentMap,
  numVertexIndexMap,
  time,
  vertData,
  vertexIndexMap,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 88}, {0, 72, 80, 64, 16, 24, 56}, {0x2a0}},
  {{{{HK700, HK2015, 8, 0}}, 192}, {0, 176, 184, 168, 16, 24, 160}, {0x2a0}},
  {{{{HK2016, HK2019, 8, 0}}, 208}, {0, 192, 200, 184, 24, 32, 176}, {0x2a0}},
  {{{{HK500, HK660, 4, 0}}, 52}, {0, 40, 44, 32, 8, 12, 28}, {0x2a0}},
  {{{{HK700, HK2015, 4, 0}}, 140}, {0, 128, 132, 120, 8, 12, 116}, {0x2a0}},
  {{{{HK2016, HK2019, 4, 0}}, 148}, {0, 136, 140, 128, 12, 16, 124}, {0x2a0}},
  {{{{HK500, HK660, 8, 1}}, 80}, {0, 64, 72, 56, 12, 16, 48}, {0x2a0}},
  {{{{HK700, HK2015, 8, 1}}, 184}, {0, 168, 176, 160, 12, 16, 152}, {0x2a0}},
  {{{{HK2016, HK2019, 8, 1}}, 200}, {0, 184, 192, 176, 20, 24, 168}, {0x2a0}},
  {{{{HK500, HK660, 4, 1}}, 52}, {0, 40, 44, 32, 8, 12, 28}, {0x2a0}},
  {{{{HK700, HK2015, 4, 1}}, 140}, {0, 128, 132, 120, 8, 12, 116}, {0x2a0}},
  {{{{HK2016, HK2019, 4, 1}}, 148}, {0, 136, 140, 128, 12, 16, 124}, {0x2a0}}
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
  float Time() const { return m(time) == -1 ? float{} : *reinterpret_cast<float*>(data + m(time)); }
  hkxVertexBuffer::Interface VertData() const { return {m(vertData) == -1 ? nullptr : data + m(vertData), lookup}; }
  Pointer<int32> VertexIndexMapPtr() {
    int16 off = m(vertexIndexMap); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  int32 *VertexIndexMap() {
    int16 off = m(vertexIndexMap); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int32**>(data + off);
    return *reinterpret_cast<es::PointerX86<int32>*>(data + off);
  }
  const int32 *VertexIndexMap() const {
    int16 off = m(vertexIndexMap); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int32**>(data + off);
    return *reinterpret_cast<es::PointerX86<int32>*>(data + off);
  }
  uint32 NumVertexIndexMap() const { return m(numVertexIndexMap) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numVertexIndexMap)); }
  Pointer<::hkxVertexAnimationUsageMap> ComponentMapPtr() {
    int16 off = m(componentMap); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  ::hkxVertexAnimationUsageMap *ComponentMap() {
    int16 off = m(componentMap); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<::hkxVertexAnimationUsageMap**>(data + off);
    return *reinterpret_cast<es::PointerX86<::hkxVertexAnimationUsageMap>*>(data + off);
  }
  const ::hkxVertexAnimationUsageMap *ComponentMap() const {
    int16 off = m(componentMap); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<::hkxVertexAnimationUsageMap**>(data + off);
    return *reinterpret_cast<es::PointerX86<::hkxVertexAnimationUsageMap>*>(data + off);
  }
  uint32 NumComponentMap() const { return m(numComponentMap) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numComponentMap)); }
  void Time(float value) { if (m(time) >= 0) *reinterpret_cast<float*>(data + m(time)) = value; }
  void NumVertexIndexMap(uint32 value) { if (m(numVertexIndexMap) >= 0) *reinterpret_cast<uint32*>(data + m(numVertexIndexMap)) = value; }
  void NumComponentMap(uint32 value) { if (m(numComponentMap) >= 0) *reinterpret_cast<uint32*>(data + m(numComponentMap)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkMeshBoneIndexMapping {
enum Members {
  mapping,
  numMapping,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK2019, 8, 0}}, 16}, {0, 8}, {0x8}},
  {{{{HK500, HK2019, 4, 0}}, 12}, {0, 4}, {0x8}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<int16> MappingPtr() {
    int16 off = m(mapping); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  int16 *Mapping() {
    int16 off = m(mapping); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  const int16 *Mapping() const {
    int16 off = m(mapping); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<int16**>(data + off);
    return *reinterpret_cast<es::PointerX86<int16>*>(data + off);
  }
  uint32 NumMapping() const { return m(numMapping) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numMapping)); }
  void NumMapping(uint32 value) { if (m(numMapping) >= 0) *reinterpret_cast<uint32*>(data + m(numMapping)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxMeshSection {
enum Members {
  basehkReferenceObject,
  boneMatrixMap,
  indexBuffers,
  linearKeyFrameHints,
  material,
  numBoneMatrixMap,
  numIndexBuffers,
  numLinearKeyFrameHints,
  numUserChannels,
  numVertexAnimations,
  userChannels,
  vertexAnimations,
  vertexBuffer,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 48}, {-1, -1, 8, -1, 24, -1, 16, -1, 40, -1, 32, -1, 0}, {0x2000, 0x2}},
  {{{{HK700, HK2011_3, 8, 0}}, 64}, {0, -1, 24, -1, 40, -1, 32, -1, 56, -1, 48, -1, 16}, {0x2000, 0x2}},
  {{{{HK2012_1, HK2012_1, 8, 0}}, 80}, {0, -1, 24, -1, 40, -1, 32, -1, 56, 72, 48, 64, 16}, {0x2000, 0xa}},
  {{{{HK2012_2, HK2013_3, 8, 0}}, 96}, {0, -1, 24, 80, 40, -1, 32, 88, 56, 72, 48, 64, 16}, {0xa000, 0xa}},
  {{{{HK2014, HK2015, 8, 0}}, 112}, {0, 96, 24, 80, 40, 104, 32, 88, 56, 72, 48, 64, 16}, {0xa800, 0xa}},
  {{{{HK2016, HK2019, 8, 0}}, 120}, {0, 104, 32, 88, 48, 112, 40, 96, 64, 80, 56, 72, 24}, {0xa800, 0xa}},
  {{{{HK500, HK660, 4, 0}}, 24}, {-1, -1, 4, -1, 12, -1, 8, -1, 20, -1, 16, -1, 0}, {0x2000, 0x2}},
  {{{{HK700, HK2011_3, 4, 0}}, 40}, {0, -1, 12, -1, 24, -1, 16, -1, 32, -1, 28, -1, 8}, {0x2000, 0x2}},
  {{{{HK2012_1, HK2012_1, 4, 0}}, 52}, {0, -1, 12, -1, 24, -1, 16, -1, 32, 44, 28, 40, 8}, {0x2000, 0xa}},
  {{{{HK2012_2, HK2013_3, 4, 0}}, 64}, {0, -1, 12, 52, 24, -1, 16, 56, 32, 44, 28, 40, 8}, {0xa000, 0xa}},
  {{{{HK2014, HK2015, 4, 0}}, 76}, {0, 64, 12, 52, 24, 68, 16, 56, 32, 44, 28, 40, 8}, {0xa800, 0xa}},
  {{{{HK2016, HK2019, 4, 0}}, 80}, {0, 68, 16, 56, 28, 72, 20, 60, 36, 48, 32, 44, 12}, {0xa800, 0xa}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<hkxVertexBuffer::Interface> VertexBufferPtr() {
    int16 off = m(vertexBuffer); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkxVertexBuffer::Interface> VertexBuffer() {
    int16 off = m(vertexBuffer); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxVertexBuffer::Interface> VertexBuffer() const {
    int16 off = m(vertexBuffer); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Pointer<Pointer<hkxIndexBuffer::Interface>> IndexBuffersPtr() {
    int16 off = m(indexBuffers); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxIndexBuffer::Interface>> IndexBuffers() {
    int16 off = m(indexBuffers); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxIndexBuffer::Interface>> IndexBuffers() const {
    int16 off = m(indexBuffers); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumIndexBuffers() const { return m(numIndexBuffers) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numIndexBuffers)); }
  Pointer<hkReferenceObject::Interface> MaterialPtr() {
    int16 off = m(material); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkReferenceObject::Interface> Material() {
    int16 off = m(material); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkReferenceObject::Interface> Material() const {
    int16 off = m(material); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Pointer<hkVariant::Interface> UserChannelsPtr() {
    int16 off = m(userChannels); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkVariant::Interface> UserChannels() {
    int16 off = m(userChannels); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkVariant::Interface> UserChannels() const {
    int16 off = m(userChannels); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumUserChannels() const { return m(numUserChannels) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numUserChannels)); }
  Pointer<Pointer<hkReferenceObject::Interface>> UserChannelsPtrHK700() {
    int16 off = m(userChannels); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkReferenceObject::Interface>> UserChannelsHK700() {
    int16 off = m(userChannels); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkReferenceObject::Interface>> UserChannelsHK700() const {
    int16 off = m(userChannels); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Pointer<Pointer<hkxVertexAnimation::Interface>> VertexAnimationsPtr() {
    int16 off = m(vertexAnimations); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxVertexAnimation::Interface>> VertexAnimations() {
    int16 off = m(vertexAnimations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxVertexAnimation::Interface>> VertexAnimations() const {
    int16 off = m(vertexAnimations); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumVertexAnimations() const { return m(numVertexAnimations) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numVertexAnimations)); }
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
  Pointer<hkMeshBoneIndexMapping::Interface> BoneMatrixMapPtr() {
    int16 off = m(boneMatrixMap); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkMeshBoneIndexMapping::Interface> BoneMatrixMap() {
    int16 off = m(boneMatrixMap); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkMeshBoneIndexMapping::Interface> BoneMatrixMap() const {
    int16 off = m(boneMatrixMap); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumBoneMatrixMap() const { return m(numBoneMatrixMap) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numBoneMatrixMap)); }
  void NumIndexBuffers(uint32 value) { if (m(numIndexBuffers) >= 0) *reinterpret_cast<uint32*>(data + m(numIndexBuffers)) = value; }
  void NumUserChannels(uint32 value) { if (m(numUserChannels) >= 0) *reinterpret_cast<uint32*>(data + m(numUserChannels)) = value; }
  void NumVertexAnimations(uint32 value) { if (m(numVertexAnimations) >= 0) *reinterpret_cast<uint32*>(data + m(numVertexAnimations)) = value; }
  void NumLinearKeyFrameHints(uint32 value) { if (m(numLinearKeyFrameHints) >= 0) *reinterpret_cast<uint32*>(data + m(numLinearKeyFrameHints)) = value; }
  void NumBoneMatrixMap(uint32 value) { if (m(numBoneMatrixMap) >= 0) *reinterpret_cast<uint32*>(data + m(numBoneMatrixMap)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxMeshUserChannelInfo {
enum Members {
  basehkxAttributeHolder,
  className,
  name,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 32}, {0, 24, 16}, {0x0}},
  {{{{HK700, HK2015, 8, 0}}, 48}, {0, 40, 32}, {0x0}},
  {{{{HK2016, HK2019, 8, 0}}, 56}, {0, 48, 40}, {0x0}},
  {{{{HK500, HK660, 4, 0}}, 16}, {0, 12, 8}, {0x0}},
  {{{{HK700, HK2015, 4, 0}}, 28}, {0, 24, 20}, {0x0}},
  {{{{HK2016, HK2019, 4, 0}}, 32}, {0, 28, 24}, {0x0}}
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
  

  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxMesh {
enum Members {
  basehkReferenceObject,
  numSections,
  numUserChannelInfos,
  sections,
  userChannelInfos,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 32}, {-1, 8, 24, 0, 16}, {0x28}},
  {{{{HK700, HK2015, 8, 0}}, 48}, {0, 24, 40, 16, 32}, {0x28}},
  {{{{HK2016, HK2019, 8, 0}}, 56}, {0, 32, 48, 24, 40}, {0x28}},
  {{{{HK500, HK660, 4, 0}}, 16}, {-1, 4, 12, 0, 8}, {0x28}},
  {{{{HK700, HK2015, 4, 0}}, 32}, {0, 12, 24, 8, 20}, {0x28}},
  {{{{HK2016, HK2019, 4, 0}}, 36}, {0, 16, 28, 12, 24}, {0x28}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  Pointer<Pointer<hkxMeshSection::Interface>> SectionsPtr() {
    int16 off = m(sections); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxMeshSection::Interface>> Sections() {
    int16 off = m(sections); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxMeshSection::Interface>> Sections() const {
    int16 off = m(sections); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumSections() const { return m(numSections) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numSections)); }
  Pointer<hkxMeshUserChannelInfo::Interface> UserChannelInfosPtr() {
    int16 off = m(userChannelInfos); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkxMeshUserChannelInfo::Interface> UserChannelInfos() {
    int16 off = m(userChannelInfos); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxMeshUserChannelInfo::Interface> UserChannelInfos() const {
    int16 off = m(userChannelInfos); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumUserChannelInfos() const { return m(numUserChannelInfos) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numUserChannelInfos)); }
  Pointer<Pointer<hkxMeshUserChannelInfo::Interface>> UserChannelInfosPtrHK700() {
    int16 off = m(userChannelInfos); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxMeshUserChannelInfo::Interface>> UserChannelInfosHK700() {
    int16 off = m(userChannelInfos); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxMeshUserChannelInfo::Interface>> UserChannelInfosHK700() const {
    int16 off = m(userChannelInfos); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  hkReferenceObject::Interface BasehkReferenceObject() const {
    int16 off = m(basehkReferenceObject); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  void NumSections(uint32 value) { if (m(numSections) >= 0) *reinterpret_cast<uint32*>(data + m(numSections)) = value; }
  void NumUserChannelInfos(uint32 value) { if (m(numUserChannelInfos) >= 0) *reinterpret_cast<uint32*>(data + m(numUserChannelInfos)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
