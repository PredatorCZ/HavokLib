from hk_base import *

blendHint = NamedType('::BlendHint', 1, 1)
hkaAnimation = NamedType('hkaAnimation')

hkaAnimationBinding = ClassData('hkaAnimationBinding')
hkaAnimationBinding.members = [
    ClassMember('animation', Pointer(hkaAnimation)),
    LegacyArray('transformTrackToBoneIndices', TYPES.int16),
    ClassMember('blendHint', blendHint),
]
hkaAnimationBinding.patches = [
    ClassPatch('HK550', ClassPatchType.insert_after, "transformTrackToBoneIndices",
               LegacyArray('floatTrackToFloatSlotIndices', TYPES.int16)),
    ClassPatch('HK600', ClassPatchType.prepend, ClassMember(
        'skeletonName', Pointer(TYPES.char))),
    ClassPatch('HK700', ClassPatchType.append, Inheritable(hkReferenceObject), ClassPatchType.replace, hkArray(
        'transformTrackToBoneIndices', TYPES.int16), hkArray('floatTrackToFloatSlotIndices', TYPES.int16)),
    ClassPatch('HK2012_1', ClassPatchType.insert_after,
               'floatTrackToFloatSlotIndices', hkArray('partitionIndices', TYPES.int16)),
]

if __name__ == "__main__":
    CLASSES = [hkaAnimationBinding,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
