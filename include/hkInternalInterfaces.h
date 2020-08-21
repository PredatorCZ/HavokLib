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
#include "datas/binwritter_stream.hpp"
#include "uni/list_vector.hpp"

namespace std {
template <> struct hash<CRule> {
  size_t operator()(const CRule &s) const noexcept {
    return reinterpret_cast<const uint32 &>(s);
  }
};
} // namespace std

struct hkFixup {
  const IhkVirtualClass *destClass;
  size_t strOffset;
  size_t destination;

  hkFixup(size_t offset, size_t dest)
      : strOffset(offset), destination(dest), destClass() {}
  hkFixup(size_t offset, const IhkVirtualClass *dest)
      : strOffset(offset), destClass(dest), destination() {}
  hkFixup(size_t offset) : strOffset(offset), destClass(), destination() {}
};

struct hkFixups {
  std::vector<hkFixup> locals;
  std::vector<hkFixup> finals;
  std::vector<hkFixup> globals;
};

struct hkVirtualClass : IhkVirtualClass {
  JenHash hash[4]{};
  CRule rule;
  es::string_view className;
  IhkPackFile *header;

  void AddHash(JenHash hsh) {
    for (size_t i = 0; i < 4; i++) {
      if (!hash[i]) {
        hash[i] = hsh;
        return;
      } else if (hash[i] == hsh) {
        return;
      }
    }

    throw std::overflow_error("New hash exceeded reserverd limit.");
  }

  void AddHash(es::string_view name) { AddHash(JenkinsHash(name)); }

  bool HasHash(JenHash hsh) const {
    for (size_t i = 0; i < 4; i++) {
      if (!hash[i]) {
        return false;
      } else if (hash[i] == hsh) {
        return true;
      }
    }

    return false;
  }

  bool HasHash(es::string_view name) const {
    return HasHash(JenkinsHash(name));
  }

  virtual es::string_view GetClassName(hkToolset toolset) const { return className; }
  virtual void SwapEndian() = 0;
  virtual void Process(){};
  virtual void SetDataPointer(void *Ptr) = 0;
  virtual void ToXML(XMLHandle hdl) const {}
  virtual void Reflect(IhkVirtualClass *input) {}
  virtual void Save(BinWritterRef wr, hkFixups &fixups) const {};

  static hkVirtualClass *Create(JenHash hash, CRule rule);
};

template <class C> struct hkRealArray {
  const C *data;
  uint32 count;
};

struct hkRootLevelContainerInternalInterface : hkRootLevelContainer,
                                               hkVirtualClass {
  void ToXML(XMLHandle hdl) const override;
  static hkVirtualClass *Create(CRule rule);
};

struct hkaAnimationContainerInternalInterface : hkaAnimationContainer,
                                                hkVirtualClass {
  void ToXML(XMLHandle hdl) const override;
  static hkVirtualClass *Create(CRule rule);
};

struct hkaSkeletonInternalInterface : hkaSkeleton, hkVirtualClass {
  void ToXML(XMLHandle hdl) const override;
  static hkVirtualClass *Create(CRule rule);
};

struct hkaAnimationBindingInternalInterface : hkaAnimationBinding,
                                              hkVirtualClass {
  void ToXML(XMLHandle hdl) const override;
  static hkVirtualClass *Create(CRule rule);
};

struct hkaAnnotationTrackInternalInterface : hkaAnnotationTrack,
                                             hkVirtualClass {
  void ToXML(XMLHandle hdl) const override;
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

  hkaAnimationInternalInterface() { this->AddHash(hkaAnimation::GetHash()); }

  virtual void GetValue(uni::RTSValue &output, float time,
                        size_t trackID) const = 0;
  void ToXML(XMLHandle hdl) const override;

  uint32 FrameRate() const override { return frameRate; }

  es::string_view Name() const override { return ""; }
  MotionType_e MotionType() const override { return Relative; }

  es::string_view GetAnimationTypeName() const override {
    return GetReflectedEnum<hkaAnimationType>()[GetAnimationType()];
  }

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

  void ToXML(XMLHandle hdl) const override;
  static hkVirtualClass *Create(CRule rule);
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
  static hkVirtualClass *Create(CRule rule);
};

struct hkaWaveletCompressedAnimationInternalInterface
    : virtual hkaAnimationLerpSampler {

  // void ToXML(XMLHandle hdl) const;
  static hkVirtualClass *Create(CRule rule);
};

struct hkaSplineCompressedAnimationInternalInterface
    : virtual hkaAnimationInternalInterface {
  virtual char *GetData() const = 0;
  virtual hkRealArray<uint32> GetBlockOffsets() const = 0;
  virtual hkRealArray<uint32> GetFloatBlockOffsets() const = 0;
  virtual hkRealArray<uint32> GetTransformOffsets() const = 0;
  virtual hkRealArray<uint32> GetFloatOffsets() const = 0;
  virtual uint32 GetNumFrames() const = 0;
  virtual uint32 GetNumBlocks() const = 0;
  virtual uint32 GetMaxFramesPerBlock() const = 0;
  virtual float GetBlockDuration() const = 0;
  virtual float GetBlockInverseDuration() const = 0;
  virtual float GetFrameDuration() const = 0;

  // void ToXML(XMLHandle hdl) const;
  static hkVirtualClass *Create(CRule rule);
};

struct hkxEnvironmentInternalInterface : hkxEnvironment, hkVirtualClass {
  void ToXML(XMLHandle hdl) const override;
  static hkVirtualClass *Create(CRule rule);
};

struct hkaAnimatedReferenceFrameInternalInterface : hkaAnimatedReferenceFrame,
                                                    hkVirtualClass {
  uint32 frameRate;

  virtual float GetDuration() const = 0;
  virtual size_t GetNumFrames() const = 0;
  virtual const Vector4A16 &GetRefFrame(size_t id) const = 0;

  TrackType_e TrackType() const override {
    return uni::MotionTrack::PositionRotationScale;
  }
  size_t BoneIndex() const override { return -1; }
  void GetValue(uni::RTSValue &output, float time) const override;

  // void ToXML(XMLHandle hdl) const;
};

struct hkaDefaultAnimatedReferenceFrameInternalInterface
    : virtual hkaAnimatedReferenceFrameInternalInterface {
  // void ToXML(XMLHandle hdl) const;
  static hkVirtualClass *Create(CRule rule);
};
