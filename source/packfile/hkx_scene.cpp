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

#include "internal/hkx_scene.hpp"
#include "base.hpp"
#include "hkx_attribute_holder.hpp"

struct hkMatrix3 {
  Vector4A16 r0;
  Vector4A16 r1;
  Vector4A16 r2;
};

#include "hkx_scene.inl"

template <> void FByteswapper(hkxCamera &item, bool) {
  FByteswapper(item.from);
  FByteswapper(item.focus);
  FByteswapper(item.up);
  FByteswapper(item.fov);
  FByteswapper(item.far);
  FByteswapper(item.near);
}

struct hkxNodeMidInterface : hkxNodeInternalInterface,
                             hkxAttributeHolderMidInterface {
  clgen::hkxNode::Interface interface;

  hkxNodeMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxNodeMidInterface(const clgen::hkxNode::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    hkxAttributeHolderMidInterface::SwapEndian();
    clgen::EndianSwap(interface);
    es::Matrix44 *keyFrames = interface.KeyFrames();
    const uint32 numKeyFrames = interface.NumKeyFrames();

    for (uint32 i = 0; i < numKeyFrames; i++) {
      FByteswapper(keyFrames[i]);
    }

    auto annotations = interface.Annotations();
    uint32 numAnnotations = interface.NumAnnotations();

    for (uint32 i = 0; i < numAnnotations; i++, annotations.Next()) {
      clgen::EndianSwap(annotations);
    }

    float *linearKFs = interface.LinearKeyFrameHints();
    uint32 numLinearKFs = interface.NumLinearKeyFrameHints();

    for (uint32 i = 0; i < numLinearKFs; i++) {
      FByteswapper(linearKFs[i]);
    }
  }

  clgen::hkxAttributeHolder::Interface AttribHolder() const override {
    return interface.BasehkxAttributeHolder();
  }

  std::string_view Name() const override { return safe_sw(interface.Name()); }

  const IhkVirtualClass *Object() const override {
    return header->GetClass(interface.Object().Object());
  }

  std::span<const es::Matrix44> KeyFrames() const override {
    return {interface.KeyFrames(), interface.NumKeyFrames()};
  }

  size_t GetNumChildren() const override { return interface.NumChildren(); }

  uni::Element<const hkxNode> GetChild(size_t at) const override {
    return {new hkxNodeMidInterface(**interface.Children().Next(at)), true};
  }

  size_t GetNumAnnotations() const override {
    return interface.NumAnnotations();
  }

  hkxNodeAnnotationData GetAnnotation(size_t at) const override {
    auto annotation = interface.Annotations().Next(at);
    return hkxNodeAnnotationData{
        .time = annotation.Time(),
        .description = annotation.Description(),
    };
  }

  std::string_view UserProperties() const override {
    return safe_sw(interface.UserProperties());
  }

  bool Selected() const override { return interface.Selected(); }
};

struct hkxNodeSelectionSetMidInterface : hkxNodeSelectionSetInternalInterface,
                                         hkxAttributeHolderMidInterface {
  clgen::hkxNodeSelectionSet::Interface interface;

  hkxNodeSelectionSetMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxNodeSelectionSetMidInterface(
      const clgen::hkxNodeSelectionSet::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    hkxAttributeHolderMidInterface::SwapEndian();
    clgen::EndianSwap(interface);
  }

  clgen::hkxAttributeHolder::Interface AttribHolder() const override {
    return interface.BasehkxAttributeHolder();
  }

  size_t GetNumSelectedNodes() const override {
    return interface.NumSelectedNodes();
  }

  uni::Element<const hkxNode> GetSelectedNode(size_t at) const override {
    return {new hkxNodeMidInterface(**interface.SelectedNodes().Next(at)),
            true};
  }

  std::string_view Name() const override { return safe_sw(interface.Name()); }
};

struct hkxLightMidInterface : hkxLightInternalInterface {
  clgen::hkxLight::Interface interface;

  hkxLightMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxLightMidInterface(const clgen::hkxLight::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override { clgen::EndianSwap(interface); }

  LightType GetLightType() const override { return interface.Type(); }

  Vector4A16 Position() const override { return interface.Position(); }

  Vector4A16 Direction() const override { return interface.Direction(); }

  uint32 Color() const override { return interface.Color(); }

  float Angle() const override { return interface.Angle(); }
};

struct hkxTextureInplaceMidInterface : hkxTextureInplaceInternalInterface {
  clgen::hkxTextureInplace::Interface interface;

  hkxTextureInplaceMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxTextureInplaceMidInterface(
      const clgen::hkxTextureInplace::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override { clgen::EndianSwap(interface); }

  std::string_view FileType() const override {
    auto ftype = interface.FileType();
    return {ftype.data(), ftype.size()};
  }

  size_t DataSize() const override { return interface.NumData_(); }

  const char *Data() const override {
    return reinterpret_cast<const char *>(interface.Data_());
  }

  std::string_view Name() const override { return safe_sw(interface.Name()); }

  std::string_view OriginalFilename() const override {
    return safe_sw(interface.OriginalFilename());
  }
};

struct hkxTextureFileMidInterface : hkxTextureFileInternalInterface {
  clgen::hkxTextureFile::Interface interface;

  hkxTextureFileMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxTextureFileMidInterface(const clgen::hkxTextureFile::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override { clgen::EndianSwap(interface); }

  std::string_view FileName() const override {
    return safe_sw(interface.FileName());
  }

  std::string_view Name() const override { return safe_sw(interface.Name()); }

  std::string_view OriginalFilename() const override {
    return safe_sw(interface.OriginalFilename());
  }
};

struct hkxSkinBindingMidInterface : hkxSkinBindingInternalInterface {
  clgen::hkxSkinBinding::Interface interface;

  hkxSkinBindingMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxSkinBindingMidInterface(const clgen::hkxSkinBinding::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    clgen::EndianSwap(interface);
    es::Matrix44 *skins = interface.BindPose();
    uint32 numSkins = interface.NumBindPose();

    for (uint32 i = 0; i < numSkins; i++) {
      FByteswapper(skins[i]);
    }

    es::Matrix44 initPose = interface.InitSkinTransform();
    FByteswapper(initPose);
    interface.InitSkinTransform(initPose);
  }

  const hkxMesh *Mesh() const override {
    return safe_deref_cast<const hkxMesh>(
        header->GetClass(interface.Mesh().data));
  }

  size_t GetNumMappings() const override { return interface.NumMapping(); }

  uni::Element<const hkxNode> GetMapping(size_t at) const override {
    return {new hkxNodeMidInterface(**interface.Mapping().Next(at)), true};
  }

  std::span<const es::Matrix44> BindPoses() const override {
    return {interface.BindPose(), interface.NumBindPose()};
  }

  es::Matrix44 InitSkinTransform() const override {
    return interface.InitSkinTransform();
  }
};

struct hkxCameraMidInterface : hkxCameraInternalInterface {
  clgen::hkxCamera::Interface interface;

  hkxCameraMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxCameraMidInterface(const clgen::hkxCamera::Interface &interface)
      : interface(interface) {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    clgen::EndianSwap(interface);
    FByteswapper(*interface.Data_().data());
  }
};

struct hkxSceneMidInterface : hkxSceneInternalInterface {
  clgen::hkxScene::Interface interface;

  hkxSceneMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    clgen::EndianSwap(interface);
    auto tm = interface.AppliedTransform();
    FByteswapper(tm.r0);
    FByteswapper(tm.r1);
    FByteswapper(tm.r2);
    interface.AppliedTransform(tm);
  }

  std::string_view Modeller() const override {
    return safe_sw(interface.Modeller());
  }

  std::string_view Asset() const override { return safe_sw(interface.Asset()); }

  float SceneLength() const override { return interface.SceneLength(); }

  uni::Element<const hkxNode> RootNode() const override {
    return {new hkxNodeMidInterface(*interface.RootNode()), true};
  }

  size_t GetNumSelectionSets() const override {
    return interface.NumSelectionSets();
  }

  uni::Element<const hkxNodeSelectionSet>
  GetSelectionSet(size_t at) const override {
    return {new hkxNodeSelectionSetMidInterface(
                **interface.SelectionSets().Next(at)),
            true};
  }

  size_t GetNumCameras() const override { return interface.NumCameras(); }

  const hkxCamera *GetCamera(size_t at) const override {
    return (**interface.Cameras().Next(at)).Data_().data();
  }

  size_t GetNumLights() const override { return interface.NumLights(); }

  uni::Element<const hkxLight> GetLight(size_t at) const override {
    return {new hkxLightMidInterface(**interface.Lights().Next(at)), true};
  }

  size_t GetNumMaterials() const override { return interface.NumMaterials(); }

  const hkxMaterial *GetMaterial(size_t at) const override {
    return safe_deref_cast<const hkxMaterial>(
        header->GetClass((**interface.Materials().Next(at)).data));
  }

  size_t GetNumMeshes() const override { return interface.NumMeshes(); }

  const hkxMesh *GetMesh(size_t at) const override {
    return safe_deref_cast<const hkxMesh>(
        header->GetClass((**interface.Meshes().Next(at)).data));
  }

  size_t GetNumInplaceTextures() const override {
    return interface.NumInplaceTextures();
  }

  uni::Element<const hkxTextureInplace>
  GetInplaceTexture(size_t at) const override {
    return {new hkxTextureInplaceMidInterface(
                **interface.InplaceTextures().Next(at)),
            true};
  }

  size_t GetNumExternalTextures() const override {
    return interface.NumExternalTextures();
  }

  uni::Element<const hkxTextureFile>
  GetExternalTexture(size_t at) const override {
    return {
        new hkxTextureFileMidInterface(**interface.ExternalTextures().Next(at)),
        true};
  }

  size_t GetNumSkinBindings() const override {
    return interface.NumSkinBindings();
  }

  uni::Element<const hkxSkinBinding> GetSkinBinding(size_t at) const override {
    return {new hkxSkinBindingMidInterface(**interface.SkinBindings().Next(at)),
            true};
  }

  es::Matrix44 AppliedTransform() const override {
    hkMatrix3 atm = interface.AppliedTransform();
    return es::Matrix44(atm.r0, atm.r1, atm.r2);
  }
};

CREATE_HK_CLASS(hkxScene);
CREATE_HK_CLASS(hkxLight);
CREATE_HK_CLASS(hkxNode);
CREATE_HK_CLASS(hkxCamera);
CREATE_HK_CLASS(hkxNodeSelectionSet);
CREATE_HK_CLASS(hkxSkinBinding);
CREATE_HK_CLASS(hkxTextureFile);
CREATE_HK_CLASS(hkxTextureInplace);
