from hk_base import *
from hkx_attribute_holder import hkxAttributeHolder

IndexType = NamedType('::IndexType', 1, 1)
DataType = NamedType('::DataType', 2, 2, SwapType.S16)
DataUsage = NamedType('::DataUsage', 2, 2, SwapType.S16)
hkxVertexAnimationUsageMap = NamedType('::hkxVertexAnimationUsageMap', 4, 2)
hkxVertexFormat = NamedType('::hkxVertexFormat', 12, 1)
hkxVertexDescriptionDataHint = NamedType('::hkxVertexDescriptionDataHint', 2, 2, SwapType.S16)

hkxVertexDescriptionElementDecl = ClassData('hkxVertexDescriptionElementDecl')
hkxVertexDescriptionElementDecl.members = [
    ClassMember('byteOffset', TYPES.uint32),
    ClassMember('type', DataType),
    ClassMember('usage', DataUsage),
]
hkxVertexDescriptionElementDecl.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            ClassMember('byteStride', TYPES.uint32),
    ),
    ClassPatch('HK2010_2',
        ClassPatchType.append,
            ClassMember('numElements', TYPES.uint8),
    ),
    ClassPatch('HK2012_2',
        ClassPatchType.append,
            ClassMember('hint', hkxVertexDescriptionDataHint),
    ),
    ClassPatch('HK2014',
        ClassPatchType.delete,
            'hint',
        ClassPatchType.append,
            ClassMember('channelID', Pointer(TYPES.char)),
    )
]

hkxVertexDescription = ClassData('hkxVertexDescription')
hkxVertexDescription.members = [
    ClassMember('stride', TYPES.uint32),
    hkArray('decls', hkxVertexDescriptionElementDecl),
]
hkxVertexDescription.patches = [
    ClassPatch('HK700',
        ClassPatchType.delete,
            'stride',
    ),
]

hkxVertexBufferVertexData = ClassData('hkxVertexBufferVertexData')
hkxVertexBufferVertexData.members = [
    hkArray('vectorData', TYPES.Vector4),
    hkArray('floatData', TYPES.float),
    hkArray('uint32Data', TYPES.uint32),
    hkArray('uint16Data', TYPES.uint16),
    hkArray('uint8Data', TYPES.uint8),
    ClassMember('numVerts', TYPES.uint32),
    ClassMember('vectorStride', TYPES.uint32),
    ClassMember('floatStride', TYPES.uint32),
    ClassMember('uint32Stride', TYPES.uint32),
    ClassMember('uint16Stride', TYPES.uint32),
    ClassMember('uint8Stride', TYPES.uint32),
]
hkxVertexBufferVertexData.patches = [
    ClassPatch('HK2013_1',
        ClassPatchType.replace,
            hkArray('vectorData', TYPES.float),
    ),
]

hkxVertexBuffer = ClassData('hkxVertexBuffer')
hkxVertexBuffer.members = [
    ClassMember('vertexDataClass', Pointer(TYPES.int8)),
    LegacyArray('vertexData', TYPES.uint8),
    ClassMember('format', Pointer(hkxVertexFormat)),
]
hkxVertexBuffer.patches = [
    ClassPatch('HK600',
        ClassPatchType.delete,
            'format',
        ClassPatchType.append,
            ClassMember('vertexDesc', Pointer(hkxVertexDescription)),
    ),
    ClassPatch('HK700',
        ClassPatchType.delete,
            'vertexDataClass',
            'vertexData',
            'vertexDesc',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
            ClassMember('data_', hkxVertexBufferVertexData),
            ClassMember('desc', hkxVertexDescription),
    ),
]

hkxIndexBuffer = ClassData('hkxIndexBuffer')
hkxIndexBuffer.members = [
    ClassMember('indexType', IndexType),
    LegacyArray('indices16', TYPES.uint16),
    LegacyArray('indices32', TYPES.uint32),
    ClassMember('vertexBaseOffset', TYPES.uint32),
    ClassMember('length', TYPES.uint32),
]
hkxIndexBuffer.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
        ClassPatchType.replace,
            hkArray('indices16', TYPES.uint16),
            hkArray('indices32', TYPES.uint32),
    ),
]

hkxVertexAnimation = ClassData('hkxVertexAnimation')
hkxVertexAnimation.inherits = [
    hkReferenceObject,
]
hkxVertexAnimation.members = [
    ClassMember('time', TYPES.float),
    ClassMember('vertData', hkxVertexBuffer),
    hkArray('vertexIndexMap', TYPES.int32),
    hkArray('componentMap', hkxVertexAnimationUsageMap),
]

hkMeshBoneIndexMapping = ClassData('hkMeshBoneIndexMapping')
hkMeshBoneIndexMapping.members = [
    hkArray('mapping', TYPES.int16),
]

hkxMeshSection = ClassData('hkxMeshSection')
hkxMeshSection.members = [
    ClassMember('vertexBuffer', Pointer(hkxVertexBuffer)),
    LegacyArray('indexBuffers', Pointer(hkxIndexBuffer)),
    ClassMember('material', Pointer(hkReferenceObject)),
    LegacyArray('userChannels', hkVariant),
]
hkxMeshSection.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
        ClassPatchType.replace,
            hkArray('indexBuffers', Pointer(hkxIndexBuffer)),
            hkArray('userChannels', Pointer(hkReferenceObject)),
    ),
    ClassPatch('HK2012_1',
        ClassPatchType.append,
            hkArray('vertexAnimations', Pointer(hkxVertexAnimation)),
    ),
    ClassPatch('HK2012_2',
        ClassPatchType.append,
            hkArray('linearKeyFrameHints', TYPES.float),
    ),
    ClassPatch('HK2014',
        ClassPatchType.append,
            hkArray('boneMatrixMap', hkMeshBoneIndexMapping),
    ),
]

hkxMeshUserChannelInfo = ClassData('hkxMeshUserChannelInfo')
hkxMeshUserChannelInfo.inherits = [
    hkxAttributeHolder
]
hkxMeshUserChannelInfo.members = [
    ClassMember('name', Pointer(TYPES.char)),
    ClassMember('className', Pointer(TYPES.char)),
]

hkxMesh = ClassData('hkxMesh')
hkxMesh.members = [
    LegacyArray('sections', Pointer(hkxMeshSection)),
    LegacyArray('userChannelInfos', hkxMeshUserChannelInfo),
]
hkxMesh.patches = [
    ClassPatch('HK700',
        ClassPatchType.append,
            Inheritable(hkReferenceObject),
        ClassPatchType.replace,
            hkArray('sections', Pointer(hkxMeshSection)),
            hkArray('userChannelInfos', Pointer(hkxMeshUserChannelInfo)),
    ),
]

if __name__ == "__main__":
    CLASSES = [hkxVertexDescriptionElementDecl, hkxVertexDescription, hkxVertexBufferVertexData,
               hkxVertexBuffer, hkxIndexBuffer, hkxVertexAnimation, hkMeshBoneIndexMapping, hkxMeshSection,
               hkxMeshUserChannelInfo, hkxMesh,]
    print("// This file has been automatically generated. Do not modify.")
    print('#include "hkx_vertexformat.hpp"')
    print('#include "hkx_attribute_holder.inl"')

    dump_classes(CLASSES)
