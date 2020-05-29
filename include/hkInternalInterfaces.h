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
#pragma warning(disable : 4250)
#include "datas/endian.hpp"

#include "HavokApi.hpp"
#include "uni/list_vector.hpp"

struct hkVirtualClass : IhkVirtualClass {
  JenHash hash;
  JenHash superHash;
  es::string_view name;
  IhkPackFile *header;

  virtual es::string_view GetClassName(hkXMLToolsets toolset) const {
    return name;
  }
  virtual void SwapEndian() = 0;
  virtual void Process(){};
  virtual void SetDataPointer(void *Ptr) = 0;
  virtual void ToXML(XMLHandle hdl) const {};
};

template <class C> struct hkRealArray {
  const C *data;
  uint32 count;
};

struct hkRootLevelContainerInternalInterface : hkRootLevelContainer,
                                               hkVirtualClass {
  void ToXML(XMLHandle hdl) const;
};

struct hkaAnimationContainerInternalInterface : hkaAnimationContainer,
                                                hkVirtualClass {
  void ToXML(XMLHandle hdl) const;
};

class hkFullBone : public uni::Bone {
public:
  es::string_view name;
  const hkQTransform *tm;
  hkFullBone *parent;
  size_t id;

  TransformType TMType() const override { return TMTYPE_RTS; }
  void GetTM(uni::RTSValue &out) const override { out = *tm; }
  const Bone *Parent() const override { return parent; }
  size_t Index() const override { return id; }
  es::string_view Name() const override { return name; }
  operator uni::Element<const uni::Bone>() const {
    return {static_cast<const uni::Bone *>(this), false};
  }
};

struct hkaSkeletonInternalInterface : hkaSkeleton,
                                      hkVirtualClass,
                                      uni::VectorList<uni::Bone, hkFullBone> {
  void Process() override;
  uni::SkeletonBonesConst Bones() const override {
    return uni::SkeletonBonesConst(
        dynamic_cast<const uni::List<uni::Bone> *>(this), false);
  }
  void ToXML(XMLHandle hdl) const override;
};

struct hkaAnimationBindingInternalInterface : hkaAnimationBinding,
                                              hkVirtualClass {
  void ToXML(XMLHandle hdl) const;
};

struct hkaAnnotationTrackInternalInterface : hkaAnnotationTrack,
                                             hkVirtualClass {
  void ToXML(XMLHandle hdl) const;
};

struct hkaAnimationInternalInterface;

struct hkaAniTrackHandle : uni::MotionTrack {
  size_t index;
  const hkaAnimationInternalInterface *hdl;

  hkaAniTrackHandle(size_t id, const hkaAnimationInternalInterface *itf)
      : index(id), hdl(itf) {}

  TrackType_e TrackType() const override {
    return uni::MotionTrack::PositionRotationScale;
  }
  size_t BoneIndex() const override { return index; }
  void GetValue(uni::RTSValue &output, float time) const override;
};

struct hkaAniTrackHandleList : uni::List<uni::MotionTrack> {
  size_t numTracks;
  const hkaAnimationInternalInterface *hdl;

  hkaAniTrackHandleList(size_t num, const hkaAnimationInternalInterface *itf)
      : numTracks(num), hdl(itf) {}

  size_t Size() const override { return numTracks; }
  uni::Element<const uni::MotionTrack> At(size_t id) const override {
    return uni::Element<const uni::MotionTrack>(new hkaAniTrackHandle{id, hdl});
  }
};

struct hkaAnimationInternalInterface : virtual hkaAnimation, hkVirtualClass {
  mutable uint32 frameRate;
  virtual void GetValue(uni::RTSValue &output, float time,
                        size_t trackID) const = 0;
  void ToXML(XMLHandle hdl) const;

  uint32 FrameRate() const override { return frameRate; }

  es::string_view Name() const override { return ""; }
  MotionType_e MotionType() const override { return Relative; }

  uni::MotionTracksConst Tracks() const override {
    return uni::MotionTracksConst(
        new hkaAniTrackHandleList(GetNumOfTransformTracks(), this));
  }
};

struct hkaAnimationLerpSampler : hkaAnimationInternalInterface {
  mutable size_t numFrames;
  virtual void GetFrame(size_t trackID, int32 frame,
                        hkQTransform &out) const = 0;
  void GetValue(uni::RTSValue &output, float time,
                size_t trackID) const override;
};

struct hkaInterleavedAnimationInternalInterface
    : virtual hkaAnimationLerpSampler {
  virtual size_t GetNumTransforms() const = 0;
  virtual size_t GetNumFloats() const = 0;
  virtual const hkQTransform *GetTransform(size_t id) const = 0;
  virtual float GetFloat(size_t id) const = 0;
  // TODO floats

  void ToXML(XMLHandle hdl) const;
};

struct hkaDeltaCompressedAnimationInternalInterface
    : virtual hkaAnimationLerpSampler {
  virtual size_t GetNumOfPoses() const = 0;
  virtual size_t GetBlockSize() const = 0;
  virtual size_t GetQuantizedDataOffset() const = 0;
  virtual size_t GetStaticMaskOffset() const = 0;
  virtual size_t GetStaticDataOffset() const = 0;
  virtual const char *GetData() const = 0;
  virtual size_t GetNumDynamicTracks() const = 0;
  virtual size_t GetOffsetsOffset() const = 0;
  virtual size_t GetBitWidthOffset() const = 0;
  virtual size_t GetScalesOffset() const = 0;
  virtual size_t GetNumPreserved() const = 0;
  // void ToXML(XMLHandle hdl) const;
};

struct hkaWaveletCompressedAnimationInternalInterface
    : virtual hkaAnimationLerpSampler {
  // void ToXML(XMLHandle hdl) const;
};

struct hkaSplineCompressedAnimationInternalInterface
    : virtual hkaAnimationInternalInterface {
  virtual char *GetData() const = 0;
  virtual hkRealArray<uint32> GetBlockOffsets() const = 0;

  // void ToXML(XMLHandle hdl) const;
};

struct hkxEnvironmentInternalInterface : hkxEnvironment, hkVirtualClass {
  void ToXML(XMLHandle hdl) const;
};

struct hkaAnimatedReferenceFrameInternalInterface : hkaAnimatedReferenceFrame,
                                                    hkVirtualClass {
  uint32 frameRate;
  virtual float GetDuration() const = 0;
  virtual size_t GetNumFrames() const = 0;
  virtual const Vector4 *GetRefFrame(size_t id) const = 0;

  TrackType_e TrackType() const override {
    return uni::MotionTrack::PositionRotationScale;
  }
  size_t BoneIndex() const override { return -1; }
  void GetValue(uni::RTSValue &output, float time) const override;
  
  // void ToXML(XMLHandle hdl) const;
};
