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
#include "hka_annotationtrack.hpp"
#include "hklib/hka_animatedreferenceframe.hpp"
#include "internal/hka_animation.hpp"
#include "reflector_util.hpp"
#include <unordered_map>

static const std::unordered_map<JenHash, hkaAnimationType>
    hkaAnimationTypeRemaps{StaticForArgID(
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

template <class C, class parentClass>
struct hkaAnimation_t : virtual parentClass {
  uni::Element<C> data;
  // std::unique_ptr<hkaAnimationSaver> saver;

  void SetDataPointer(void *ptr) override {
    data = {static_cast<C *>(ptr), false};
  }
  const void *GetPointer() const override { return data.get(); }

  void SwapEndian() override { data->SwapEndian(); }

  hkaAnimationType GetAnimationType() const override {
    return data->GetAnimationType();
  }
  float Duration() const override { return data->GetDuration(); }
  size_t GetNumOfTransformTracks() const override {
    return data->GetNumOfTransformTracks();
  }
  size_t GetNumOfFloatTracks() const override {
    return data->GetNumOfFloatTracks(0);
  }
  const hkaAnimatedReferenceFrame *GetExtractedMotion() const override {
    return data->GetExtractedMotion(this->header);
  }
  size_t GetNumAnnotations() const override {
    return data->GetNumAnnotations(0);
  }
  hkaAnnotationTrackPtr GetAnnotation(size_t id) const override {
    return data->GetAnnotation(id);
  }
};

template <template <class C> class _ipointer, bool rp,
          template <template <class C> class __ipointer, bool _rp>
          class _parent>
struct hkaAnimation_t_shared : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parent_class;
  typedef typename parent_class::annot_type annot_type;
  typedef typename parent_class::anim_type anim_type;

  hkaAnimationType GetAnimationType() const {
    const JenHash hsh = anim_type::GetHashes()[this->animationType];
    return hkaAnimationTypeRemaps.at(hsh);
  }

  float GetDuration() const { return this->duration; }

  size_t GetNumOfTransformTracks() const { return this->numOfTransformTracks; }

  const hkaAnimatedReferenceFrame *
  GetExtractedMotion(IhkPackFile *header) const {
    return dynamic_cast<const hkaAnimatedReferenceFrame *>(
        header->GetClass(this->extractedMotion));
  }

  GetNum(OfFloatTracks);
  GetNum(Annotations);

  AccessMethod2(Annotation, hkaAnnotationTrackPtr{}) {
    hkaAnnotation_t<annot_type> *ano = new hkaAnnotation_t<annot_type>;
    ano->SetDataPointer(this->Annotations[id]);
    return hkaAnnotationTrackPtr(ano);
  }

  AccessMethod2Arr(Annotation, hkaAnnotationTrackPtr{}) {
    hkaAnnotation_t<annot_type> *ano = new hkaAnnotation_t<annot_type>;
    ano->SetDataPointer(&this->Annotations[id]);
    return hkaAnnotationTrackPtr(ano);
  }

  SwapMethod2(Annotations) {
    for (size_t a = 0; a < GetNumAnnotations(0); a++) {
      this->Annotations[a]->SwapEndian();
    }
  }

  SwapMethod2Arr(Annotations) {
    for (size_t a = 0; a < GetNumAnnotations(0); a++) {
      this->Annotations[a].SwapEndian();
    }
  }

  void SwapEndian() {
    FByteswapper(this->animationType);
    FByteswapper(this->duration);
    FByteswapper(this->numOfTransformTracks);
    FByteswapper(GetNumOfFloatTracks(0));
    FByteswapper(GetNumAnnotations(0));
    SwapAnnotations(0);
  }
};

template <template <class C> class _ipointer, bool rp>
struct hkaAnimation500_t_data : hkReferenceObject<_ipointer> {
  using annot_type = hkaAnnotation_t_shared<_ipointer, hkaAnnotationTrack1>;
  using anim_type = AnimationType1;

  uint32 animationType;
  float duration;
  uint32 numOfTransformTracks;
  _ipointer<hkaAnimatedReferenceFrame> extractedMotion;
  mutable _ipointer<_ipointer<annot_type>> Annotations;
  uint32 numAnnotations;

  GNU_PADDING(4);
};

#pragma MSC_RP_PACK(4)
template <template <class C> class _ipointer>
struct hkaAnimation500_t_data<_ipointer, true>
    : hkReferenceObject_rp<_ipointer> {
  using annot_type = hkaAnnotation_t_shared<_ipointer, hkaAnnotationTrack1>;
  using anim_type = AnimationType1;

  uint32 animationType;
  float duration;
  uint32 numOfTransformTracks;
  _ipointer<hkaAnimatedReferenceFrame> extractedMotion;
  mutable _ipointer<_ipointer<annot_type>> Annotations;
  uint32 numAnnotations;
};
#pragma pack()

template <template <class C> class _ipointer, bool rp>
struct hkaAnimation550_t_data : hkReferenceObject<_ipointer> {
  using annot_type = hkaAnnotation_t_shared<_ipointer, hkaAnnotationTrack1>;
  using anim_type = AnimationType1;

  uint32 animationType;
  float duration;
  uint32 numOfTransformTracks;
  uint32 numOfFloatTracks;
  _ipointer<hkaAnimatedReferenceFrame> extractedMotion;
  mutable _ipointer<_ipointer<annot_type>> Annotations;
  uint32 numAnnotations;

  GNU_PADDING(4);
};

#pragma MSC_RP_PACK(4)
template <template <class C> class _ipointer>
struct hkaAnimation550_t_data<_ipointer, true>
    : hkReferenceObject_rp<_ipointer> {
  using annot_type = hkaAnnotation_t_shared<_ipointer, hkaAnnotationTrack1>;
  using anim_type = AnimationType1;

  uint32 animationType;
  float duration;
  uint32 numOfTransformTracks;
  uint32 numOfFloatTracks;
  _ipointer<hkaAnimatedReferenceFrame> extractedMotion;
  mutable _ipointer<_ipointer<annot_type>> Annotations;
  uint32 numAnnotations;
};
#ifdef _MSC_VER
template <>
struct hkaAnimation550_t_data<esPointerX64, true>
    : hkReferenceObject_rp<esPointerX64> {
  using annot_type = hkaAnnotation_t_shared<esPointerX64, hkaAnnotationTrack1>;
  using anim_type = AnimationType1;

  uint32 animationType;
  float duration;
  uint32 numOfTransformTracks;
  uint32 numOfFloatTracks;
  uint32 _padding;
  esPointerX64<hkaAnimatedReferenceFrame> extractedMotion;
  mutable esPointerX64<esPointerX64<annot_type>> Annotations;
  uint32 numAnnotations;
};
#endif
#pragma pack()

template <template <class C> class _ipointer, class AniType,
          template <template <class C> class __ipointer> class _parent>
struct hkaAnimation700_data_t : _parent<_ipointer> {
  using annot_type = hkaAnnotation_t_shared<_ipointer, hkaAnnotationTrack2>;
  using anim_type = AniType;

  uint32 animationType;
  float duration;
  uint32 numOfTransformTracks;
  uint32 numOfFloatTracks;
  _ipointer<hkaAnimatedReferenceFrame> extractedMotion;
  mutable hkArray<annot_type, _ipointer> Annotations;
};

template <template <class C> class _ipointer, bool rp>
using hkaAnimation500 =
    hkaAnimation_t_shared<_ipointer, rp, hkaAnimation500_t_data>;

template <template <class C> class _ipointer, bool rp>
using hkaAnimation550 =
    hkaAnimation_t_shared<_ipointer, rp, hkaAnimation550_t_data>;

template <template <class C> class _ipointer, bool rp>
struct hkaAnimation700_t_data
    : hkaAnimation700_data_t<_ipointer, AnimationType1, hkReferenceObject> {};

template <template <class C> class _ipointer>
struct hkaAnimation700_t_data<_ipointer, true>
    : hkaAnimation700_data_t<_ipointer, AnimationType1, hkReferenceObject_rp> {
};

template <template <class C> class _ipointer, bool rp>
using hkaAnimation700 =
    hkaAnimation_t_shared<_ipointer, rp, hkaAnimation700_t_data>;

template <template <class C> class _ipointer, bool rp>
struct hkaAnimation2011_t_data
    : hkaAnimation700_data_t<_ipointer, AnimationType2, hkReferenceObject> {};

template <template <class C> class _ipointer>
struct hkaAnimation2011_t_data<_ipointer, true>
    : hkaAnimation700_data_t<_ipointer, AnimationType2, hkReferenceObject_rp> {
};

template <template <class C> class _ipointer, bool rp>
using hkaAnimation2011 =
    hkaAnimation_t_shared<_ipointer, rp, hkaAnimation2011_t_data>;

template <template <class C> class _ipointer, bool rp>
struct hkaAnimation2016_t_data
    : hkaAnimation700_data_t<_ipointer, AnimationType2, hkReferenceObject2016> {
};

template <template <class C> class _ipointer>
struct hkaAnimation2016_t_data<_ipointer, true>
    : hkaAnimation700_data_t<_ipointer, AnimationType2,
                             hkReferenceObject2016_rp> {};

template <template <class C> class _ipointer, bool rp>
using hkaAnimation2016 =
    hkaAnimation_t_shared<_ipointer, rp, hkaAnimation2016_t_data>;
