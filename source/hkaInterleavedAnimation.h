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
#include "hkaAnimation.h"

template<class C> struct hkaInterleavedUncompressedAnimation_t : hkaInterleavedAnimationInternalInterface, hkaSkeletalAnimation_t<typename C::parentClass>
{
	typedef C value_type;
	typedef hkaSkeletalAnimation_t<typename C::parentClass> parent;
	hkClassConstructor(hkaInterleavedUncompressedAnimation_t);
	void SwapEndian() { hkaSkeletalAnimation_t<typename C::parentClass>::SwapEndian(); static_cast<value_type *>(Data)->SwapEndian(masterBuffer); }

	void GetTrack(int trackID, int frame, float delta, TrackType type, Vector4 &out) const 
	{
		const hkQTransform *ctr = GetTransform(frame * GetNumOfTransformTracks() + trackID);

		switch (type)
		{
		case hkaAnimation::Rotation:
			out = ctr->rotation;
			break;
		case hkaAnimation::Position:
			out = reinterpret_cast<const Vector4 &>(ctr->position);
			out.W = 1.0f;
			break;
		case hkaAnimation::Scale:
			out = reinterpret_cast<const Vector4 &>(ctr->scale);
			out.W = 0.0f;
			break;
		}
	}

	void GetTransform(int trackID, int frame, float delta, hkQTransform &out) const { out = *GetTransform(frame * GetNumOfTransformTracks() + trackID); }

	int GetNumTransforms() const { return static_cast<value_type *>(Data)->NumTransforms(); }
	int GetNumFloats() const { return static_cast<value_type *>(Data)->NumFloats(); }

	const hkQTransform *GetTransform(int id) const { return static_cast<value_type *>(Data)->GetTransform(masterBuffer, id); }
	float GetFloat(int id) const { return static_cast<value_type *>(Data)->GetFloat(masterBuffer, id); }
};

template<class C> using hkaInterleavedSkeletalAnimation_t = hkaInterleavedUncompressedAnimation_t<C>;

template<
	template<class C>class _ipointer,
	template<template<class C>class __ipointer> class _parent
>struct hkaInterleavedAnimation_t_shared : _parent<_ipointer>
{
	enablePtrPair(transforms) NumTransforms() const { return numTransforms; }
	enablehkArray(transforms) NumTransforms() const { return transforms.count; }
	enablePtrPairRef(transforms) NumTransforms() { return numTransforms; }
	enablehkArrayRef(transforms) NumTransforms() { return transforms.count; }

	enablePtrPair(floats) NumFloats() const { return numFloats; }
	enablehkArray(floats) NumFloats() const { return floats.count; }
	enablePtrPairRef(floats) NumFloats() { return numFloats; }
	enablehkArrayRef(floats) NumFloats() { return floats.count; }

	ES_FORCEINLINE const hkQTransform *GetTransform(char *masterBuffer, int id) const { return &transforms.GetData(masterBuffer)[id]; }
	ES_FORCEINLINE float GetFloat(char *masterBuffer, int id) const { return floats.GetData(masterBuffer)[id]; }

	ES_FORCEINLINE void SwapEndian(char *masterBuffer)
	{
		FByteswapper(NumTransforms());
		FByteswapper(NumFloats());
		const int numTransforms = NumTransforms();
		const int numFloats = NumFloats();

		for (int i = 0; i < numTransforms; i++)
		{
			transforms.GetData(masterBuffer)[i].position.SwapEndian();
			transforms.GetData(masterBuffer)[i].rotation.SwapEndian();
			transforms.GetData(masterBuffer)[i].scale.SwapEndian();
		}

		for (int i = 0; i < numFloats; i++)
			FByteswapper(floats.GetData(masterBuffer)[i]);
	}
};

template<template<class C>class _ipointer, template<template<class C>class __ipointer> class _parent>struct hkaInterleavedAnimation550_tt : _parent<_ipointer>
{
	typedef _parent<_ipointer> parentClass;
	_ipointer<hkQTransform> transforms;
	int numTransforms;
	_ipointer<float> floats;
	int numFloats;
};

template<template<class C>class _ipointer> using hkaInterleavedAnimation550_t_sharedData = hkaInterleavedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_t>;
template<template<class C>class _ipointer> using hkaInterleavedAnimation550_rp_t_sharedData = hkaInterleavedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_rp_t>;

template<template<class C>class _ipointer> using hkaInterleavedSkeletalAnimation550_t = hkaInterleavedAnimation_t_shared<_ipointer, hkaInterleavedAnimation550_t_sharedData>;
template<template<class C>class _ipointer> using hkaInterleavedSkeletalAnimation550_rp_t = hkaInterleavedAnimation_t_shared<_ipointer, hkaInterleavedAnimation550_rp_t_sharedData>;

template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation660_t : hkaInterleavedSkeletalAnimation550_t<_ipointer> {};
template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation660_rp_t : hkaInterleavedSkeletalAnimation550_rp_t<_ipointer> {};


template<template<class C>class _ipointer, template<template<class C>class __ipointer> class _parent>struct hkaInterleavedAnimation710_tt : _parent<_ipointer>
{
	typedef _parent<_ipointer> parentClass;
	hkArray<hkQTransform, _ipointer> transforms;
	hkArray<float, _ipointer> floats;
};

template<template<class C>class _ipointer> using hkaInterleavedAnimation710_t_sharedData = hkaInterleavedAnimation710_tt<_ipointer, hkaAnimation710_t>;
template<template<class C>class _ipointer> using hkaInterleavedAnimation710_rp_t_sharedData = hkaInterleavedAnimation710_tt<_ipointer, hkaAnimation710_rp_t>;

template<template<class C>class _ipointer> using hkaInterleavedUncompressedAnimation710_t = hkaInterleavedAnimation_t_shared<_ipointer, hkaInterleavedAnimation710_t_sharedData>;
template<template<class C>class _ipointer> using hkaInterleavedUncompressedAnimation710_rp_t = hkaInterleavedAnimation_t_shared<_ipointer, hkaInterleavedAnimation710_rp_t_sharedData>;

template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2010_t : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2010_rp_t : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};

template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2011_t : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2011_rp_t : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};

template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2012_t : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2012_rp_t : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};

template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2013_t : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2013_rp_t : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};

template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2014_t : hkaInterleavedUncompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer> struct hkaInterleavedUncompressedAnimation2014_rp_t : hkaInterleavedUncompressedAnimation710_rp_t<_ipointer> {};