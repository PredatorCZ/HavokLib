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
#include "hk_internal_api.hpp"
#include "hklib/hka_animatedreferenceframe.hpp"

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
