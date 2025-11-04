from hk_base import *
from hkx_attribute_holder import hkxAttributeHolder

LightType = NamedType('::LightType', 1, 1)
hkxCameraData = NamedType('::hkxCamera', 64, 16)
hkxSplineControlPoint = NamedType('::hkxSplineControlPoint', 64, 16)

hkxNodeAnnotationData = ClassData('hkxNodeAnnotationData')
hkxNodeAnnotationData.members = [
    ClassMember('time', TYPES.float),
    ClassMember('description', Pointer(TYPES.char)),
]

hkxNode = ClassData('hkxNode')
hkxNode.inherits = [
    hkxAttributeHolder
]
hkxNode.members = [
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('object', hkVariant),
    LegacyArray('keyFrames', hkMatrix4),
    LegacyArray('children', Pointer(hkxNode)),
    LegacyArray('annotations', hkxNodeAnnotationData),
    ClassMember('userProperties', Pointer(TYPES.char)),
    ClassMember('selected', TYPES.bool),
]
hkxNode.patches = [
    ClassPatch('HK700',
        ClassPatchType.replace,
            ClassMember('object', Pointer(hkReferenceObject)),
            hkArray('keyFrames', hkMatrix4),
            hkArray('children', Pointer(hkxNode)),
            hkArray('annotations', hkxNodeAnnotationData),
    ),
    ClassPatch('HK2012_1',
        ClassPatchType.insert_after,
            'annotations',
            hkArray('linearKeyFrameHints', TYPES.float),
        ClassPatchType.insert_after,
            'selected',
            hkArray('bone', TYPES.bool)
    ),
]

hkxNodeSelectionSet = ClassData('hkxNodeSelectionSet')
hkxNodeSelectionSet.inherits = [
    hkxAttributeHolder
]
hkxNodeSelectionSet.members = [
    LegacyArray('selectedNodes', Pointer(hkxNode)),
    ClassMember('name', Pointer(TYPES.char)),
]
hkxNodeSelectionSet.patches = [
    ClassPatch('HK700',
        ClassPatchType.replace,
            hkArray('selectedNodes', Pointer(hkxNode)),
    ),
]

hkxCamera = ClassData('hkxCamera')
hkxCamera.members = [
    ClassMember('data_', InlineArray(hkxCameraData, 1)),
]
hkxCamera.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
    ),
]

hkxLight = ClassData('hkxLight')
hkxLight.members = [
    ClassMember('type', LightType),
    ClassMember('position', TYPES.Vector4A16),
    ClassMember('direction', TYPES.Vector4A16),
    ClassMember('color', TYPES.uint32),
    ClassMember('angle', TYPES.float),
]
hkxLight.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
    ),
    ClassPatch('HK2012_1',
        ClassPatchType.append,
            ClassMember('range', TYPES.float),
            ClassMember('fadeStart', TYPES.float),
            ClassMember('fadeEnd', TYPES.float),
            ClassMember('intensity', TYPES.float),
            ClassMember('shadowCaster', TYPES.bool),
    ),
]

hkxTextureInplace = ClassData('hkxTextureInplace')
hkxTextureInplace.members = [
    ClassMember('fileType', InlineArray(TYPES.char, 4)),
    LegacyArray('data_', TYPES.uint8),
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('originalFilename', Pointer(TYPES.char)),
]
hkxTextureInplace.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
        ClassPatchType.replace,
            hkArray('data_', TYPES.uint8),
    ),
]

hkxTextureFile = ClassData('hkxTextureFile')
hkxTextureFile.members = [
    ClassMember('fileName', Pointer(TYPES.char)),
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('originalFilename', Pointer(TYPES.char)),
]
hkxTextureFile.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
    ),
]

hkxSkinBinding = ClassData('hkxSkinBinding')
hkxSkinBinding.members = [
    ClassMember('mesh', Pointer(hkReferenceObject)),
    LegacyArray('mapping', Pointer(hkxNode)),
    LegacyArray('bindPose', hkMatrix4),
    ClassMember('initSkinTransform', hkMatrix4),
]
hkxSkinBinding.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
        ClassPatchType.replace,
            hkArray('mapping', Pointer(hkxNode)),
            hkArray('bindPose', hkMatrix4),
    ),
    ClassPatch('HK710',
        ClassPatchType.delete,
            'mapping',
        ClassPatchType.insert_after,
            'mesh',
            hkArray('nodeNames', Pointer(TYPES.char))
    ),
]

hkxSpline = ClassData('hkxSpline')
hkxSpline.inherits = [
    hkReferenceObject,
]
hkxSpline.members = [
    hkArray('controlPoints', hkxSplineControlPoint),
    ClassMember('isClosed', TYPES.bool),
]

hkxScene = ClassData('hkxScene')
hkxScene.members = [
    ClassMember('modeller', Pointer(TYPES.char)),
    ClassMember('asset', Pointer(TYPES.char)),
    ClassMember('sceneLength', TYPES.float),
    ClassMember('rootNode', Pointer(hkxNode)),
    LegacyArray('selectionSets', Pointer(hkxNodeSelectionSet)),
    LegacyArray('cameras', Pointer(hkxCamera)),
    LegacyArray('lights', Pointer(hkxLight)),
    LegacyArray('meshes', Pointer(hkReferenceObject)),
    LegacyArray('materials', Pointer(hkReferenceObject)),
    LegacyArray('inplaceTextures', Pointer(hkxTextureInplace)),
    LegacyArray('externalTextures', Pointer(hkxTextureFile)),
    LegacyArray('skinBindings', Pointer(hkxSkinBinding)),
    ClassMember('appliedTransform', hkMatrix3),
]
hkxScene.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
        ClassPatchType.replace,
            hkArray('selectionSets', Pointer(hkxNodeSelectionSet)),
            hkArray('cameras', Pointer(hkxCamera)),
            hkArray('lights', Pointer(hkxLight)),
            hkArray('meshes', Pointer(hkReferenceObject)),
            hkArray('materials', Pointer(hkReferenceObject)),
            hkArray('inplaceTextures', Pointer(hkxTextureInplace)),
            hkArray('externalTextures', Pointer(hkxTextureFile)),
            hkArray('skinBindings', Pointer(hkxSkinBinding)),
    ),
    ClassPatch('HK2012_1',
        ClassPatchType.insert_after,
            'sceneLength',
            ClassMember('numFrames', TYPES.uint32),
        ClassPatchType.insert_after,
            'skinBindings',
            hkArray('splines', Pointer(hkxSpline)),
    ),
]

if __name__ == "__main__":
    CLASSES = [hkxSpline, hkxNodeAnnotationData, hkxNode, hkxNodeSelectionSet, hkxCamera, hkxLight, hkxTextureInplace, hkxTextureFile, hkxSkinBinding, hkxScene,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hk_base.inl"')

    dump_classes(CLASSES)
