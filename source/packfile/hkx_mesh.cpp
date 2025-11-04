/*  Havok Format Library
    Copyright(C) 2016-2025 Lukas Cone

    This program is free software : you can redistribute it and / or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see <https://www.gnu.org/licenses/>.
*/

#include "internal/hkx_mesh.hpp"
#include "base.hpp"
#include "hklib/hkx_material.hpp"

#include "hkx_mesh.inl"

template <>
void FByteswapper(clgen::hkxVertexBuffer::Interface &interface, bool) {
  clgen::EndianSwap(interface);

  if (interface.LayoutVersion() < HK600) {
    const hkxVertexFormat &format = *interface.Format();
    uint8 *data = interface.VertexData();
    const uint32 numVertices = interface.NumVertexData();
    const uint32 stride = format.stride;

    if (format.positionOffset < 0xff) {
      for (uint32 i = 0; i < numVertices; i++) {
        FByteswapper(*reinterpret_cast<Vector4 *>(data + stride * i));
      }

      data += 16;
    }

    if (format.normalOffset < 0xff) {
      for (uint32 i = 0; i < numVertices; i++) {
        FByteswapper(*reinterpret_cast<Vector4 *>(data + stride * i));
      }

      data += 16;
    }

    if (format.tangentOffset < 0xff) {
      for (uint32 i = 0; i < numVertices; i++) {
        FByteswapper(*reinterpret_cast<Vector4 *>(data + stride * i));
      }

      data += 16;
    }

    if (format.binormalOffset < 0xff) {
      for (uint32 i = 0; i < numVertices; i++) {
        FByteswapper(*reinterpret_cast<Vector4 *>(data + stride * i));
      }

      data += 16;
    }

    if (format.numBonesPerVertex > 0) {
      if (format.boneIndexOffset < 0xff) {
        data += 4;
      }

      if (format.boneWeightOffset < 0xff) {
        data += 4;
      }
    }

    if (format.colorOffset < 0xff) {
      data += 4;
    }

    for (uint8 t = 0; t < format.numTextureChannels; t++) {
      if (format.tFloatCoordOffset < 0xff) {
        for (uint32 i = 0; i < numVertices; i++) {
          FByteswapper(*reinterpret_cast<Vector2 *>(data + stride * i));
        }

        data += 8;
      } else if (format.tQuantizedCoordOffset < 0xff) {
        for (uint32 i = 0; i < numVertices; i++) {
          FByteswapper(*reinterpret_cast<USVector2 *>(data + stride * i));
        }

        data += 4;
      }
    }
  }

  if (interface.LayoutVersion() >= HK700) {
    auto vdata = interface.Data_();
    clgen::EndianSwap(vdata);

    float *floatData = vdata.FloatData();
    uint32 numItems = vdata.NumFloatData();

    for (uint32 i = 0; i < numItems; i++) {
      FByteswapper(floatData[i]);
    }

    uint16 *uint16Data = vdata.Uint16Data();
    numItems = vdata.NumUint16Data();

    for (uint32 i = 0; i < numItems; i++) {
      FByteswapper(uint16Data[i]);
    }

    uint32 *uint32Data = vdata.Uint32Data();
    numItems = vdata.NumUint32Data();

    for (uint32 i = 0; i < numItems; i++) {
      FByteswapper(uint32Data[i]);
    }

    numItems = vdata.NumVectorData();

    if (interface.LayoutVersion() >= HK2013_1) {
      float *vecData = vdata.VectorDataHK2013_1();

      for (uint32 i = 0; i < numItems; i++) {
        FByteswapper(vecData[i]);
      }
    } else {
      Vector4 *vecData = vdata.VectorData();

      for (uint32 i = 0; i < numItems; i++) {
        FByteswapper(vecData[i]);
      }
    }
  }
}

struct hkxIndexBufferMidInterface : hkxIndexBufferInternalInterface {
  clgen::hkxIndexBuffer::Interface interface;

  hkxIndexBufferMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxIndexBufferMidInterface(const clgen::hkxIndexBuffer::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    clgen::EndianSwap(interface);

    uint32 *indices32 = interface.Indices32();
    const uint32 numIndices32 = interface.NumIndices32();
    for (uint32 i = 0; i < numIndices32; i++) {
      FByteswapper(indices32[i]);
    }

    uint16 *indices16 = interface.Indices16();
    const uint32 numIndices16 = interface.NumIndices16();
    for (uint32 i = 0; i < numIndices16; i++) {
      FByteswapper(indices16[i]);
    }
  }

