import sys
import os.path as pt
sys.path.append(pt.dirname(pt.abspath(__file__)) + '/../3rd_party/PreCore/classgen')
from classgen import *

BASE = MainSettings()
BASE.permutators = [
    'HK500',
    'HK510',
    'HK550',
    'HK600',
    'HK610',
    'HK650',
    'HK660',
    'HK700',
    'HK710',
    'HK2010_1',
    'HK2010_2',
    'HK2011_1',
    'HK2011_2',
    'HK2011_3',
    'HK2012_1',
    'HK2012_2',
    'HK2013',
    'HK2014',
    'HK2015',
    'HK2016',
    'HK2017'
]
BASE.pointer_x86 = True
BASE.pointer_x64 = True
BASE.class_layout_gnu = True
BASE.class_layout_msvc = True
BASE.ignore_pointer_endianness = True

hkQTransform = NamedType('hkQTransform', 48, 16)
hkLocalFrame = NamedType('hkLocalFrame')
esMatrix44 = NamedType('es::Matrix44', 64, 16)

hkVariant = ClassData('hkVariant')
hkVariant.members = [
    ClassMember('object', Pointer(TYPES.char)),
    ClassMember('classDesc', Pointer(TYPES.char)),
]

class LegacyArray():
    def __init__(self, name: str, type):
        self.name = name
        self.pointer = ClassMember(name, Pointer(type))
        self.counter = ClassMember(
            'num' + name[0].upper() + name[1:], TYPES.uint32)

    def get_location(self, cur_offset, settings: PermSettings):
        loc = self.pointer.get_location(cur_offset, settings)
        return self.counter.get_location(loc.end, settings)

    def append_offsets(self, offsets: list, cur_offset: int, member_names: list, settings: MainSettings):
        data_loc = self.pointer.type.get_location(cur_offset, settings)
        count_loc = self.counter.type.get_location(data_loc.end, settings)
        offsets[member_names.index(self.name)] = data_loc.begin
        offsets[member_names.index(self.counter.name)] = count_loc.begin

        return offsets, count_loc.end

    def append_names(self, items: list):
        if not self.name in items:
            items.append(self.name)
        if not self.counter.name in items:
            items.append(self.counter.name)
        return items

    def append_swaps(self, swaps: list, member_names: list, settings: PermSettings):
        swaps = self.pointer.append_swaps(swaps, member_names, settings)
        return self.counter.append_swaps(swaps, member_names, settings)

    def cpp_method(self, context):
        self.pointer.perm = self.perm
        self.pointer.cpp_method(context)
        self.counter.perm = self.perm
        self.counter.cpp_method(context)

    def str_decl(self):
        return '%s\n  %s' % (self.pointer.str_decl(), self.counter.str_decl())

class hkArray(LegacyArray):
    sub = ClassData('hkArray')
    sub.templates = [Template('Type')]
    sub.members = [
        ClassMember('data', Pointer(sub.templates[0])),
        ClassMember('count', TYPES.uint32),
        ClassMember('capacityAndFlags', TYPES.uint32)
    ]
    def get_location(self, cur_offset, settings: PermSettings):
        return self.sub.get_location(cur_offset, settings)
    def append_offsets(self, offsets: list, cur_offset: int, member_names: list, settings: MainSettings):
        offsets, cur_offset = super(hkArray, self).append_offsets(offsets, cur_offset, member_names, settings)
        flags_loc = TYPES.uint32.get_location(cur_offset, settings)

        return offsets, flags_loc.end

hkReferenceObject = ClassData('hkReferenceObject')
hkReferenceObject.members = [
    ClassMember('vtable', Pointer(TYPES.int32)),
    ClassMember('memSizeAndFlags', TYPES.int16),
    ClassMember('referenceCount', TYPES.int16),
]
hkReferenceObject.patches = [
    ClassPatch('HK2016', ClassPatchType.insert_after, 'vtable',
               ClassMember('unk', Pointer(TYPES.int32))),
]

def dump_classes(CLASSES):
    for cls in CLASSES:
        print('namespace clgen::%s {' % cls.name)
        print(
            cls.gen_enum_cpp())
        print(
            cls.gen_table_cpp(BASE))
        print(
            cls.gen_interface_cpp(BASE))
        print('}')

if __name__ == "__main__":
    BASE.class_layout_gnu = False
    CLASSES = [hkVariant,hkReferenceObject,]
    print('// This file has been automatically generated. Do not modify.')
    print('#include "classgen/classgen.hpp"')
    dump_classes(CLASSES)
