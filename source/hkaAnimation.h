/*	Havok Format Library
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
#include "HavokApi.hpp"
#include "hkObjectBase.h"
#include "datas/reflector.hpp"
#include "hkReflectorUtil.h"
#include "hkaAnnotation.h"
#include <map>

REFLECTOR_WOENUM_WREMAP(hkaAnimationType, HK_UNKNOWN_ANIMATION, HK_INTERLEAVED_ANIMATION, HK_DELTA_COMPRESSED_ANIMATION, HK_WAVELET_COMPRESSED_ANIMATION, HK_MIRRORED_ANIMATION, HK_SPLINE_COMPRESSED_ANIMATION, HK_QUANTIZED_COMPRESSED_ANIMATION, HK_PREDICTIVE_COMPRESSED_ANIMATION, HK_REFERENCE_POSE_ANIMATION);

struct AnimationType1
{
	REFLECTOR_ENUM_HASH(AnimationType, HK_UNKNOWN_ANIMATION, HK_INTERLEAVED_ANIMATION, HK_DELTA_COMPRESSED_ANIMATION, HK_WAVELET_COMPRESSED_ANIMATION, HK_MIRRORED_ANIMATION, HK_SPLINE_COMPRESSED_ANIMATION) //550, 660, 710
};
struct AnimationType2010
{
	REFLECTOR_ENUM_HASH(AnimationType, HK_UNKNOWN_ANIMATION, HK_INTERLEAVED_ANIMATION, HK_DELTA_COMPRESSED_ANIMATION, HK_WAVELET_COMPRESSED_ANIMATION, HK_MIRRORED_ANIMATION, HK_SPLINE_COMPRESSED_ANIMATION, HK_QUANTIZED_COMPRESSED_ANIMATION)
};
struct AnimationType2011
{
	REFLECTOR_ENUM_HASH(AnimationType, HK_UNKNOWN_ANIMATION, HK_INTERLEAVED_ANIMATION, HK_MIRRORED_ANIMATION, HK_SPLINE_COMPRESSED_ANIMATION, HK_QUANTIZED_COMPRESSED_ANIMATION, HK_PREDICTIVE_COMPRESSED_ANIMATION)
};
struct AnimationType2
{
	REFLECTOR_ENUM_HASH(AnimationType, HK_UNKNOWN_ANIMATION, HK_INTERLEAVED_ANIMATION, HK_MIRRORED_ANIMATION, HK_SPLINE_COMPRESSED_ANIMATION, HK_QUANTIZED_COMPRESSED_ANIMATION, HK_PREDICTIVE_COMPRESSED_ANIMATION, HK_REFERENCE_POSE_ANIMATION) //2012, 2013, 2014
};

template<class C> struct hkaSkeletalAnimation_t : virtual hkaAnimationInternalInterface
{
	C *Data;
	hkClassConstructor_nohash(hkaSkeletalAnimation_t<C>);
	void SwapEndian() { Data->SwapEndian(masterBuffer); }
	const char *GetAnimationTypeName() const { return Data->GetAnimationTypeName(); }
	const hkaAnimationType GetAnimationType() const { return Data->GetAnimationType(); }
	const float GetDuration() const { return Data->GetDuration(); }
	const int GetNumOfTransformTracks() const { return Data->GetNumOfTransformTracks(); }
	const int GetNumOfFloatTracks() const { return Data->GetNumOfFloatTracks(); }
	const hkaAnimatedReferenceFrame *GetExtractedMotion() const { return Data->GetExtractedMotion(masterBuffer, header); }
	const int GetNumAnnotations() const { return Data->GetNumAnnotations(); }
	hkaAnnotationTrackPtr GetAnnotation(int id) const { return Data->GetAnnotation(masterBuffer, id); }
};
template<class C>using hkaAnimation_t = hkaSkeletalAnimation_t<C>;

template<
	template<class C>class _ipointer, 
	class AniType, 
	template<template<class C>class __ipointer, class _AniType> class _parent
>struct hkaSkeletalAnimation_t_shared : _parent<_ipointer, AniType>
{
	ES_FORCEINLINE const char *GetAnimationTypeName() const { return hkaAnimationType_reflected[GetAnimationType()]; }
	ES_FORCEINLINE const hkaAnimationType GetAnimationType() const
	{
		const JenHash hsh = AnimationType1{}.AnimationType_hashed[static_cast<int>(animationType)];
		return hkaAnimationType_remap.at(hsh);
	}

	ES_FORCEINLINE const float GetDuration() const { return duration; }
	ES_FORCEINLINE const int GetNumOfTransformTracks() const { return numOfTransformTracks; }
	ES_FORCEINLINE const int GetNumOfFloatTracks() const { return numOfFloatTracks; }
	ES_FORCEINLINE const hkaAnimatedReferenceFrame *GetExtractedMotion(char *masterbuffer, IhkPackFile *header) const { return dynamic_cast<const hkaAnimatedReferenceFrame*>(header->GetClass(extractedMotion.GetData(masterbuffer))); }
	
	enablePtrPair(annotations) GetNumAnnotations() const { return numAnnotations; }
	enablehkArray(annotations) GetNumAnnotations() const { return annotations.count; }
	enablePtrPairRef(annotations) GetNumAnnotations() { return numAnnotations; }
	enablehkArrayRef(annotations) GetNumAnnotations() { return annotations.count; }

	typedef typename _parent<_ipointer, AniType>::annot_type annot_type;
	
	enablePtrPairArg(annotations, hkaAnnotationTrackPtr) GetAnnotation(char *masterbuffer, int id) const
	{
		hkaAnnotation_t<annot_type> *ano = new hkaAnnotation_t<annot_type>;
		ano->SetDataPointer(annotations.GetData(masterbuffer)[id].GetData(masterbuffer));
		ano->masterBuffer = masterbuffer;
		return hkaAnnotationTrackPtr(ano);
	}

	enablehkArrayArg(annotations, hkaAnnotationTrackPtr) GetAnnotation(char *masterbuffer, int id) const
	{
		hkaAnnotation_t<annot_type> *ano = new hkaAnnotation_t<annot_type>;
		ano->SetDataPointer(&annotations.GetData(masterbuffer)[id]);
		ano->masterBuffer = masterbuffer;
		return hkaAnnotationTrackPtr(ano);
	}

	enablePtrPairArg(annotations, void) SwapAnnotations(char *masterBuffer)
	{
		for (int a = 0; a < GetNumAnnotations(); a++)
		{
			annotations.GetData(masterBuffer)[a].GetData(masterBuffer)->SwapEndian(masterBuffer);
		}
	}

	enablehkArrayArg(annotations, void) SwapAnnotations(char *masterBuffer)
	{
		for (int a = 0; a < GetNumAnnotations(); a++)
		{
			annotations.GetData(masterBuffer)[a].SwapEndian(masterBuffer);
		}
	}

	ES_FORCEINLINE void SwapEndian(char *masterbuffer)
	{
		FByteswapper(reinterpret_cast<int&>(animationType));
		FByteswapper(duration);
		FByteswapper(numOfFloatTracks);
		FByteswapper(numOfTransformTracks);
		FByteswapper(GetNumAnnotations());
		SwapAnnotations(masterbuffer);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// hkaSkeletalAnimation550
template<template<class C>class _ipointer, class AniType>struct hkaSkeletalAnimation550_t_data : hkReferenceObject<_ipointer>
{
	hkEnum<typename AniType::AnimationType, int> animationType;
	float duration;
	int numOfTransformTracks;
	int numOfFloatTracks;
	_ipointer<hkaAnimatedReferenceFrame> extractedMotion;
	typedef hkaAnnotation_t_shared<_ipointer, hkaAnnotationTrack1> annot_type;
	mutable _ipointer<_ipointer<annot_type>> annotations;
	int numAnnotations;
};

#pragma pack(4)
template<template<class C>class _ipointer, class AniType>struct hkaSkeletalAnimation550_rp_t_data : hkReferenceObject_rp<_ipointer>
{
	hkEnum<typename AniType::AnimationType, int> animationType;
	float duration;
	int numOfTransformTracks;
	int numOfFloatTracks;
	_ipointer<hkaAnimatedReferenceFrame> extractedMotion;
	typedef hkaAnnotation_t_shared<_ipointer, hkaAnnotationTrack1> annot_type;
	mutable _ipointer<_ipointer<annot_type>> annotations;
	int numAnnotations;
};

template<class AniType>struct hkaSkeletalAnimation550_rp_t_data<hkPointerX64, AniType> : hkReferenceObject_rp<hkPointerX64>
{
	hkEnum<typename AniType::AnimationType, int> animationType;
	float duration;
	int numOfTransformTracks;
	int numOfFloatTracks;
	int _padding;
	hkPointerX64<hkaAnimatedReferenceFrame> extractedMotion;
	typedef hkaAnnotation_t_shared<hkPointerX64, hkaAnnotationTrack1> annot_type;
	mutable hkPointerX64<hkPointerX64<annot_type>> annotations;
	int numAnnotations;
};
#pragma pack()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// hkaAnimation2k_t
template<
	template<class C>class _ipointer, 
	class AniType, template<template<class C>class __ipointer>class _parent
>struct hkaAnimation2k_sharedData_t : _parent<_ipointer>
{
	hkEnum<typename AniType::AnimationType, int> animationType;
	float duration;
	int numOfTransformTracks;
	int numOfFloatTracks;
	_ipointer<hkaAnimatedReferenceFrame> extractedMotion;
	typedef hkaAnnotation_t_shared<_ipointer, hkaAnnotationTrack2> annot_type;
	mutable hkArray<annot_type, _ipointer> annotations;
};

template<template<class C>class _ipointer, class AniType>using hkaAnimation2k_t = hkaAnimation2k_sharedData_t<_ipointer, AniType, hkReferenceObject>;
template<template<class C>class _ipointer, class AniType>using hkaAnimation2k_rp_t = hkaAnimation2k_sharedData_t<_ipointer, AniType, hkReferenceObject_rp>;


template<template<class C>class _ipointer> using hkaSkeletalAnimation550_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType1, hkaSkeletalAnimation550_t_data>;
template<template<class C>class _ipointer> using hkaSkeletalAnimation550_rp_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType1, hkaSkeletalAnimation550_rp_t_data>;
template<template<class C>class _ipointer>struct hkaAnimation660_t : hkaSkeletalAnimation550_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimation660_rp_t : hkaSkeletalAnimation550_rp_t<_ipointer> {};

template<template<class C>class _ipointer>using hkaAnimation710_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType1, hkaAnimation2k_t>;
template<template<class C>class _ipointer>using hkaAnimation2010_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType2010, hkaAnimation2k_t >;
template<template<class C>class _ipointer>using hkaAnimation2011_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType2011, hkaAnimation2k_t >;
template<template<class C>class _ipointer>using hkaAnimation2012_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType2, hkaAnimation2k_t >;
template<template<class C>class _ipointer>struct hkaAnimation2013_t : hkaAnimation2012_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimation2014_t : hkaAnimation2012_t<_ipointer> {};

template<template<class C>class _ipointer>using hkaAnimation710_rp_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType1, hkaAnimation2k_rp_t >;
template<template<class C>class _ipointer>using hkaAnimation2010_rp_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType2010, hkaAnimation2k_rp_t >;
template<template<class C>class _ipointer>using hkaAnimation2011_rp_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType2011, hkaAnimation2k_rp_t >;
template<template<class C>class _ipointer>using hkaAnimation2012_rp_t = hkaSkeletalAnimation_t_shared<_ipointer, AnimationType2, hkaAnimation2k_rp_t >;
template<template<class C>class _ipointer>struct hkaAnimation2013_rp_t : hkaAnimation2012_rp_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimation2014_rp_t : hkaAnimation2012_rp_t<_ipointer> {};
