from hka_animation import *

hkaQuantizationFormat = NamedType('QuantizationFormat', 20, 4)

hkaWaveletCompressedAnimation = ClassData('hkaWaveletCompressedAnimation')
hkaWaveletCompressedAnimation.inherits = [
    hkaAnimation,
]

hkaWaveletCompressedAnimation.members = [
    ClassMember('numberOfPoses', TYPES.uint32),
    ClassMember('blockSize', TYPES.uint32),
    ClassMember('qFormat', hkaQuantizationFormat),
    ClassMember('staticMaskIdx', TYPES.uint32),
    ClassMember('staticDOFsIdx', TYPES.uint32),
    ClassMember('blockIndexIdx', TYPES.uint32),
    ClassMember('blockIndexSize', TYPES.uint32),
    ClassMember('quantizedDataIdx', TYPES.uint32),
    ClassMember('quantizedDataSize', TYPES.uint32),
    LegacyArray('dataBuffer', TYPES.char),
]
hkaWaveletCompressedAnimation.patches = [
    ClassPatch('HK600', ClassPatchType.insert_after, 'staticDOFsIdx', ClassMember('numStaticTransformDOFs', TYPES.uint32), ClassMember('numDynamicTransformDOFs', TYPES.uint32)),
    ClassPatch('HK700', ClassPatchType.replace,  hkArray('dataBuffer', TYPES.char)),
]

BASE.permutators = BASE.permutators[0:BASE.permutators.index('HK2011_1')]

if __name__ == "__main__":
    CLASSES = [hkaWaveletCompressedAnimation,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hka_animation.inl"')

    dump_classes(CLASSES)
