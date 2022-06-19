from hka_animated_reference_frame import *

hkaAnnotation = ClassData('hkaAnnotation')
hkaAnnotation.members = [
    ClassMember('time', TYPES.float),
    ClassMember('text', Pointer(TYPES.char)),
]

hkaAnnotationTrack = ClassData('hkaAnnotationTrack')
hkaAnnotationTrack.members = [
    ClassMember('name', Pointer(TYPES.char)),
    LegacyArray('annotations', hkaAnnotation),
]
hkaAnnotationTrack.patches = [
    ClassPatch('HK700', ClassPatchType.replace, hkArray('annotations', hkaAnnotation)),
]

hkaAnimation = ClassData('hkaAnimation')
hkaAnimation.inherits = [
    hkReferenceObject,
]
hkaAnimation.members = [
    ClassMember('animationType', TYPES.uint32),
    ClassMember('duration', TYPES.float),
    ClassMember('numOfTransformTracks', TYPES.uint32),
    ClassMember('extractedMotion', Pointer(hkaAnimatedReferenceFrame)),
    LegacyArray('annotations', Pointer(hkaAnnotationTrack)),
]
hkaAnimation.patches = [
    ClassPatch('HK550', ClassPatchType.insert_after, "numOfTransformTracks", ClassMember('numOfFloatTracks', TYPES.uint32)),
    ClassPatch('HK700', ClassPatchType.replace, hkArray('annotations', hkaAnnotationTrack)),
]

if __name__ == "__main__":
    CLASSES = [hkaAnnotation, hkaAnnotationTrack, hkaAnimation,]
    print("// This file has been automatically generated. Do not modify.")
    print('#pragma once')
    print('#include "hka_animated_reference_frame.inl"')

    dump_classes(CLASSES)