  IndexType GetIndexType() const override { return interface.IndexType(); }

  size_t NumIndices() const override {
    return interface.NumIndices16() + interface.NumIndices32();
  }

  bool Is32Bit() const override { return interface.NumIndices32() > 0; }

  const char *Indices() const override {
    const uint16 *indices16 = interface.Indices16();
    const uint32 *indices32 = interface.Indices32();

    if (indices16) {
      return reinterpret_cast<const char *>(indices16);
    }

    return reinterpret_cast<const char *>(indices32);
  }

  uint32 VerrtexBaseOffset() const override {
    return interface.VertexBaseOffset();
  }

  uint32 Length() const override { return interface.Length(); }
};

struct hkxVertexDescriptionMidInterface
    : hkxVertexDescriptionInternalInterface {
  clgen::hkxVertexDescription::Interface interface;

  hkxVertexDescriptionMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxVertexDescriptionMidInterface(
      const clgen::hkxVertexDescription::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  size_t GetNumDecls() const override { return interface.NumDecls(); }

  void SwapEndian() override {
    clgen::EndianSwap(interface);
    auto decls = interface.Decls();
    const uint32 numDecls = interface.NumDecls();

    for (uint32 i = 0; i < numDecls; i++, decls.Next()) {
      clgen::EndianSwap(decls);
    }
  }

  hkxVertexDescriptionElementDecl GetDecls(size_t at) const override {
    auto decl = interface.Decls().Next(at);
    hkxVertexDescriptionElementDecl retval{
        .byteOffset = decl.ByteOffset(),
        .byteStride = interface.LayoutVersion() >= HK700 ? decl.ByteStride()
                                                         : interface.Stride(),
        .type = decl.Type(),
        .usage = decl.Usage(),
        .channelId = decl.ChannelID(),
    };

    if (interface.LayoutVersion() >= HK2012_2 &&
        interface.LayoutVersion() < HK2014 &&
        decl.Hint() == hkxVertexDescriptionDataHint::LIGHTMAP) {
      retval.channelId = "lightmap";
    }

    return retval;
  }
};

struct hkxVertexDescriptionFromFormat : hkxVertexDescriptionInternalInterface {
  std::vector<hkxVertexDescriptionElementDecl> decls;

  hkxVertexDescriptionFromFormat(const hkxVertexFormat &format) {
    uint32 curOffset = 0;

    if (format.positionOffset < 0xff) {
      decls.emplace_back(hkxVertexDescriptionElementDecl{
          .byteOffset = curOffset,
          .byteStride = format.stride,
          .type = DataType::FLOAT4,
          .usage = DataUsage::POSITION,
      });
      curOffset += 16;
    }

    if (format.normalOffset < 0xff) {
      decls.emplace_back(hkxVertexDescriptionElementDecl{
          .byteOffset = curOffset,
          .byteStride = format.stride,
          .type = DataType::FLOAT4,
          .usage = DataUsage::NORMAL,
      });
      curOffset += 16;
    }

    if (format.tangentOffset < 0xff) {
      decls.emplace_back(hkxVertexDescriptionElementDecl{
          .byteOffset = curOffset,
          .byteStride = format.stride,
          .type = DataType::FLOAT4,
          .usage = DataUsage::TANGENT,
      });
      curOffset += 16;
    }

    if (format.binormalOffset < 0xff) {
      decls.emplace_back(hkxVertexDescriptionElementDecl{
          .byteOffset = curOffset,
          .byteStride = format.stride,
          .type = DataType::FLOAT4,
          .usage = DataUsage::BINORMAL,
      });
      curOffset += 16;
    }

    if (format.numBonesPerVertex > 0) {
      if (format.boneIndexOffset < 0xff) {
        decls.emplace_back(hkxVertexDescriptionElementDecl{
            .byteOffset = curOffset,
            .byteStride = format.stride,
            .type = DataType::UINT8,
            .usage = DataUsage::BLENDINDICES,
        });
        curOffset += 4;
      }

      if (format.boneWeightOffset < 0xff) {
        decls.emplace_back(hkxVertexDescriptionElementDecl{
            .byteOffset = curOffset,
            .byteStride = format.stride,
            .type = DataType::UINT8,
            .usage = DataUsage::BLENDWEIGHTS,
        });
        curOffset += 4;
      }
    }

    if (format.colorOffset < 0xff) {
      decls.emplace_back(hkxVertexDescriptionElementDecl{
          .byteOffset = curOffset,
          .byteStride = format.stride,
          .type = DataType::UINT32,
          .usage = DataUsage::COLOR,
      });
      curOffset += 4;
    }

    for (uint8 t = 0; t < format.numTextureChannels; t++) {
      if (format.tFloatCoordOffset < 0xff) {
        decls.emplace_back(hkxVertexDescriptionElementDecl{
            .byteOffset = curOffset,
            .byteStride = format.stride,
            .type = DataType::FLOAT2,
            .usage = DataUsage::TEXCOORD,
        });
        curOffset += 8;
      } else if (format.tQuantizedCoordOffset < 0xff) {
        decls.emplace_back(hkxVertexDescriptionElementDecl{
            .byteOffset = curOffset,
            .byteStride = format.stride,
            .type = DataType::INT16,
            .usage = DataUsage::TEXCOORD,
        });
        curOffset += 4;
      }
    }
  };

  void SetDataPointer(void *) override {}

  const void *GetPointer() const override { return this; }

  void SwapEndian() override {}

  size_t GetNumDecls() const override { return decls.size(); }

  hkxVertexDescriptionElementDecl GetDecls(size_t at) const override {
    return decls.at(at);
  }
};

struct hkxVertexBufferMidInterface : hkxVertexBufferInternalInterface {
  clgen::hkxVertexBuffer::Interface interface;

  hkxVertexBufferMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxVertexBufferMidInterface(
      const clgen::hkxVertexBuffer::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override { FByteswapper(interface); }

  uint32 NumVertices() const override {
    return interface.LayoutVersion() >= HK700 ? interface.Data_().NumVerts()
                                              : interface.NumVertexData();
  }

  const char *VertexData(DataType type) const override {
    if (interface.LayoutVersion() >= HK700) {
      auto data = interface.Data_();
      switch (type) {
      case DataType::FLOAT:
      case DataType::FLOAT2:
        return reinterpret_cast<const char *>(data.FloatData());
      case DataType::FLOAT3:
      case DataType::FLOAT4:
        return reinterpret_cast<const char *>(data.VectorData());
      case DataType::UINT8:
        return reinterpret_cast<const char *>(data.Uint8Data());
      case DataType::INT16:
        return reinterpret_cast<const char *>(data.Uint16Data());
      case DataType::UINT32:
        return reinterpret_cast<const char *>(data.Uint32Data());
      case DataType::NONE:
        return nullptr;
      }
    }
    const uint8 *data = interface.VertexData();
    return reinterpret_cast<const char *>(data);
  }

  uni::Element<const hkxVertexDescription> VertexDesc() const override {
    if (interface.LayoutVersion() < HK600) {
      return {new hkxVertexDescriptionFromFormat(*interface.Format()), true};
    }
    return {new hkxVertexDescriptionMidInterface(interface.VertexDesc()), true};
  }
};

struct hkxVertexAnimationMidInterface : hkxVertexAnimationInternalInterface {
  clgen::hkxVertexAnimation::Interface interface;

  hkxVertexAnimationMidInterface(
      const clgen::hkxVertexAnimation::Interface &interface)
      : interface{interface} {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    clgen::EndianSwap(interface);
    auto vertData = interface.VertData();
    FByteswapper(vertData);
    int32 *vtxIndices = interface.VertexIndexMap();
    uint32 numVtxIdx = interface.NumVertexIndexMap();

    for (uint32 i = 0; i < numVtxIdx; i++) {
      FByteswapper(vtxIndices[i]);
    }

    hkxVertexAnimationUsageMap *components = interface.ComponentMap();
    const uint32 numComponents = interface.NumComponentMap();

    for (uint32 i = 0; i < numComponents; i++) {
      FByteswapper(components[i].use);
    }
  }

  float Time() const override { return interface.Time(); }

  uni::Element<hkxVertexBuffer> VertData() const override {
    return {new hkxVertexBufferMidInterface(interface.VertData()), true};
  }

  std::span<const int32> VertexIndexMap() const override {
    return {interface.VertexIndexMap(), interface.NumVertexIndexMap()};
  }

  std::span<const hkxVertexAnimationUsageMap> ComponentMaps() const override {
    return {interface.ComponentMap(), interface.NumComponentMap()};
  }
};

struct hkxMeshSectionMidInterface : hkxMeshSectionInternalInterface {
  clgen::hkxMeshSection::Interface interface;

  hkxMeshSectionMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxMeshSectionMidInterface(const clgen::hkxMeshSection::Interface &interface,
                             IhkPackFile *header)
      : interface(interface) {
    this->header = header;
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    clgen::EndianSwap(interface);
    auto boneMatricesMap = interface.BoneMatrixMap();
    const uint32 numBoneMatricesMaps = interface.NumBoneMatrixMap();

    for (uint32 i = 0; i < numBoneMatricesMaps; i++, boneMatricesMap.Next()) {
      clgen::EndianSwap(boneMatricesMap);
      int16 *mapping = boneMatricesMap.Mapping();
      uint32 numMappings = boneMatricesMap.NumMapping();

      for (uint32 j = 0; j < numMappings; j++) {
        FByteswapper(mapping[j]);
      }
    }

    float *linearKFs = interface.LinearKeyFrameHints();
    uint32 numLinearKFs = interface.NumLinearKeyFrameHints();

    for (uint32 i = 0; i < numLinearKFs; i++) {
      FByteswapper(linearKFs[i]);
    }
  }

  uni::Element<const hkxVertexBuffer> VertexBuffer() const override {
    return {new hkxVertexBufferMidInterface(*interface.VertexBuffer()), true};
  }

  size_t GetNumIndexBuffers() const override {
    return interface.NumIndexBuffers();
  }

  uni::Element<const hkxIndexBuffer> GetIndexBuffer(size_t at) const override {
    return {new hkxIndexBufferMidInterface(**interface.IndexBuffers().Next(at)),
            true};
  }

  const hkxMaterial *Material() const override {
    return *header->GetClass(interface.Material().data);
  }

  size_t GetNumUserChannels() const override {
    return interface.NumUserChannels();
  }

  const IhkVirtualClass *GetUserChannel(size_t at) const override {
    return header->GetClass(interface.UserChannels().Next(at).Object());
  }

  size_t GetNumVertexAnimations() const override {
    return interface.NumVertexAnimations();
  }

  uni::Element<hkxVertexAnimation>
  GetVertexAnimation(size_t at) const override {
    return {new hkxVertexAnimationMidInterface(
                **interface.VertexAnimations().Next(at)),
            true};
  }

  std::span<const float> LinearKeyFrameHints() const override {
    return {interface.LinearKeyFrameHints(),
            interface.NumLinearKeyFrameHints()};
  }

  size_t NumBoneMatrixMaps() const override {
    return interface.NumBoneMatrixMap();
  }

  std::span<const int16> BoneMatrixMapping(size_t at) const override {
    return {interface.BoneMatrixMap().Next(at).Mapping(),
            interface.BoneMatrixMap().Next(at).NumMapping()};
  }
};

struct hkxMeshMidInterface : hkxMeshInternalInterface {
  clgen::hkxMesh::Interface interface;

  hkxMeshMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}
  hkxMeshMidInterface(const clgen::hkxMesh::Interface &interface)
      : interface{interface} {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override { clgen::EndianSwap(interface); }

  size_t GetNumSections() const override { return interface.NumSections(); }

  uni::Element<const hkxMeshSection> GetSection(size_t at) const override {
    return {
        new hkxMeshSectionMidInterface(**interface.Sections().Next(at), header),
        true};
  }

  size_t GetNumUserChannelInfos() const override {
    return interface.NumUserChannelInfos();
  }

  hkxMeshUserChannelInfo GetUserChannelInfo(size_t at) const override {
    auto info = interface.UserChannelInfos().Next(at);

    return {
        .name = info.Name(),
        .className = info.ClassName(),
    };
  }
};

hkxMeshInternalInterface *hkxMeshInternalInterface::Create(void *interface) {
  return new hkxMeshMidInterface(
      *static_cast<clgen::hkxMesh::Interface *>(interface));
}

CREATE_HK_CLASS(hkxMesh);
CREATE_HK_CLASS(hkxVertexBuffer);
CREATE_HK_CLASS(hkxIndexBuffer);
CREATE_HK_CLASS(hkxMeshSection);
CREATE_HK_CLASS(hkxVertexDescription);
