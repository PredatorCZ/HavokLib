// This file has been automatically generated. Do not modify.
#include "hkx_attribute_holder.inl"
namespace clgen::hkxMaterialTextureStage {
enum Members {
  tcoordChannel,
  texture,
  usageHint,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK550, 8, 0}}, 24}, {-1, 0, 16}, {0x20}},
  {{{{HK600, HK660, 8, 0}}, 24}, {20, 0, 16}, {0x22}},
  {{{{HK700, HK2019, 8, 0}}, 16}, {12, 0, 8}, {0x22}},
  {{{{HK500, HK550, 4, 0}}, 12}, {-1, 0, 8}, {0x20}},
  {{{{HK600, HK660, 4, 0}}, 16}, {12, 0, 8}, {0x22}},
  {{{{HK700, HK2019, 4, 0}}, 12}, {8, 0, 4}, {0x22}}
};
struct Interface {
  Interface(char *data_, LayoutLookup layout_): data{data_}, layout{GetLayout(LAYOUTS, {layout_, {LookupFlag::Ptr}})}, lookup{layout_} {}
  Interface(const Interface&) = default;
  Interface(Interface&&) = default;
  Interface &operator=(const Interface&) = default;
  Interface &operator=(Interface&&) = default;
  uint16 LayoutVersion() const { return lookup.version; }
  hkVariant::Interface Texture() const { return {m(texture) == -1 ? nullptr : data + m(texture), lookup}; }
  Pointer<hkReferenceObject::Interface> TexturePtr() {
    int16 off = m(texture); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkReferenceObject::Interface> TextureHK700() {
    int16 off = m(texture); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkReferenceObject::Interface> TextureHK700() const {
    int16 off = m(texture); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  ::TextureType UsageHint() const { return m(usageHint) == -1 ? ::TextureType{} : *reinterpret_cast<::TextureType*>(data + m(usageHint)); }
  int32 TcoordChannel() const { return m(tcoordChannel) == -1 ? int32{} : *reinterpret_cast<int32*>(data + m(tcoordChannel)); }
  void UsageHint(::TextureType value) { if (m(usageHint) >= 0) *reinterpret_cast<::TextureType*>(data + m(usageHint)) = value; }
  void TcoordChannel(int32 value) { if (m(tcoordChannel) >= 0) *reinterpret_cast<int32*>(data + m(tcoordChannel)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
namespace clgen::hkxMaterial {
enum Members {
  ambientColor,
  basehkxAttributeHolder,
  diffuseColor,
  emissiveColor,
  extraData,
  name,
  numProperties,
  numStages,
  numSubMaterials,
  properties,
  specularColor,
  specularExponent,
  specularMultiplier,
  stages,
  subMaterials,
  transparency,
  userData,
  uvMapAlgorithm,
  uvMapOffset,
  uvMapRotation,
  uvMapScale,
  _count_,
};
static const std::set<ClassData<_count_>> LAYOUTS {
  {{{{HK500, HK660, 8, 0}}, 144}, {64, 0, 48, 96, 128, 16, -1, 32, 120, -1, 80, -1, -1, 24, 112, -1, -1, -1, -1, -1, -1}, {0x8000, 0x2, 0x0}},
  {{{{HK700, HK710, 8, 0}}, 160}, {80, 0, 64, 112, 144, 32, -1, 48, 136, -1, 96, -1, -1, 40, 128, -1, -1, -1, -1, -1, -1}, {0x8000, 0x2, 0x0}},
  {{{{HK2010_1, HK2011_3, 8, 0}}, 176}, {80, 0, 64, 112, 144, 32, 160, 48, 136, 152, 96, -1, -1, 40, 128, -1, -1, -1, -1, -1, -1}, {0xa000, 0x2, 0x0}},
  {{{{HK2012_1, HK2013_1, 8, 0}}, 208}, {80, 0, 64, 112, 144, 32, 200, 48, 136, 192, 96, 180, 176, 40, 128, 184, -1, 172, 160, 168, 152}, {0xa000, 0x282, 0x88}},
  {{{{HK2013_2, HK2015, 8, 0}}, 224}, {80, 0, 64, 112, 144, 32, 208, 48, 136, 200, 96, 180, 176, 40, 128, 184, 192, 172, 160, 168, 152}, {0xa000, 0x282, 0x8b}},
  {{{{HK2016, HK2019, 8, 0}}, 224}, {80, 0, 64, 112, 144, 40, 208, 56, 136, 200, 96, 180, 176, 48, 128, 184, 192, 172, 160, 168, 152}, {0xa000, 0x282, 0x8b}},
  {{{{HK500, HK660, 4, 0}}, 112}, {48, 0, 32, 80, 104, 8, -1, 16, 100, -1, 64, -1, -1, 12, 96, -1, -1, -1, -1, -1, -1}, {0x8000, 0x2, 0x0}},
  {{{{HK700, HK710, 4, 0}}, 128}, {64, 0, 48, 96, 124, 20, -1, 28, 116, -1, 80, -1, -1, 24, 112, -1, -1, -1, -1, -1, -1}, {0x8000, 0x2, 0x0}},
  {{{{HK2010_1, HK2011_3, 4, 0}}, 144}, {64, 0, 48, 96, 124, 20, 132, 28, 116, 128, 80, -1, -1, 24, 112, -1, -1, -1, -1, -1, -1}, {0xa000, 0x2, 0x0}},
  {{{{HK2012_1, HK2013_1, 4, 0}}, 176}, {64, 0, 48, 96, 124, 20, 168, 28, 116, 164, 80, 156, 152, 24, 112, 160, -1, 148, 136, 144, 128}, {0xa000, 0x282, 0x88}},
  {{{{HK2013_2, HK2015, 4, 0}}, 192}, {64, 0, 48, 96, 124, 20, 180, 28, 116, 176, 80, 156, 152, 24, 112, 160, 168, 148, 136, 144, 128}, {0xa000, 0x282, 0x8b}},
  {{{{HK2016, HK2019, 4, 0}}, 192}, {64, 0, 48, 96, 124, 24, 180, 32, 116, 176, 80, 156, 152, 28, 112, 160, 168, 148, 136, 144, 128}, {0xa000, 0x282, 0x8b}}
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
  Pointer<hkxMaterialTextureStage::Interface> StagesPtr() {
    int16 off = m(stages); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkxMaterialTextureStage::Interface> Stages() {
    int16 off = m(stages); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkxMaterialTextureStage::Interface> Stages() const {
    int16 off = m(stages); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumStages() const { return m(numStages) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numStages)); }
  Vector4A16 DiffuseColor() const { return m(diffuseColor) == -1 ? Vector4A16{} : *reinterpret_cast<Vector4A16*>(data + m(diffuseColor)); }
  Vector4A16 AmbientColor() const { return m(ambientColor) == -1 ? Vector4A16{} : *reinterpret_cast<Vector4A16*>(data + m(ambientColor)); }
  Vector4A16 SpecularColor() const { return m(specularColor) == -1 ? Vector4A16{} : *reinterpret_cast<Vector4A16*>(data + m(specularColor)); }
  Vector4A16 EmissiveColor() const { return m(emissiveColor) == -1 ? Vector4A16{} : *reinterpret_cast<Vector4A16*>(data + m(emissiveColor)); }
  Pointer<Pointer<hkxMaterial::Interface>> SubMaterialsPtr() {
    int16 off = m(subMaterials); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<Pointer<hkxMaterial::Interface>> SubMaterials() {
    int16 off = m(subMaterials); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<Pointer<hkxMaterial::Interface>> SubMaterials() const {
    int16 off = m(subMaterials); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  uint32 NumSubMaterials() const { return m(numSubMaterials) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numSubMaterials)); }
  hkVariant::Interface ExtraData() const { return {m(extraData) == -1 ? nullptr : data + m(extraData), lookup}; }
  Pointer<hkReferenceObject::Interface> ExtraDataPtr() {
    int16 off = m(extraData); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  Iterator<hkReferenceObject::Interface> ExtraDataHK700() {
    int16 off = m(extraData); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Iterator<hkReferenceObject::Interface> ExtraDataHK700() const {
    int16 off = m(extraData); if (off == -1) return {nullptr, lookup};
    if (layout->ptrSize == 8) return {*reinterpret_cast<char**>(data + off), lookup};
    return {*reinterpret_cast<es::PointerX86<char>*>(data + off), lookup};
  }
  Pointer<::hkxMaterialProperty> PropertiesPtr() {
    int16 off = m(properties); if (off == -1) return {nullptr, lookup};
    return {data + off, lookup};
  }
  ::hkxMaterialProperty *Properties() {
    int16 off = m(properties); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<::hkxMaterialProperty**>(data + off);
    return *reinterpret_cast<es::PointerX86<::hkxMaterialProperty>*>(data + off);
  }
  const ::hkxMaterialProperty *Properties() const {
    int16 off = m(properties); if (off == -1) return nullptr;
    if (layout->ptrSize == 8) return *reinterpret_cast<::hkxMaterialProperty**>(data + off);
    return *reinterpret_cast<es::PointerX86<::hkxMaterialProperty>*>(data + off);
  }
  uint32 NumProperties() const { return m(numProperties) == -1 ? uint32{} : *reinterpret_cast<uint32*>(data + m(numProperties)); }
  Vector2 UvMapScale() const { return m(uvMapScale) == -1 ? Vector2{} : *reinterpret_cast<Vector2*>(data + m(uvMapScale)); }
  Vector2 UvMapOffset() const { return m(uvMapOffset) == -1 ? Vector2{} : *reinterpret_cast<Vector2*>(data + m(uvMapOffset)); }
  float UvMapRotation() const { return m(uvMapRotation) == -1 ? float{} : *reinterpret_cast<float*>(data + m(uvMapRotation)); }
  ::UVMappingAlgorithm UvMapAlgorithm() const { return m(uvMapAlgorithm) == -1 ? ::UVMappingAlgorithm{} : *reinterpret_cast<::UVMappingAlgorithm*>(data + m(uvMapAlgorithm)); }
  float SpecularMultiplier() const { return m(specularMultiplier) == -1 ? float{} : *reinterpret_cast<float*>(data + m(specularMultiplier)); }
  float SpecularExponent() const { return m(specularExponent) == -1 ? float{} : *reinterpret_cast<float*>(data + m(specularExponent)); }
  ::Transparency Transparency() const { return m(transparency) == -1 ? ::Transparency{} : *reinterpret_cast<::Transparency*>(data + m(transparency)); }
  uint64 UserData() const { return m(userData) == -1 ? uint64{} : *reinterpret_cast<uint64*>(data + m(userData)); }
  void NumStages(uint32 value) { if (m(numStages) >= 0) *reinterpret_cast<uint32*>(data + m(numStages)) = value; }
  void DiffuseColor(Vector4A16 value) { if (m(diffuseColor) >= 0) *reinterpret_cast<Vector4A16*>(data + m(diffuseColor)) = value; }
  void AmbientColor(Vector4A16 value) { if (m(ambientColor) >= 0) *reinterpret_cast<Vector4A16*>(data + m(ambientColor)) = value; }
  void SpecularColor(Vector4A16 value) { if (m(specularColor) >= 0) *reinterpret_cast<Vector4A16*>(data + m(specularColor)) = value; }
  void EmissiveColor(Vector4A16 value) { if (m(emissiveColor) >= 0) *reinterpret_cast<Vector4A16*>(data + m(emissiveColor)) = value; }
  void NumSubMaterials(uint32 value) { if (m(numSubMaterials) >= 0) *reinterpret_cast<uint32*>(data + m(numSubMaterials)) = value; }
  void NumProperties(uint32 value) { if (m(numProperties) >= 0) *reinterpret_cast<uint32*>(data + m(numProperties)) = value; }
  void UvMapScale(Vector2 value) { if (m(uvMapScale) >= 0) *reinterpret_cast<Vector2*>(data + m(uvMapScale)) = value; }
  void UvMapOffset(Vector2 value) { if (m(uvMapOffset) >= 0) *reinterpret_cast<Vector2*>(data + m(uvMapOffset)) = value; }
  void UvMapRotation(float value) { if (m(uvMapRotation) >= 0) *reinterpret_cast<float*>(data + m(uvMapRotation)) = value; }
  void UvMapAlgorithm(::UVMappingAlgorithm value) { if (m(uvMapAlgorithm) >= 0) *reinterpret_cast<::UVMappingAlgorithm*>(data + m(uvMapAlgorithm)) = value; }
  void SpecularMultiplier(float value) { if (m(specularMultiplier) >= 0) *reinterpret_cast<float*>(data + m(specularMultiplier)) = value; }
  void SpecularExponent(float value) { if (m(specularExponent) >= 0) *reinterpret_cast<float*>(data + m(specularExponent)) = value; }
  void Transparency(::Transparency value) { if (m(transparency) >= 0) *reinterpret_cast<::Transparency*>(data + m(transparency)) = value; }
  void UserData(uint64 value) { if (m(userData) >= 0) *reinterpret_cast<uint64*>(data + m(userData)) = value; }


  int16 m(uint32 id) const { return layout->vtable[id]; }
  char *data;
  const ClassData<_count_> *layout;
  LayoutLookup lookup;
};
}
