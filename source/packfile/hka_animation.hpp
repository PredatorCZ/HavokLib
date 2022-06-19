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
#include "hklib/hk_packfile.hpp"
#include "hklib/hka_animatedreferenceframe.hpp"
#include "internal/hka_animation.hpp"
#include "internal/hka_annotationtrack.hpp"
#include "reflector_util.hpp"
#include <map>

#include "hka_animation.inl"

static const std::map<JenHash, hkaAnimationType> hkaAnimationTypeRemaps{
    StaticForArgID(
        _REFLECTOR_ADDN_REMAP, hkaAnimationType, HK_UNKNOWN_ANIMATION,
        HK_INTERLEAVED_ANIMATION, HK_DELTA_COMPRESSED_ANIMATION,
        HK_WAVELET_COMPRESSED_ANIMATION, HK_MIRRORED_ANIMATION,
        HK_SPLINE_COMPRESSED_ANIMATION, HK_QUANTIZED_COMPRESSED_ANIMATION,
        HK_PREDICTIVE_COMPRESSED_ANIMATION, HK_REFERENCE_POSE_ANIMATION)};

struct AnimationType1 {
  REFLECTOR_ENUM_HASH(AnimationType, HK_UNKNOWN_ANIMATION,
                      HK_INTERLEAVED_ANIMATION, HK_DELTA_COMPRESSED_ANIMATION,
                      HK_WAVELET_COMPRESSED_ANIMATION, HK_MIRRORED_ANIMATION,
                      HK_SPLINE_COMPRESSED_ANIMATION,
                      HK_QUANTIZED_COMPRESSED_ANIMATION) // < 2011
};

struct AnimationType2 {
  REFLECTOR_ENUM_HASH(AnimationType, HK_UNKNOWN_ANIMATION,
                      HK_INTERLEAVED_ANIMATION, HK_MIRRORED_ANIMATION,
                      HK_SPLINE_COMPRESSED_ANIMATION,
                      HK_QUANTIZED_COMPRESSED_ANIMATION,
                      HK_PREDICTIVE_COMPRESSED_ANIMATION,
                      HK_REFERENCE_POSE_ANIMATION)
};

struct hkaAnnotationTrackMidInterface : hkaAnnotationTrackInternalInterface {
  clgen::hkaAnnotationTrack::Interface interface;

  hkaAnnotationTrackMidInterface(clgen::LayoutLookup rules, char *data)
      : interface {
    data, rules
  } {
  }

  hkaAnnotationTrackMidInterface(
      const clgen::hkaAnnotationTrack::Interface &interface)
      : interface {
    interface
  } {
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override {
    clgen::EndianSwap(interface);

    {
      size_t numParts = interface.NumAnnotations();
      auto parts = interface.Annotations();

      for (size_t i = 0; i < numParts; i++, parts.Next()) {
        clgen::EndianSwap(parts);
      }
    }
  }

  es::string_view GetName() const override { return interface.Name(); };
  size_t Size() const override { return interface.NumAnnotations(); };
  const hkaAnnotationFrame At(size_t id) const override {
    auto item = interface.Annotations().Next(id);
    return {item.Time(), item.Text()};
  }
};

template <class ParentInterface>
struct hkaAnimationMidInterface : virtual ParentInterface {

  virtual clgen::hkaAnimation::Interface Anim() const = 0;

  hkaAnimationType GetAnimationType() const override {
    auto anim = Anim();
    uint32 animType = anim.AnimationType();
    auto animHash = anim.LayoutVersion() < HK2011_1
                        ? AnimationType1::GetHashes()[animType]
                        : AnimationType2::GetHashes()[animType];
    return hkaAnimationTypeRemaps.at(animHash);
  }
  float Duration() const override { return Anim().Duration(); }
  size_t GetNumOfTransformTracks() const override {
    return Anim().NumOfTransformTracks();
  }
  size_t GetNumOfFloatTracks() const override {
    return Anim().NumOfFloatTracks();
  }
  const hkaAnimatedReferenceFrame *GetExtractedMotion() const override {
    return safe_deref_cast<const hkaAnimatedReferenceFrame>(
        this->header->GetClass(Anim().ExtractedMotion().data));
  }
  size_t GetNumAnnotations() const override { return Anim().NumAnnotations(); }
  hkaAnnotationTrackPtr GetAnnotation(size_t id) const override {
    if (Anim().LayoutVersion() >= HK700) {
      return {new hkaAnnotationTrackMidInterface{
                  Anim().AnnotationsHK700().Next(id)},
              true};
    } else {
      return {
          new hkaAnnotationTrackMidInterface{*Anim().Annotations().Next(id)},
          true};
    }
  }

  void SwapEndian() override {
    auto base = Anim();
    clgen::EndianSwap(base);

    {
      size_t numParts = base.NumAnnotations();

      if (base.LayoutVersion() >= HK700) {
        auto parts = base.AnnotationsHK700();

        for (size_t i = 0; i < numParts; i++, parts.Next()) {
          hkaAnnotationTrackMidInterface{parts}.SwapEndian();
        }
      } else {
        auto parts = base.Annotations();

        for (size_t i = 0; i < numParts; i++, parts.Next()) {
          hkaAnnotationTrackMidInterface{*parts}.SwapEndian();
        }
      }
    }
  }
};
