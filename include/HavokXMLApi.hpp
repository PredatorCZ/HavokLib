/*  Havok Format Library
    Copyright(C) 2016-2020 Lukas Cone

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
#include "hkInternalInterfaces.h"

#define DECLARE_XMLCLASS(classname, parent)                                    \
  DECLARE_HKCLASS(classname)                                                   \
  void SwapEndian() {}                                                         \
  const void *GetPointer() const { return this; };                             \
  void Process() {}                                                            \
  void SetDataPointer(void *Ptr) {}                                            \
                                                                               \
public:                                                                        \
  classname() {                                                                \
    this->hash = classname::HASH;                                              \
    this->superHash = parent::HASH;                                            \
    this->name = #parent;                                                      \
  }

class xmlHavokFile : public IhkPackFile {
  VirtualClasses classes;
  int32 GetVersion() const override { return -1; }

public:
  VirtualClasses &GetAllClasses() { return classes; }
  template <class C> C *NewClass() {
    C *cls = new C();
    classes.emplace_back(dynamic_cast<hkVirtualClass *>(cls));
    return cls;
  }
};

class xmlRootLevelContainer : public hkRootLevelContainerInternalInterface {
  DECLARE_XMLCLASS(xmlRootLevelContainer, hkRootLevelContainer)

  const int GetNumVariants() const { return static_cast<int>(variants.size()); }
  const hkNamedVariant GetVariant(int id) const { return variants.at(id); }
  void AddVariant(hkVirtualClass *input) {
    variants.push_back({input->name, input->name, input});
  }

private:
  std::vector<hkNamedVariant> variants;
};

class xmlAnimationContainer : public hkaAnimationContainerInternalInterface {
  DECLARE_XMLCLASS(xmlAnimationContainer, hkaAnimationContainer)

  size_t GetNumSkeletons() const override { return skeletons.size(); }
  const hkaSkeleton *GetSkeleton(size_t id) const override {
    return skeletons.at(id);
  }
  size_t GetNumAnimations() const override { return animations.size(); }
  const hkaAnimation *GetAnimation(size_t id) const override {
    return animations.at(id);
  }
  size_t GetNumBindings() const override { return bindings.size(); }
  const hkaAnimationBinding *GetBinding(size_t id) const override {
    return bindings.at(id);
  }
  size_t GetNumAttachments() const override { return attachments.size(); }
  const hkaBoneAttachment *GetAttachment(size_t id) const override {
    return attachments.at(id);
  }
  size_t GetNumSkins() const override { return skins.size(); }
  const hkaMeshBinding *GetSkin(size_t id) const { return skins.at(id); }

  // private:
  std::vector<hkaSkeleton *> skeletons;
  std::vector<hkaAnimation *> animations;
  std::vector<hkaAnimationBinding *> bindings;
  std::vector<hkaBoneAttachment *> attachments;
  std::vector<hkaMeshBinding *> skins;
};

struct xmlBone {
  int16 ID;
  std::string name;
  xmlBone *parent;
  hkQTransform transform;
};

struct xmlRefFloat {
  std::string name;
  float value;
};

class xmlSkeleton : public hkaSkeletonInternalInterface {
  DECLARE_XMLCLASS(xmlSkeleton, hkaSkeleton)

  es::string_view Name() const override { return name; }
  size_t GetNumLocalFrames() const override { return localFrames.size(); }
  hkLocalFrameOnBone GetLocalFrame(size_t id) const override {
    return localFrames.at(id);
  }
  size_t GetNumPartitions() const override { return partitions.size(); }
  hkaPartition GetPartition(size_t id) const override {
    return partitions.at(id);
  }
  size_t GetNumBones() const override { return bones.size(); }
  es::string_view GetBoneName(size_t id) const override {
    return bones.at(id)->name;
  }
  const hkQTransform *GetBoneTM(size_t id) const override {
    return &bones.at(id)->transform;
  }
  int16 GetBoneParentID(size_t id) const override {
    xmlBone *parent = bones.at(id)->parent;
    return parent ? parent->ID : -1;
  }

  size_t GetNumReferenceFloats() const override { return floats.size(); };
  float GetReferenceFloat(size_t id) const override {
    return floats.at(id).value;
  }
  size_t GetNumFloatSlots() const override { return GetNumReferenceFloats(); }
  es::string_view GetFloatSlot(size_t id) const override {
    return floats.at(id).name;
  }

  std::string name;
  std::vector<std::unique_ptr<xmlBone>> bones;
  std::vector<xmlRefFloat> floats;
  std::vector<hkLocalFrameOnBone> localFrames;
  std::vector<hkaPartition> partitions;
};

class xmlAnnotationTrack : public hkaAnnotationTrackInternalInterface {
  DECLARE_XMLCLASS(xmlAnnotationTrack, hkaAnnotationTrack);

  es::string_view GetName() const { return name; }
  std::string name;
};

template <class _parent> class xmlAnimation : public virtual _parent {
  DECLARE_XMLCLASS(xmlAnimation, hkaAnimation);

  es::string_view GetAnimationTypeName() const override {
    return GetReflectedEnum<hkaAnimationType>()[animType];
  }
  hkaAnimationType GetAnimationType() const override { return animType; }
  float Duration() const override { return duration; }
  const hkaAnimatedReferenceFrame *GetExtractedMotion() const override {
    return nullptr;
  } // TODO
  size_t GetNumAnnotations() const override { return annotations.size(); }
  hkaAnnotationTrackPtr GetAnnotation(size_t id) const override {
    return hkaAnnotationTrackPtr(&annotations[id], false);
  }

  hkaAnimationType animType;
  float duration;
  mutable std::vector<xmlAnnotationTrack> annotations;
};

class xmlInterleavedAnimation
    : public xmlAnimation<hkaAnimationLerpSampler>,
      public hkaInterleavedAnimationInternalInterface {
  DECLARE_HKCLASS(xmlInterleavedAnimation)
  void SwapEndian() {}
  const void *GetPointer() const { return this; };
  void Process() {}
  void SetDataPointer(void *Ptr) {}

public:
  xmlInterleavedAnimation() { xmlAnimation::hash = HASH; }
  typedef std::vector<hkQTransform> Transform_Container;
  typedef std::vector<float> Float_Container;

  std::vector<Transform_Container *> transforms;
  std::vector<Float_Container *> floats;

  size_t GetNumOfTransformTracks() const override { return transforms.size(); }
  size_t GetNumOfFloatTracks() const override { return floats.size(); }

  es::string_view GetClassName(hkXMLToolsets toolset) const override {
    if (toolset == HK550)
      return "hkaInterleavedSkeletalAnimation";
    else
      return "hkaInterleavedUncompressedAnimation";
  }

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override {
    out = transforms[trackID]->at(frame);
  }

  size_t GetNumTransforms() const override {
    if (transforms.size())
      return transforms[0]->size() * transforms.size();

    return 0;
  }

  size_t GetNumFloats() const override {
    if (floats.size())
      return floats[0]->size() * floats.size();

    return 0;
  }

  const hkQTransform *GetTransform(size_t id) const override {
    const size_t numTracks = transforms.size();
    return &transforms[id % numTracks]->at(id / numTracks);
  }

  float GetFloat(size_t id) const override {
    const int numTracks = static_cast<int>(floats.size());
    return floats[id % numTracks]->at(id / numTracks);
  }

  ~xmlInterleavedAnimation() {
    for (auto &t : transforms)
      delete t;

    for (auto &f : floats)
      delete f;
  }
};

class xmlAnimationBinding : public hkaAnimationBindingInternalInterface {
  DECLARE_XMLCLASS(xmlAnimationBinding, hkaAnimationBinding);
  hkaAnimation *animation = nullptr;
  std::string skeletonName;
  std::vector<int16> transformTrackToBoneIndices;
  std::vector<int16> floatTrackToFloatSlotIndices;
  std::vector<int16> partitionIndices;
  BlendHint blendHint = NORMAL;

  es::string_view GetSkeletonName() const override { return skeletonName; }
  const hkaAnimation *GetAnimation() const override { return animation; }
  BlendHint GetBlendHint() const override { return blendHint; }
  size_t GetNumTransformTrackToBoneIndices() const override {
    return transformTrackToBoneIndices.size();
  }
  int16 GetTransformTrackToBoneIndex(size_t id) const override {
    return transformTrackToBoneIndices[id];
  }
  size_t GetNumFloatTrackToFloatSlotIndices() const override {
    return floatTrackToFloatSlotIndices.size();
  }
  int16 GetFloatTrackToFloatSlotIndex(size_t id) const override {
    return floatTrackToFloatSlotIndices[id];
  }
  size_t GetNumPartitionIndices() const override {
    return partitionIndices.size();
  }
  int16 GetPartitionIndex(size_t id) const override {
    return partitionIndices[id];
  }
};

struct xmlEnvironmentVariable {
  std::string name, value;

  xmlEnvironmentVariable() = default;
  xmlEnvironmentVariable(const std::string &iname, const std::string &ivalue)
      : name(iname), value(ivalue) {}

  operator hkxEnvironmentVariable() const { return {name, value}; }
};

class xmlEnvironment
    : public hkxEnvironmentInternalInterface,
      public uni::VectorList<hkxEnvironmentVariable, xmlEnvironmentVariable,
                             uni::Vector> {
  DECLARE_XMLCLASS(xmlEnvironment, hkxEnvironment);
};