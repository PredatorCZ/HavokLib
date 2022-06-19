import sys
import os.path as pt
sys.path.append(pt.dirname(pt.abspath(__file__)) + '/..')

from hka_animation import *

USVector4 = NamedType('USVector4', 8, 2)

hkaLosslessCompressedAnimation = ClassData('hkaLosslessCompressedAnimation')
hkaLosslessCompressedAnimation.inherits = [
    hkaAnimation,
]
hkaLosslessCompressedAnimation.members = [
  hkArray('dynamicTranslations', TYPES.float),
  hkArray('staticTranslations', TYPES.float),
  hkArray('translationTypeAndOffsets', USVector4),
  hkArray('dynamicRotations', TYPES.Vector4A16),
  hkArray('staticRotations', TYPES.Vector4A16),
  hkArray('rotationTypeAndOffsets', TYPES.uint16),
  hkArray('dynamicScales', TYPES.float),
  hkArray('staticScales', TYPES.float),
  hkArray('scaleTypeAndOffsets', USVector4),
  hkArray('floats', TYPES.float),
  ClassMember('numFrames', TYPES.uint32),
]

BASE.permutators = ['HK2014']

if __name__ == "__main__":
    CLASSES = {
        'hkaLosslessCompressedAnimation': hkaLosslessCompressedAnimation,
    }
    print("// This file has been automatically generated. Do not modify.")
    print('#include "../hka_animation.inl"')

    for name, cls in CLASSES.items():
        print('namespace clgen::%s {' % name)
        print(
            cls.gen_enum_cpp())
        print(
            cls.gen_table_cpp(BASE))
        print(
            cls.gen_interface_cpp(BASE))
        print('}')
