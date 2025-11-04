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
#include <span>

static const char *INDEX_TYPE[]{
    "INDEX_TYPE_INVALID",
    "INDEX_TYPE_TRI_LIST",
    "INDEX_TYPE_TRI_STRIP",
    "INDEX_TYPE_TRI_FAN",
};

static const char *DATA_TYPE[]{
    /**/ //
    "HKX_DT_NONE",
    "HKX_DT_UINT8",
    "HKX_DT_INT16",
    "HKX_DT_UINT32",
    "HKX_DT_FLOAT",
    "HKX_DT_FLOAT2",
    "HKX_DT_FLOAT3",
    "HKX_DT_FLOAT4",
};

static const char *DATA_USAGE[]{
    /**/ //
    "HKX_DU_NONE",
    "HKX_DU_POSITION",
    "HKX_DU_COLOR",
    "HKX_DU_NORMAL",
    "HKX_DU_TANGENT",
    "HKX_DU_BINORMAL",
    "HKX_DU_TEXCOORD",
    "HKX_DU_BLENDWEIGHTS",
    "HKX_DU_BLENDINDICES",
    "HKX_DU_USERDATA",
};

void hkxVertexAnimationInternalInterface::ToXML(XMLHandle) const {}

void hkxVertexDescriptionInternalInterface::ToXML(XMLHandle hdl) const {
  //::ToXML("stride", Stride(), *hdl.node);
  auto declsNode = ToXMLArray("decls", GetNumDecls(), *hdl.node);

  for (auto decl : Decls()) {
    auto obj = declsNode.append_child(_hkObject);
    ::ToXML("byteOffset", decl.byteOffset, obj);
    ::ToXML("type", DATA_TYPE[uint32(decl.type)], obj);
    ::ToXML("dataUsage", DATA_USAGE[uint32(std::log2(uint32(decl.usage)))],
            obj);
  }
}

void hkxIndexBufferInternalInterface::ToXML(XMLHandle hdl) const {
  ::ToXML("indexType", INDEX_TYPE[uint32(GetIndexType())], *hdl.node);
  uint32 numFaces = Length();

  if (Is32Bit()) {
    std::span<const uint32> data(reinterpret_cast<const uint32 *>(Indices()),
                                 numFaces);
    auto indices = ToXMLArray("indices32", numFaces, *hdl.node);
    ::ToXML(data, indices);
  } else {
    std::span<const uint16> data(reinterpret_cast<const uint16 *>(Indices()),
                                 numFaces);
    auto indices = ToXMLArray("indices16", numFaces, *hdl.node);
    ::ToXML(data, indices);
  }

  ::ToXML("vertexBaseOffset", VerrtexBaseOffset(), *hdl.node);
  ::ToXML("length", numFaces, *hdl.node);
}

void hkxVertexBufferInternalInterface::ToXML(XMLHandle hdl) const {
  // auto vtxData = ToXMLArray("vertexData", NumVertices(), *hdl.node);
  //  TODO dump homogenous classes

  ::ToXML("vertexDesc", VertexDesc(), *hdl.node);
}

void hkxMeshSectionInternalInterface::ToXML(XMLHandle hdl) const {
  ::ToXML("vertexBuffer", VertexBuffer(), *hdl.node);
  auto channels = ToXMLArray("userChannels", GetNumUserChannels(), *hdl.node);
  ::ToXML(IndexBuffers(), channels);
}

void hkxMeshInternalInterface::ToXML(XMLHandle hdl) const {
  auto sections = ToXMLArray("sections", GetNumSections(), *hdl.node);
  ::ToXML(Sections(), sections);

  // auto userChanInfos =
  //    ToXMLArray("userChannelInfos", GetNumUserChannelInfos(), *hdl.node);
  // TODO ::ToXML(UserInfos(), userChanInfos);
}
