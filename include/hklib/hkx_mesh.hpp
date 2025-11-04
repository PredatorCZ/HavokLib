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

#pragma once
#include "hk_base.hpp"
#include "hkx_material.hpp"
#include <span>

enum class DataType : uint16 {
  NONE,
  UINT8,
  INT16,
  UINT32,
  FLOAT,
  FLOAT2,
  FLOAT3,
  FLOAT4,
};

enum class DataUsage : uint16 {
  NONE,
  POSITION,
  COLOR,
  NORMAL = 4,
  TANGENT = 8,
  BINORMAL = 16,
  TEXCOORD = 32,
  BLENDWEIGHTS = 64,
  BLENDINDICES = 128,
  USERDATA = 256,
};

struct hkxVertexDescriptionElementDecl {
  uint32 byteOffset;
  uint32 byteStride;
  DataType type;
  DataUsage usage;
  std::string_view channelId{};
};

struct hkxVertexAnimationUsageMap {
  DataUsage use;
  uint8 useIndexOrig;
  uint8 useIndexLocal;
};

struct hkxVertexAnimation : IhkxVirtualClass {
  virtual float Time() const = 0;
  virtual uni::Element<hkxVertexBuffer> VertData() const = 0;
  virtual std::span<const int32> VertexIndexMap() const = 0;
  virtual std::span<const hkxVertexAnimationUsageMap> ComponentMaps() const = 0;
};

struct hkxVertexDescription : IhkxVirtualClass {
  virtual const void *GetPointer() const = 0;
  virtual ~hkxVertexDescription() = default;
  virtual size_t GetNumDecls() const = 0;
  virtual hkxVertexDescriptionElementDecl GetDecls(size_t at) const = 0;

  typedef uni::VirtualIteratorProxy<
      hkxVertexDescription, &hkxVertexDescription::GetNumDecls,
      hkxVertexDescriptionElementDecl, &hkxVertexDescription::GetDecls>
      iteratorDecls;

  const iteratorDecls Decls() const { return iteratorDecls(this); }
};

struct hkxVertexBuffer : IhkxVirtualClass {
  virtual uint32 NumVertices() const = 0;
  virtual const char *VertexData(DataType type) const = 0;
  virtual uni::Element<const hkxVertexDescription> VertexDesc() const = 0;
};

enum class IndexType : uint8 {
  INVALID,
  TRI_LIST,
  TRI_STRIP,
  TRI_FAN,
};

struct hkxIndexBuffer : IhkxVirtualClass {
  virtual IndexType GetIndexType() const = 0;
  virtual size_t NumIndices() const = 0;
  virtual const char *Indices() const = 0;
  virtual uint32 VerrtexBaseOffset() const = 0;
  virtual uint32 Length() const = 0;
  virtual bool Is32Bit() const = 0;
};

struct hkxMeshSection : IhkxVirtualClass {
  virtual uni::Element<const hkxVertexBuffer> VertexBuffer() const = 0;
  virtual size_t GetNumIndexBuffers() const = 0;
  virtual uni::Element<const hkxIndexBuffer>
  GetIndexBuffer(size_t at) const = 0;
  virtual const hkxMaterial *Material() const = 0;
  virtual size_t GetNumUserChannels() const = 0;
  virtual const IhkVirtualClass *GetUserChannel(size_t at) const = 0;
  virtual size_t GetNumVertexAnimations() const = 0;
  virtual uni::Element<hkxVertexAnimation>
  GetVertexAnimation(size_t at) const = 0;
  virtual std::span<const float> LinearKeyFrameHints() const = 0;
  virtual size_t NumBoneMatrixMaps() const = 0;
  virtual std::span<const int16> BoneMatrixMapping(size_t at) const = 0;

  typedef uni::VirtualIteratorProxy<
      hkxMeshSection, &hkxMeshSection::GetNumIndexBuffers,
      uni::Element<const hkxIndexBuffer>, &hkxMeshSection::GetIndexBuffer>
      iteratorIndexBuffer;

  typedef uni::VirtualIteratorProxy<
      hkxMeshSection, &hkxMeshSection::GetNumUserChannels,
      const IhkVirtualClass *, &hkxMeshSection::GetUserChannel>
      iteratorUserChannels;

  const iteratorIndexBuffer IndexBuffers() const {
    return iteratorIndexBuffer(this);
  }
  const iteratorUserChannels UserIChannels() const {
    return iteratorUserChannels(this);
  }
};

struct hkxMeshUserChannelInfo {
  std::string_view name;
  std::string_view className;
};

struct hkxMesh : IhkxVirtualClass {
  virtual size_t GetNumSections() const = 0;
  virtual uni::Element<const hkxMeshSection> GetSection(size_t at) const = 0;
  virtual size_t GetNumUserChannelInfos() const = 0;
  virtual hkxMeshUserChannelInfo GetUserChannelInfo(size_t at) const = 0;

  typedef uni::VirtualIteratorProxy<hkxMesh, &hkxMesh::GetNumSections,
                                    uni::Element<const hkxMeshSection>,
                                    &hkxMesh::GetSection>
      iteratorSections;

  typedef uni::VirtualIteratorProxy<hkxMesh, &hkxMesh::GetNumUserChannelInfos,
                                    hkxMeshUserChannelInfo,
                                    &hkxMesh::GetUserChannelInfo>
      iteratorUserChannelInfos;

  const iteratorSections Sections() const { return iteratorSections(this); }
  const iteratorUserChannelInfos UserInfos() const {
    return iteratorUserChannelInfos(this);
  }
};
