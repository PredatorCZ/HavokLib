from hk_base import *

hkaAnimatedReferenceFrameType = NamedType('hkaAnimatedReferenceFrameType', 1, 1)

hkaAnimatedReferenceFrame = ClassData('hkaAnimatedReferenceFrame')
hkaAnimatedReferenceFrame.inherits = [
    hkReferenceObject,
]
hkaAnimatedReferenceFrame.patches = [
    ClassPatch('HK2012_1', ClassPatchType.append,  ClassMember('frameType', hkaAnimatedReferenceFrameType)),
]

if __name__ == "__main__":
    CLASSES = [hkaAnimatedReferenceFrame,]
    print("// This file has been automatically generated. Do not modify.")
    print('#pragma once')
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
