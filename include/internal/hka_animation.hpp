/*  Havok Format Library
    Copyright(C) 2016-2022 Lukas Cone

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
#include "hk_internal_api.hpp"
#include "hklib/hka_animation.hpp"

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

struct hkaDeltaCompressedAnimationInternalInterface;
struct hkaInterleavedAnimationInternalInterface;
struct hkaSplineCompressedAnimationInternalInterface;
struct hkaWaveletCompressedAnimationInternalInterface;

struct hkaAnimationInternalInterface : hkaAnimation, hkVirtualClass {
  mutable uint32 frameRate;

  hkaAnimationInternalInterface() { this->AddHash(hkaAnimation::GetHash()); }

  virtual operator hkaDeltaCompressedAnimationInternalInterface const *() const { return nullptr; }
  virtual operator hkaInterleavedAnimationInternalInterface const *() const { return nullptr; }
  virtual operator hkaSplineCompressedAnimationInternalInterface const *() const { return nullptr; }
  virtual operator hkaWaveletCompressedAnimationInternalInterface const *() const { return nullptr; }
  operator hkaAnimation const *() const { return this; }

  virtual void GetValue(uni::RTSValue &output, float time,
                        size_t trackID) const = 0;
  void ToXML(XMLHandle hdl) const override;

  uint32 FrameRate() const override { return frameRate; }

  std::string Name() const override { return ""; }
  MotionType_e MotionType() const override { return Relative; }

  es::string_view GetAnimationTypeName() const override {
    return GetReflectedEnum<hkaAnimationType>()->names[GetAnimationType()];
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
