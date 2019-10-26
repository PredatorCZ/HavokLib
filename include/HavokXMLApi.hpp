/*  Havok Format Library
    Copyright(C) 2016-2019 Lukas Cone

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
    hash = classname::HASH;                                                    \
    superHash = parent::HASH;                                                  \
    namePtr = #parent;                                                         \
  }

class xmlHavokFile : public IhkPackFile {
  VirtualClasses classes;
  int GetVersion() { return -1; }

public:
  VirtualClasses &GetAllClasses() { return classes; }
  template <class C> C *NewClass() {
    C *cls = new C();
    classes.push_back(dynamic_cast<hkVirtualClass *>(cls));
    return cls;
  }
  ~xmlHavokFile();
};

class xmlRootLevelContainer : public hkRootLevelContainerInternalInterface {
  DECLARE_XMLCLASS(xmlRootLevelContainer, hkRootLevelContainer)

  const int GetNumVariants() const { return static_cast<int>(variants.size()); }
  const hkNamedVariant GetVariant(int id) const { return variants.at(id); }
  void AddVariant(hkVirtualClass *input) {
    variants.push_back({input->namePtr, input->namePtr, input});
  }

private:
  std::vector<hkNamedVariant> variants;
};

class xmlAnimationContainer : public hkaAnimationContainerInternalInterface {
  DECLARE_XMLCLASS(xmlAnimationContainer, hkaAnimationContainer)

  const int GetNumSkeletons() const {
    return static_cast<int>(skeletons.size());
  }
  const hkaSkeleton *GetSkeleton(int id) const { return skeletons.at(id); }
  const int GetNumAnimations() const {
    return static_cast<int>(animations.size());
  }
  const hkaAnimation *GetAnimation(int id) const { return animations.at(id); }
  const int GetNumBindings() const { return static_cast<int>(bindings.size()); }
  const hkaAnimationBinding *GetBinding(int id) const {
    return bindings.at(id);
  }
  const int GetNumAttachments() const {
    return static_cast<int>(attachments.size());
  }
  const hkaBoneAttachment *GetAttachment(int id) const {
    return attachments.at(id);
  }
  const int GetNumSkins() const { return static_cast<int>(skins.size()); }
  const hkaMeshBinding *GetSkin(int id) const { return skins.at(id); }

  // private:
  std::vector<hkaSkeleton *> skeletons;
  std::vector<hkaAnimation *> animations;
  std::vector<hkaAnimationBinding *> bindings;
  std::vector<hkaBoneAttachment *> attachments;
  std::vector<hkaMeshBinding *> skins;
};

struct xmlBone {
  short ID;
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

  const char *GetSkeletonName() const { return name.c_str(); }
  const int GetNumLocalFrames() const {
    return static_cast<int>(localFrames.size());
  }
  const hkLocalFrameOnBone GetLocalFrame(int id) const {
    return localFrames.at(id);
  }
  const int GetNumPartitions() const {
    return static_cast<int>(partitions.size());
  }
  const hkaPartition GetPartition(int id) const { return partitions.at(id); }
  const int GetNumBones() const { return static_cast<int>(bones.size()); }
  const char *GetBoneName(int id) const { return bones.at(id)->name.c_str(); }
  const hkQTransform *GetBoneTM(int id) const {
    return &bones.at(id)->transform;
  }
  const short GetBoneParentID(int id) const {
    xmlBone *parent = bones.at(id)->parent;
    return parent ? parent->ID : -1;
  }

  const int GetNumReferenceFloats() const {
    return static_cast<int>(floats.size());
  };
  const float GetReferenceFloat(int id) const { return floats.at(id).value; }
  const int GetNumFloatSlots() const { return GetNumReferenceFloats(); }
  const char *GetFloatSlot(int id) const { return floats.at(id).name.c_str(); }

  std::string name;
  std::vector<xmlBone *> bones;
  std::vector<xmlRefFloat> floats;
  std::vector<hkLocalFrameOnBone> localFrames;
  std::vector<hkaPartition> partitions;

  ~xmlSkeleton() {
    for (auto &b : bones)
      delete b;
  }
};

class xmlAnnotationTrack : public hkaAnnotationTrackInternalInterface {
  DECLARE_XMLCLASS(xmlAnnotationTrack, hkaAnnotationTrack);

  const char *GetName() const { return name.c_str(); }
  const int GetNumAnnotations() const {
    return static_cast<int>(annotations.size());
  }
  Annotation GetAnnotation(int id) const { return annotations[id]; }

  std::string name;
  std::vector<hkaAnnotationTrack::Annotation> annotations;
};

class xmlAnimation : public virtual hkaAnimationInternalInterface {
  DECLARE_XMLCLASS(xmlAnimation, hkaAnimation);

  const char *GetAnimationTypeName() const {
    return _EnumWrap<hkaAnimationType>{}._reflected[animType];
  }
  const hkaAnimationType GetAnimationType() const { return animType; }
  const float GetDuration() const { return duration; }
  const hkaAnimatedReferenceFrame *GetExtractedMotion() const {
    return nullptr;
  } // TODO
  const int GetNumAnnotations() const {
    return static_cast<int>(annotations.size());
  }
  hkaAnnotationTrackPtr GetAnnotation(int id) const {
    return hkaAnnotationTrackPtr(&annotations[id], false);
  }

  hkaAnimationType animType;
  float duration;
  mutable std::vector<xmlAnnotationTrack> annotations;
};

class xmlInterleavedAnimation
    : public xmlAnimation,
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

  const int GetNumOfTransformTracks() const {
    return static_cast<int>(transforms.size());
  }
  const int GetNumOfFloatTracks() const {
    return static_cast<int>(floats.size());
  }

  const char *GetClassName(hkXMLToolsets toolset) const {
    if (toolset == HK550)
      return "hkaInterleavedSkeletalAnimation";
    else
      return "hkaInterleavedUncompressedAnimation";
  }

  void GetTrack(int trackID, int frame, float, TrackType type,
                Vector4A16 &out) const {
    switch (type) {
    case hkaAnimation::Rotation:
      out = transforms[trackID]->at(frame).rotation;
      break;
    case hkaAnimation::Position:
      out = reinterpret_cast<const Vector4A16 &>(
          transforms[trackID]->at(frame).position);
      out.W = 1.0f;
      break;
    case hkaAnimation::Scale:
      out = reinterpret_cast<const Vector4A16 &>(
          transforms[trackID]->at(frame).scale);
      out.W = 0.0f;
      break;
    }
  }

  void GetTransform(int trackID, int frame, float, hkQTransform &out) const {
    out = transforms[trackID]->at(frame);
  }

  int GetNumTransforms() const {
    if (transforms.size())
      return static_cast<int>(transforms[0]->size() * transforms.size());

    return 0;
  }

  int GetNumFloats() const {
    if (floats.size())
      return static_cast<int>(floats[0]->size() * floats.size());

    return 0;
  }

  const hkQTransform *GetTransform(int id) const {
    const int numTracks = static_cast<int>(transforms.size());
    return &transforms[id % numTracks]->at(id / numTracks);
  }

  float GetFloat(int id) const {
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
  std::vector<short> transformTrackToBoneIndices;
  std::vector<short> floatTrackToFloatSlotIndices;
  std::vector<short> partitionIndices;
  BlendHint blendHint = NORMAL;

  const char *GetSkeletonName() const { return skeletonName.c_str(); }
  const hkaAnimation *GetAnimation() const { return animation; }
  BlendHint GetBlendHint() const { return blendHint; }
  const int GetNumTransformTrackToBoneIndices() const {
    return static_cast<int>(transformTrackToBoneIndices.size());
  }
  const short GetTransformTrackToBoneIndex(int id) const {
    return transformTrackToBoneIndices[id];
  }
  const int GetNumFloatTrackToFloatSlotIndices() const {
    return static_cast<int>(floatTrackToFloatSlotIndices.size());
  }
  const short GetFloatTrackToFloatSlotIndex(int id) const {
    return floatTrackToFloatSlotIndices[id];
  }
  const int GetNumPartitionIndices() const {
    return static_cast<int>(partitionIndices.size());
  }
  const short GetPartitionIndex(int id) const { return partitionIndices[id]; }
};

struct xmlEnvironmentVariable {
  std::string name, value;
};

class xmlEnvironment : public hkxEnvironmentInternalInterface {
  DECLARE_XMLCLASS(xmlEnvironment, hkxEnvironment);
  std::vector<xmlEnvironmentVariable> variables;

  const int GetNumVars() const { return static_cast<int>(variables.size()); }
  hkxEnvironmentVariable GetVar(int id) const {
    return {variables[id].name.c_str(), variables[id].value.c_str()};
  }
};