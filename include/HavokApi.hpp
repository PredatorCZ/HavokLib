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
#include "datas/VectorsSimd.hpp"
#include "datas/jenkinshash.hpp"
#include "datas/reflector.hpp"
#include "datas/string_view.hpp"
#include "uni/motion.hpp"
#include "uni/skeleton.hpp"

class BinReader;
struct IhkPackFile;
struct hkLocalFrame;
struct hkaAnimation;
struct hkaAnimationBinding;
struct hkaBoneAttachment;
struct hkaMeshBinding;
struct hkaAnimatedReferenceFrame;
struct hkaAnnotationTrack;
struct hkRootLevelContainer;
struct hkVirtualClass;

#define DECLARE_HKCLASS(classname)                                             \
  static const JenHash HASH = JenkinsHashC(#classname);

//////////////////////////////////////////////////////// Havok Interface Classes

enum hkXMLToolsets {
  HKUNKVER,
  HK550,
  HK660,
  HK710,
  HK2010_2,
  HK2011,
  HK2011_2,
  HK2012_2,
  HK2013,
  HK2014,
};

namespace pugi {
class xml_node;
class xml_document;
}; // namespace pugi
typedef pugi::xml_node XMLnode;

struct XMLHandle {
  XMLnode *node;
  hkXMLToolsets toolset;
};

struct IhkVirtualClass {
  virtual const void *GetPointer() const = 0;
  virtual ~IhkVirtualClass() {}
};

using hkQTransform = uni::RTSValue;

struct hkLocalFrameOnBone {
  const hkLocalFrame *localFrame;
  int boneIndex;
};

struct hkaPartition {
  DECLARE_REFLECTOR;
  std::string name;
  int16 startBoneIndex;
  uint16 numBones;
};

struct hkaSkeleton : IhkVirtualClass, uni::Skeleton {
  DECLARE_HKCLASS(hkaSkeleton)
  virtual size_t GetNumFloatSlots() const = 0;
  virtual es::string_view GetFloatSlot(size_t id) const = 0;
  virtual size_t GetNumLocalFrames() const = 0;
  virtual hkLocalFrameOnBone GetLocalFrame(size_t id) const = 0;
  virtual size_t GetNumPartitions() const = 0;
  virtual hkaPartition GetPartition(size_t id) const = 0;
  virtual size_t GetNumBones() const = 0;
  virtual es::string_view GetBoneName(size_t id) const = 0;
  virtual const hkQTransform *GetBoneTM(size_t id) const = 0;
  virtual int16 GetBoneParentID(size_t id) const = 0;
  virtual size_t GetNumReferenceFloats() const = 0;
  virtual float GetReferenceFloat(size_t id) const = 0;

  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumFloatSlots,
                                    es::string_view, &hkaSkeleton::GetFloatSlot>
      iteratorFloatSlots;
  typedef uni::VirtualIteratorProxy<
      hkaSkeleton, &hkaSkeleton::GetNumLocalFrames, hkLocalFrameOnBone,
      &hkaSkeleton::GetLocalFrame>
      iteratorLocalFrames;
  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumPartitions,
                                    hkaPartition, &hkaSkeleton::GetPartition>
      iteratorPartitions;
  typedef uni::VirtualIteratorProxy<hkaSkeleton,
                                    &hkaSkeleton::GetNumReferenceFloats, float,
                                    &hkaSkeleton::GetReferenceFloat>
      iteratorReferenceFloat;
  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumBones,
                                    es::string_view, &hkaSkeleton::GetBoneName>
      iteratorBoneNames;
  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumBones,
                                    const hkQTransform *,
                                    &hkaSkeleton::GetBoneTM>
      iteratorBoneTMs;
  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumBones,
                                    int16, &hkaSkeleton::GetBoneParentID>
      iteratorBoneParentIDs;

  iteratorFloatSlots FloatSlots() const { return iteratorFloatSlots(this); }
  iteratorLocalFrames LocalFrames() const { return iteratorLocalFrames(this); }
  iteratorPartitions Partitions() const { return iteratorPartitions(this); }
  iteratorReferenceFloat ReferenceFloats() const {
    return iteratorReferenceFloat(this);
  }
  iteratorBoneNames BoneNames() const { return iteratorBoneNames(this); }
  iteratorBoneTMs BoneTransforms() const { return iteratorBoneTMs(this); }
  iteratorBoneParentIDs BoneParentIDs() const {
    return iteratorBoneParentIDs(this);
  }
};

struct hkNamedVariant {
  es::string_view name;
  es::string_view className;
  const IhkVirtualClass *pointer;

  template <class C> operator const C *() const {
    return dynamic_cast<const C *>(pointer);
  }
  operator es::string_view() const { return name; }
  operator JenHash() const { return JenkinsHash(className); }
  bool operator==(const JenHash iHash) const {
    return static_cast<JenHash>(*this) == iHash;
  }

  void ToXML(XMLHandle hdl) const;
};

struct hkRootLevelContainer : uni::Vector<hkNamedVariant>, IhkVirtualClass {
  DECLARE_HKCLASS(hkRootLevelContainer)
};

struct hkaAnimationContainer : IhkVirtualClass {
  DECLARE_HKCLASS(hkaAnimationContainer)

  virtual size_t GetNumSkeletons() const = 0;
  virtual const hkaSkeleton *GetSkeleton(size_t id) const = 0;
  virtual size_t GetNumAnimations() const = 0;
  virtual const hkaAnimation *GetAnimation(size_t id) const = 0;
  virtual size_t GetNumBindings() const = 0;
  virtual const hkaAnimationBinding *GetBinding(size_t id) const = 0;
  virtual size_t GetNumAttachments() const = 0;
  virtual const hkaBoneAttachment *GetAttachment(size_t id) const = 0;
  virtual size_t GetNumSkins() const = 0;
  virtual const hkaMeshBinding *GetSkin(size_t id) const = 0;

  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumSkeletons,
      const hkaSkeleton *, &hkaAnimationContainer::GetSkeleton>
      iteratorSkeletons;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumAnimations,
      const hkaAnimation *, &hkaAnimationContainer::GetAnimation>
      iteratorAnimations;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumBindings,
      const hkaAnimationBinding *, &hkaAnimationContainer::GetBinding>
      iteratorBindings;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumAttachments,
      const hkaBoneAttachment *, &hkaAnimationContainer::GetAttachment>
      iteratorAttachments;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumSkins,
      const hkaMeshBinding *, &hkaAnimationContainer::GetSkin>
      iteratorMeshBinds;

  const iteratorSkeletons Skeletons() const { return iteratorSkeletons(this); }
  const iteratorAnimations Animations() const {
    return iteratorAnimations(this);
  }
  const iteratorBindings Bindings() const { return iteratorBindings(this); }
  const iteratorAttachments Attachments() const {
    return iteratorAttachments(this);
  }
  const iteratorMeshBinds MeshBinds() const { return iteratorMeshBinds(this); }
};

// ADDITIVE_DEPRECATED since 2010
REFLECTOR_CREATE(BlendHint, ENUM, 1, 8, NORMAL, ADDITIVE_DEPRECATED, ADDITIVE)

struct hkaAnimationBinding : IhkVirtualClass {
  DECLARE_HKCLASS(hkaAnimationBinding)

  virtual es::string_view GetSkeletonName() const = 0;
  virtual const hkaAnimation *GetAnimation() const = 0;
  virtual BlendHint GetBlendHint() const = 0;
  virtual size_t GetNumTransformTrackToBoneIndices() const = 0;
  virtual int16 GetTransformTrackToBoneIndex(size_t id) const = 0;
  virtual size_t GetNumFloatTrackToFloatSlotIndices() const = 0;
  virtual int16 GetFloatTrackToFloatSlotIndex(size_t id) const = 0;
  virtual size_t GetNumPartitionIndices() const = 0;
  virtual int16 GetPartitionIndex(size_t id) const = 0;

  typedef uni::VirtualIteratorProxy<
      hkaAnimationBinding,
      &hkaAnimationBinding::GetNumTransformTrackToBoneIndices, int16,
      &hkaAnimationBinding::GetTransformTrackToBoneIndex>
      iteratorTransformTrackToBoneIndices;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationBinding,
      &hkaAnimationBinding::GetNumFloatTrackToFloatSlotIndices, int16,
      &hkaAnimationBinding::GetFloatTrackToFloatSlotIndex>
      iteratorFloatTrackToFloatSlotIndices;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationBinding, &hkaAnimationBinding::GetNumPartitionIndices, int16,
      &hkaAnimationBinding::GetPartitionIndex>
      iteratorNumPartitionIndices;

  const iteratorTransformTrackToBoneIndices
  TransformTrackToBoneIndices() const {
    return iteratorTransformTrackToBoneIndices(this);
  }
  const iteratorFloatTrackToFloatSlotIndices
  FloatTrackToFloatSlotIndices() const {
    return iteratorFloatTrackToFloatSlotIndices(this);
  }
  const iteratorNumPartitionIndices PartitionIndices() const {
    return iteratorNumPartitionIndices(this);
  }
};

REFLECTOR_CREATE(hkaAnimationType, ENUM, 0, HK_UNKNOWN_ANIMATION,
                 HK_INTERLEAVED_ANIMATION, HK_DELTA_COMPRESSED_ANIMATION,
                 HK_WAVELET_COMPRESSED_ANIMATION, HK_MIRRORED_ANIMATION,
                 HK_SPLINE_COMPRESSED_ANIMATION,
                 HK_QUANTIZED_COMPRESSED_ANIMATION,
                 HK_PREDICTIVE_COMPRESSED_ANIMATION,
                 HK_REFERENCE_POSE_ANIMATION);

// event trigger
struct hkaAnnotationFrame {
  float time;
  es::string_view text;
};

struct hkaAnnotationTrack : uni::Vector<hkaAnnotationFrame>, IhkVirtualClass {
  DECLARE_HKCLASS(hkaAnnotationTrack)

  virtual es::string_view GetName() const = 0;
  operator es::string_view() const { return GetName(); }
};

using hkaAnnotationTrackPtr = uni::Element<hkaAnnotationTrack>;

struct hkaAnimation : uni::Motion, IhkVirtualClass {
  DECLARE_HKCLASS(hkaAnimation)

  virtual es::string_view GetAnimationTypeName() const = 0;
  virtual hkaAnimationType GetAnimationType() const = 0;
  virtual size_t GetNumOfTransformTracks() const = 0;
  virtual size_t GetNumOfFloatTracks() const = 0;
  virtual const hkaAnimatedReferenceFrame *GetExtractedMotion() const = 0;
  virtual size_t GetNumAnnotations() const = 0;
  virtual hkaAnnotationTrackPtr GetAnnotation(size_t id) const = 0;

  typedef uni::VirtualIteratorProxy<hkaAnimation, &hkaAnimation::GetNumAnnotations,
                      hkaAnnotationTrackPtr, &hkaAnimation::GetAnnotation>
      interatorAnnotation;

  const interatorAnnotation Annotations() const {
    return interatorAnnotation(this);
  }
};

struct hkxEnvironmentVariable {
  es::string_view name;
  es::string_view value;
};

struct hkxEnvironment : IhkVirtualClass, virtual uni::Vector<hkxEnvironmentVariable> {
  DECLARE_HKCLASS(hkxEnvironment)
};

enum class hkaAnimatedReferenceFrameType : uint8 {
  UNKNOWN,
  DEFAULT,
  PARAMETRIC
};

struct hkaAnimatedReferenceFrame : IhkVirtualClass, uni::MotionTrack {
  virtual Vector4 GetUp() const = 0;
  virtual Vector4 GetForward() const = 0;
  virtual hkaAnimatedReferenceFrameType GetType() const = 0;
};
struct hkaBoneAttachment : IhkVirtualClass {};
struct hkaMeshBinding : IhkVirtualClass {};

struct IhkPackFile {
  typedef std::unique_ptr<IhkVirtualClass> VirtualClass;
  typedef std::vector<VirtualClass> VirtualClasses;
  typedef std::vector<IhkVirtualClass *> VirtualClassesRef;

private:
  template <class _Ty>
  int _ExportXML(const _Ty *fileName, hkXMLToolsets toolsetVersion);

  void _GenerateXML(pugi::xml_document &doc, hkXMLToolsets toolsetVersion);

public:
  IhkPackFile();
  virtual VirtualClasses &GetAllClasses() = 0;
  virtual int32 GetVersion() const = 0;
  virtual ~IhkPackFile() {};

  VirtualClassesRef GetClasses(es::string_view hkClassName) {
    return GetClasses(JenkinsHash(hkClassName));
  }

  VirtualClassesRef GetClasses(JenHash hash);

  hkRootLevelContainer *GetRootLevelContainer() {
    return dynamic_cast<hkRootLevelContainer *>(
        GetClasses(hkRootLevelContainer::HASH)[0]);
  }

  const IhkVirtualClass *GetClass(const void *ptr);

  static IhkPackFile *Create(const std::string &fileName,
                             bool suppressErrors = false);
  static IhkPackFile *Create(es::string_view fileName,
                             bool suppressErrors = false);

  int ExportXML(const wchar_t *fileName, hkXMLToolsets toolsetVersion) {
    return _ExportXML(fileName, toolsetVersion);
  }
  int ExportXML(const char *fileName, hkXMLToolsets toolsetVersion) {
    return _ExportXML(fileName, toolsetVersion);
  }

  // Internal use only
  static hkVirtualClass *ConstructClass(JenHash hash);
};
