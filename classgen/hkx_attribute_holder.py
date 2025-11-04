from hk_base import *

hkxAttribute = ClassData('hkxAttribute')
hkxAttribute.members = [
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('value', hkVariant),
]
hkxAttribute.patches = [
    ClassPatch('HK700', ClassPatchType.replace, ClassMember('value', Pointer(hkReferenceObject))),
]

hkxAttributeGroup = ClassData('hkxAttributeGroup')
hkxAttributeGroup.members = [
    ClassMember('name', Pointer(TYPES.char)),
    LegacyArray('attributes', hkxAttribute),
]
hkxAttributeGroup.patches = [
    ClassPatch('HK700', ClassPatchType.replace, hkArray('attributes', hkxAttribute)),
]

hkxAttributeHolder = ClassData('hkxAttributeHolder')
hkxAttributeHolder.members = [
    LegacyArray('attributeGroups', hkxAttributeGroup)
]
hkxAttributeHolder.patches = [
    ClassPatch('HK700', ClassPatchType.append, Inheritable(hkReferenceObject), ClassPatchType.replace, hkArray('attributeGroups', hkxAttributeGroup)),
]

if __name__ == "__main__":
    CLASSES = [hkxAttribute, hkxAttributeGroup, hkxAttributeHolder,]
    print("// This file has been automatically generated. Do not modify.")
    print('#pragma once')
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
