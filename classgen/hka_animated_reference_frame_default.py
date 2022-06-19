from hka_animated_reference_frame import *

hkaDefaultAnimatedReferenceFrame = ClassData('hkaDefaultAnimatedReferenceFrame')
hkaDefaultAnimatedReferenceFrame.inherits = [
    hkaAnimatedReferenceFrame,
]
hkaDefaultAnimatedReferenceFrame.members = [
    ClassMember('up', TYPES.Vector4A16),
    ClassMember('forward', TYPES.Vector4A16),
    ClassMember('duration', TYPES.float),
    LegacyArray('referenceFrameSamples', TYPES.Vector4A16),
]

hkaDefaultAnimatedReferenceFrame.patches = [
    ClassPatch('HK700', ClassPatchType.replace, hkArray('referenceFrameSamples', TYPES.Vector4A16)),
]

if __name__ == "__main__":
    CLASSES = [hkaDefaultAnimatedReferenceFrame,]
    print("// This file has been automatically generated. Do not modify.")
    print('#pragma once')
    print('#include "hka_animated_reference_frame.inl"')

    dump_classes(CLASSES)
