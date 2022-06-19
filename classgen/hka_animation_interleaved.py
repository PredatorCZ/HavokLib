from hka_animation import *

hkaInterleavedAnimation = ClassData('hkaInterleavedAnimation')
hkaInterleavedAnimation.inherits = [
    hkaAnimation,
]
hkaInterleavedAnimation.members = [
    LegacyArray('transforms', hkQTransform),
]
hkaInterleavedAnimation.patches = [
    ClassPatch('HK550', ClassPatchType.append,
               LegacyArray('floats', TYPES.float)),
    ClassPatch('HK700', ClassPatchType.replace, hkArray(
        'transforms', hkQTransform), hkArray('floats', TYPES.float))
]

if __name__ == "__main__":
    CLASSES = [hkaInterleavedAnimation,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hka_animation.inl"')

    dump_classes(CLASSES)
