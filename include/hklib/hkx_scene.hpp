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
#include "hkx_attributeholder.hpp"
#include "hkx_mesh.hpp"
#include "spike/type/matrix44.hpp"
#include <span>

struct hkxNodeAnnotationData {
  float time;
  std::string_view description;
};

struct hkxNode : hkxAttributeHolder {
  virtual std::string_view Name() const = 0;
  virtual const IhkVirtualClass *Object() const = 0;
  virtual std::span<const es::Matrix44> KeyFrames() const = 0;
  virtual size_t GetNumChildren() const = 0;
  virtual uni::Element<const hkxNode> GetChild(size_t at) const = 0;
  virtual size_t GetNumAnnotations() const = 0;
  virtual hkxNodeAnnotationData GetAnnotation(size_t at) const = 0;
  virtual std::string_view UserProperties() const = 0;
  virtual bool Selected() const = 0;

  typedef uni::VirtualIteratorProxy<hkxNode, &hkxNode::GetNumChildren,
                                    uni::Element<const hkxNode>,
                                    &hkxNode::GetChild>
      iteratorChildren;

  typedef uni::VirtualIteratorProxy<hkxNode, &hkxNode::GetNumAnnotations,
                                    hkxNodeAnnotationData,
                                    &hkxNode::GetAnnotation>
      iteratorAnnotations;

  const iteratorChildren Children() const { return iteratorChildren(this); }
  const iteratorAnnotations Annotations() const {
    return iteratorAnnotations(this);
  }
};

struct hkxNodeSelectionSet : hkxAttributeHolder {
  virtual size_t GetNumSelectedNodes() const = 0;
  virtual uni::Element<const hkxNode> GetSelectedNode(size_t at) const = 0;
  virtual std::string_view Name() const = 0;

  typedef uni::VirtualIteratorProxy<
      hkxNodeSelectionSet, &hkxNodeSelectionSet::GetNumSelectedNodes,
      uni::Element<const hkxNode>, &hkxNodeSelectionSet::GetSelectedNode>
      iteratorNodes;

  const iteratorNodes SelectedNodes() const { return iteratorNodes(this); }
};

struct hkxCamera {
  Vector4A16 from;
  Vector4A16 focus;
  Vector4A16 up;
  float fov;
  float far;
  float near;
  bool leftHanded;
};

enum class LightType : uint8 {
  POINT_LIGHT,
  DIRECTIONAL_LIGHT,
  SPOT_LIGHT,
};

struct hkxLight : IhkxVirtualClass {
  virtual LightType GetLightType() const = 0;
  virtual Vector4A16 Position() const = 0;
  virtual Vector4A16 Direction() const = 0;
  virtual uint32 Color() const = 0;
  virtual float Angle() const = 0;
};

struct hkxTextureInplace : IhkxVirtualClass {
  virtual std::string_view FileType() const = 0;
  virtual size_t DataSize() const = 0;
  virtual const char *Data() const = 0;
  virtual std::string_view Name() const = 0;
  virtual std::string_view OriginalFilename() const = 0;
};

struct hkxTextureFile : IhkxVirtualClass {
  virtual std::string_view FileName() const = 0;
  virtual std::string_view Name() const = 0;
  virtual std::string_view OriginalFilename() const = 0;
};

struct hkxSkinBinding : IhkxVirtualClass {
  virtual const hkxMesh *Mesh() const = 0;
  virtual size_t GetNumMappings() const = 0;
  virtual uni::Element<const hkxNode> GetMapping(size_t at) const = 0;
  virtual std::span<const es::Matrix44> BindPoses() const = 0;
  virtual es::Matrix44 InitSkinTransform() const = 0;

  typedef uni::VirtualIteratorProxy<
      hkxSkinBinding, &hkxSkinBinding::GetNumMappings,
      uni::Element<const hkxNode>, &hkxSkinBinding::GetMapping>
      iteratorMappings;

  const iteratorMappings Mappings() const { return iteratorMappings(this); }
};

struct hkxSplineControlPoint {
  enum class ControlType : uint8 {
    BEZIER_SMOOTH,
    BEZIER_CORNER,
    LINEAR,
    CUSTOM,
  };

