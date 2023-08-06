/*  Havok Format Library
    Copyright(C) 2016-2023 Lukas Cone

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
#include "hk_base.hpp"
#include "spike/uni/motion.hpp"

struct hkaAnimatedReferenceFrame;
struct hkaAnnotationTrack;

using hkaAnnotationTrackPtr = uni::Element<hkaAnnotationTrack>;

MAKE_ENUM(ENUM(hkaAnimationType), EMEMBER(HK_UNKNOWN_ANIMATION),
          EMEMBER(HK_INTERLEAVED_ANIMATION),
          EMEMBER(HK_DELTA_COMPRESSED_ANIMATION),
          EMEMBER(HK_WAVELET_COMPRESSED_ANIMATION),
          EMEMBER(HK_MIRRORED_ANIMATION),
          EMEMBER(HK_SPLINE_COMPRESSED_ANIMATION),
          EMEMBER(HK_QUANTIZED_COMPRESSED_ANIMATION),
          EMEMBER(HK_PREDICTIVE_COMPRESSED_ANIMATION),
          EMEMBER(HK_REFERENCE_POSE_ANIMATION));

struct hkaAnimation : uni::Motion, IhkVirtualClass {
  DECLARE_HKCLASS(hkaAnimation)

  virtual std::string_view GetAnimationTypeName() const = 0;
  virtual hkaAnimationType GetAnimationType() const = 0;
  virtual size_t GetNumOfTransformTracks() const = 0;
  virtual size_t GetNumOfFloatTracks() const = 0;
  virtual const hkaAnimatedReferenceFrame *GetExtractedMotion() const = 0;
  virtual size_t GetNumAnnotations() const = 0;
  virtual hkaAnnotationTrackPtr GetAnnotation(size_t id) const = 0;

  typedef uni::VirtualIteratorProxy<
      hkaAnimation, &hkaAnimation::GetNumAnnotations, hkaAnnotationTrackPtr,
      &hkaAnimation::GetAnnotation>
      interatorAnnotation;

  const interatorAnnotation Annotations() const {
    return interatorAnnotation(this);
  }
};
