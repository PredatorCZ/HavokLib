from hk_base import *

hkaAnimationContainer = ClassData('hkaAnimationContainer')
hkaAnimationContainer.members = [
    LegacyArray('skeletons', Pointer(TYPES.char)),
    LegacyArray('animations', Pointer(TYPES.char)),
    LegacyArray('bindings', Pointer(TYPES.char)),
    LegacyArray('attachments', Pointer(TYPES.char)),
    LegacyArray('skins', Pointer(TYPES.char)),
]
hkaAnimationContainer.patches = [
    ClassPatch('HK700', ClassPatchType.append, Inheritable(hkReferenceObject), ClassPatchType.replace,
               hkArray('skeletons', Pointer(TYPES.char)),
               hkArray('animations', Pointer(TYPES.char)),
               hkArray('bindings', Pointer(TYPES.char)),
               hkArray('attachments', Pointer(TYPES.char)),
               hkArray('skins', Pointer(TYPES.char)),),
]

if __name__ == "__main__":
    CLASSES = [hkaAnimationContainer,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
