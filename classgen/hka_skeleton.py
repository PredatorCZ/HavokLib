from hk_base import *

hkaBone = ClassData('hkaBone')
hkaBone.members = [
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('lockTranslation', TYPES.int32),
]

hkLocalFrameOnBone = ClassData('hkLocalFrameOnBone')
hkLocalFrameOnBone.members = [
    ClassMember('localFrame', Pointer(hkLocalFrame)),
    ClassMember('boneIndex', TYPES.int32),
]
hkLocalFrameOnBone.patches = [
    ClassPatch('HK2011_1', ClassPatchType.replace,
               ClassMember('boneIndex', TYPES.int16)),
]

hkaPartition = ClassData('hkaPartition')
hkaPartition.members = [
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('startBoneIndex', TYPES.int16),
    ClassMember('numBones', TYPES.uint16),
]

hkaSkeleton = ClassData('hkaSkeleton')
hkaSkeleton.members = [
    ClassMember('name', Pointer(TYPES.char)),
    LegacyArray('parentIndices', TYPES.int16),
    LegacyArray('bones', Pointer(hkaBone)),
    LegacyArray('transforms', hkQTransform),
]

hkaSkeleton.patches = [
    ClassPatch('HK550', ClassPatchType.append, LegacyArray(
        'floatSlots', Pointer(TYPES.char))),
    ClassPatch('HK600', ClassPatchType.append, LegacyArray(
        'localFrames', hkLocalFrameOnBone)),
    ClassPatch('HK700', ClassPatchType.replace,
               hkArray('parentIndices', TYPES.int16),
               hkArray('bones', hkaBone),
               hkArray('transforms', hkQTransform),
               hkArray('floatSlots', Pointer(TYPES.char)),
               hkArray('localFrames', hkLocalFrameOnBone),
               ClassPatchType.append, Inheritable(hkReferenceObject)
               ),
    ClassPatch('HK2010_1', ClassPatchType.insert_after, 'transforms', hkArray(
        'referenceFloats', TYPES.float)),
    ClassPatch('HK2012_1', ClassPatchType.append, hkArray(
        'partitions', hkaPartition)),
]

if __name__ == "__main__":
    CLASSES = [hkaBone, hkLocalFrameOnBone, hkaPartition, hkaSkeleton,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
