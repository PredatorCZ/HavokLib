from hka_animation import *

hkaSplineCompressedAnimation = ClassData('hkaSplineCompressedAnimation')
hkaSplineCompressedAnimation.inherits = [
    hkaAnimation,
]
hkaSplineCompressedAnimation.members = [
    ClassMember('numFrames', TYPES.uint32),
    ClassMember('numBlocks', TYPES.uint32),
    ClassMember('maxFramesPerBlock', TYPES.uint32),
    ClassMember('maskAndQuantizationSize', TYPES.uint32),
    ClassMember('blockDuration', TYPES.float),
    ClassMember('blockInverseDuration', TYPES.float),
    ClassMember('frameDuration', TYPES.float),
    hkArray('blockOffsets', TYPES.uint32),
    hkArray('floatBlockOffsets', TYPES.uint32),
    hkArray('transformBlockOffsets', TYPES.uint32),
    hkArray('floatOffsets', TYPES.uint32),
    hkArray('dataBuffer', TYPES.char),
]

if __name__ == "__main__":
    CLASSES = [hkaSplineCompressedAnimation,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hka_animation.inl"')

    dump_classes(CLASSES)
