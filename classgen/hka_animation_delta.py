from hka_animation import *

hkaQuantizationFormat = NamedType('QuantizationFormat', 20, 4)

hkaDeltaCompressedAnimation = ClassData('hkaDeltaCompressedAnimation')
hkaDeltaCompressedAnimation.inherits = [
    hkaAnimation,
]
hkaDeltaCompressedAnimation.members = [
    ClassMember('numberOfPoses', TYPES.uint32),
    ClassMember('blockSize', TYPES.uint32),
    ClassMember('qFormat', hkaQuantizationFormat),
    ClassMember('quantizedDataIdx', TYPES.uint32),
    ClassMember('quantizedDataSize', TYPES.uint32),
    ClassMember('staticMaskIdx', TYPES.uint32),
    ClassMember('staticMaskSize', TYPES.uint32),
    ClassMember('maskDOFsIdx', TYPES.uint32),
    ClassMember('maskDOFsSize', TYPES.uint32),
    ClassMember('totalBlockSize', TYPES.uint32),
    ClassMember('lastBlockSize', TYPES.uint32),
    LegacyArray('dataBuffer', TYPES.char),
]
hkaDeltaCompressedAnimation.patches = [
    ClassPatch('HK600', ClassPatchType.insert_after, 'maskDOFsSize', ClassMember('numStaticTransformDOFs', TYPES.uint32), ClassMember('numDynamicTransformDOFs', TYPES.uint32)),
    ClassPatch('HK700', ClassPatchType.replace,  hkArray('dataBuffer', TYPES.char)),
]

BASE.permutators = BASE.permutators[0:BASE.permutators.index('HK2011_1')]

if __name__ == "__main__":
    CLASSES = [hkaDeltaCompressedAnimation,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hka_animation.inl"')

    dump_classes(CLASSES)
