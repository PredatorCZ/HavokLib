from hk_base import *
from hkx_attribute_holder import hkxAttributeHolder

TextureType = NamedType('::TextureType', 4, 4, SwapType.S32)
UVMappingAlgorithm = NamedType('::UVMappingAlgorithm', 4, 4, SwapType.S32)
Transparency = NamedType('::Transparency', 1, 1)
hkxMaterialProperty = NamedType('::hkxMaterialProperty', 8, 4)

hkxMaterialTextureStage = ClassData('hkxMaterialTextureStage')
hkxMaterialTextureStage.members = [
    ClassMember('texture', hkVariant),
    ClassMember('usageHint', TextureType),
]
hkxMaterialTextureStage.patches = [
    ClassPatch('HK600', ClassPatchType.append, ClassMember('tcoordChannel', TYPES.int32)),
    ClassPatch('HK700', ClassPatchType.replace, ClassMember('texture', Pointer(hkReferenceObject))),
]

hkxMaterial = ClassData('hkxMaterial')
hkxMaterial.inherits = [
    hkxAttributeHolder
]
hkxMaterial.members = [
    ClassMember('name', Pointer(TYPES.char)),
    LegacyArray('stages', hkxMaterialTextureStage),
    ClassMember('diffuseColor', TYPES.Vector4A16),
    ClassMember('ambientColor', TYPES.Vector4A16),
    ClassMember('specularColor', TYPES.Vector4A16),
    ClassMember('emissiveColor', TYPES.Vector4A16),
    LegacyArray('subMaterials', Pointer(hkxMaterial)),
    ClassMember('extraData', hkVariant),
]

hkxMaterial.patches = [
    ClassPatch('HK700',
        ClassPatchType.replace,
            hkArray('stages', hkxMaterialTextureStage),
            hkArray('subMaterials', Pointer(hkxMaterial)),
            ClassMember('extraData', Pointer(hkReferenceObject)),
    ),
    ClassPatch('HK2010_1',
        ClassPatchType.append,
            hkArray('properties', hkxMaterialProperty)
    ),
    ClassPatch('HK2012_1',
        ClassPatchType.insert_after, 'extraData',
            ClassMember('uvMapScale', TYPES.Vector2),
            ClassMember('uvMapOffset', TYPES.Vector2),
            ClassMember('uvMapRotation', TYPES.float),
            ClassMember('uvMapAlgorithm', UVMappingAlgorithm),
            ClassMember('specularMultiplier', TYPES.float),
            ClassMember('specularExponent', TYPES.float),
            ClassMember('transparency', Transparency),
    ),
    ClassPatch('HK2013_2',
        ClassPatchType.insert_after, 'transparency',
            ClassMember('userData', TYPES.uint64),
    ),
]

if __name__ == "__main__":
    CLASSES = [hkxMaterialTextureStage, hkxMaterial,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hkx_attribute_holder.inl"')

    dump_classes(CLASSES)