  Vector4A16 position;
  Vector4A16 tangentIn;
  Vector4A16 tangentOut;
  ControlType inType;
  ControlType outType;
};

struct hkxScene : virtual IhkxVirtualClass {
  virtual std::string_view Modeller() const = 0;
  virtual std::string_view Asset() const = 0;
  virtual float SceneLength() const = 0;
  virtual uni::Element<const hkxNode> RootNode() const = 0;
  virtual size_t GetNumSelectionSets() const = 0;
  virtual uni::Element<const hkxNodeSelectionSet>
  GetSelectionSet(size_t at) const = 0;
  virtual size_t GetNumCameras() const = 0;
  virtual const hkxCamera *GetCamera(size_t at) const = 0;
  virtual size_t GetNumLights() const = 0;
  virtual uni::Element<const hkxLight> GetLight(size_t at) const = 0;
  virtual size_t GetNumMaterials() const = 0;
  virtual const hkxMaterial *GetMaterial(size_t at) const = 0;
  virtual size_t GetNumMeshes() const = 0;
  virtual const hkxMesh *GetMesh(size_t at) const = 0;
  virtual size_t GetNumInplaceTextures() const = 0;
  virtual uni::Element<const hkxTextureInplace>
  GetInplaceTexture(size_t at) const = 0;
  virtual size_t GetNumExternalTextures() const = 0;
  virtual uni::Element<const hkxTextureFile>
  GetExternalTexture(size_t at) const = 0;
  virtual size_t GetNumSkinBindings() const = 0;
  virtual uni::Element<const hkxSkinBinding>
  GetSkinBinding(size_t at) const = 0;
  virtual es::Matrix44 AppliedTransform() const = 0;

  typedef uni::VirtualIteratorProxy<hkxScene, &hkxScene::GetNumSelectionSets,
                                    uni::Element<const hkxNodeSelectionSet>,
                                    &hkxScene::GetSelectionSet>
      iteratorSelectionSets;

  typedef uni::VirtualIteratorProxy<hkxScene, &hkxScene::GetNumCameras,
                                    const hkxCamera *, &hkxScene::GetCamera>
      iteratorCameras;

  typedef uni::VirtualIteratorProxy<hkxScene, &hkxScene::GetNumLights,
                                    uni::Element<const hkxLight>,
                                    &hkxScene::GetLight>
      iteratorLights;

  typedef uni::VirtualIteratorProxy<hkxScene, &hkxScene::GetNumMaterials,
                                    const hkxMaterial *, &hkxScene::GetMaterial>
      iteratorMaterials;

  typedef uni::VirtualIteratorProxy<hkxScene, &hkxScene::GetNumMeshes,
                                    const hkxMesh *, &hkxScene::GetMesh>
      iteratorMeshes;

  typedef uni::VirtualIteratorProxy<hkxScene, &hkxScene::GetNumInplaceTextures,
                                    uni::Element<const hkxTextureInplace>,
                                    &hkxScene::GetInplaceTexture>
      iteratorInplaceTextures;

  typedef uni::VirtualIteratorProxy<hkxScene, &hkxScene::GetNumExternalTextures,
                                    uni::Element<const hkxTextureFile>,
                                    &hkxScene::GetExternalTexture>
      iteratorExternalTextures;

  typedef uni::VirtualIteratorProxy<hkxScene, &hkxScene::GetNumSkinBindings,
                                    uni::Element<const hkxSkinBinding>,
                                    &hkxScene::GetSkinBinding>
      iteratorSkinBindings;

  const iteratorSelectionSets SelectionSets() const {
    return iteratorSelectionSets(this);
  }
  const iteratorLights Lights() const { return iteratorLights(this); }
  const iteratorCameras Cameras() const { return iteratorCameras(this); }
  const iteratorMaterials Materials() const { return iteratorMaterials(this); }
  const iteratorMeshes Meshes() const { return iteratorMeshes(this); }
  const iteratorInplaceTextures InplaceTextures() const {
    return iteratorInplaceTextures(this);
  }
  const iteratorExternalTextures ExternalTextures() const {
    return iteratorExternalTextures(this);
  }
  const iteratorSkinBindings SkinBindings() const {
    return iteratorSkinBindings(this);
  }
};
