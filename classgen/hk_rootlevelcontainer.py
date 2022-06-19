from hk_base import *

hkNamedVariant = ClassData('hkNamedVariant')
hkNamedVariant.members = [
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('className', Pointer(TYPES.char)),
    ClassMember('variant', hkVariant),
]
hkNamedVariant.patches = [
    ClassPatch('HK700', ClassPatchType.replace, ClassMember('variant', Pointer(TYPES.char))),
]

hkRootLevelContainer = ClassData('hkRootLevelContainer')
hkRootLevelContainer.members = [
    LegacyArray('variants', hkNamedVariant)
]
hkRootLevelContainer.patches = [
    ClassPatch('HK700', ClassPatchType.replace, hkArray('variants', hkNamedVariant))
]

if __name__ == "__main__":
    BASE.class_layout_gnu = False
    CLASSES = [hkNamedVariant, hkRootLevelContainer,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hk_base.inl"')
    dump_classes(CLASSES)
