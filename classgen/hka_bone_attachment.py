from hk_base import *

hkaBoneAttachment = ClassData('hkaBoneAttachment')
hkaBoneAttachment.members = [
    ClassMember('boneFromAttachment', hkMatrix4),
    ClassMember('attachment', hkVariant),
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('boneIndex', TYPES.int16),
]
hkaBoneAttachment.patches = [
    ClassPatch('HK600',
        ClassPatchType.prepend,
            ClassMember('skeletonName', Pointer(TYPES.char))
    ),
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
        ClassPatchType.replace,
            ClassMember('attachment', Pointer(hkReferenceObject))
    ),
]

if __name__ == "__main__":
    CLASSES = [hkaBoneAttachment,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
