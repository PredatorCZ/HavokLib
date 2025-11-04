from hk_base import *

hkClassMemberType = NamedType('hkClassMemberType', 1, 1)

hkClass = ClassData('hkClass')

hkClassMember = ClassData('hkClassMember')
hkClassMember.members = [
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('class_', Pointer(hkClass)),
    ClassMember('enum_', Pointer(TYPES.int8)), # TODO hkClassEnum
    ClassMember('type', hkClassMemberType),
    ClassMember('subtype', hkClassMemberType),
    ClassMember('cArraySize', TYPES.int16),
    ClassMember('flags', TYPES.uint16),
    ClassMember('offset', TYPES.uint16),
    ClassMember('attributes', Pointer(TYPES.int8)),
]

hkClass.members = [
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('parent', Pointer(hkClass)),
    ClassMember('objectSize', TYPES.int32),
    ClassMember('numImplementedInterfaces', TYPES.int32),
    LegacyArray('declaredEnums', TYPES.int8), # TODO hkClassEnum
    LegacyArray('declaredMembers', hkClassMember),
    ClassMember('defaults', Pointer(TYPES.int8)),
    ClassMember('attributes', Pointer(TYPES.int8)),
    ClassMember('flags', TYPES.uint32),
    ClassMember('describedVersion', TYPES.int32),
]

BASE.permutators = ['HK660']

if __name__ == "__main__":
    CLASSES = [hkClassMember, hkClass,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
