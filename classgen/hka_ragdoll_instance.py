from hk_base import *

hkpRigidBodyClass = NamedType('hkpRigidBodyClass')
hkpConstraintInstance = NamedType('hkpConstraintInstance')
hkaSkeleton = NamedType('hkaSkeleton')

hkaRagdollInstance = ClassData('hkaRagdollInstance')
hkaRagdollInstance.inherits = [
    hkReferenceObject,
]
hkaRagdollInstance.members = [
    hkArray('rigidBodies', Pointer(hkpRigidBodyClass)),
    hkArray('constraints', Pointer(hkpConstraintInstance)),
    hkArray('boneToRigidBodyMap', TYPES.int32),
    ClassMember('skeleton', Pointer(hkaSkeleton)),
]

BASE.permutators = BASE.permutators[0:BASE.permutators.index('HK2015')]

if __name__ == "__main__":
    CLASSES = [hkaRagdollInstance,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
