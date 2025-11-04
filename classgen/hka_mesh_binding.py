from hk_base import *

hkaMeshBindingMapping = ClassData('hkaMeshBindingMapping')
hkaMeshBindingMapping.members = [
    LegacyArray('mapping', TYPES.int16),
]
hkaMeshBindingMapping.patches = [
    ClassPatch('HK700',
        ClassPatchType.replace,
            hkArray('mapping', TYPES.int16),
    ),
]

hkaMeshBinding = ClassData('hkaMeshBinding')
hkaMeshBinding.members = [
    ClassMember('mesh', Pointer(hkReferenceObject)),
    ClassMember('skeleton', Pointer(hkReferenceObject)),
    LegacyArray('mappings', hkaMeshBindingMapping), # int16
    LegacyArray('boneFromSkinMeshTransforms', hkMatrix4),
]
hkaMeshBinding.patches = [
    ClassPatch('HK600',
        ClassPatchType.insert_after,
            'mesh',
            ClassMember('skeletonName', Pointer(TYPES.char))
    ),
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
        ClassPatchType.replace,
            hkArray('mappings', hkaMeshBindingMapping),
            hkArray('boneFromSkinMeshTransforms', hkMatrix4)
    ),
    ClassPatch('HK2011_1',
        ClassPatchType.insert_after,
            'skeletonName',
            ClassMember('name', Pointer(TYPES.char))
    ),
]

if __name__ == "__main__":
    CLASSES = [hkaMeshBindingMapping, hkaMeshBinding,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
