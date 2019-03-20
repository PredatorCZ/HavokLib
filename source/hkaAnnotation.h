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

template<class C> struct hkaAnnotation_t : hkaAnnotationTrackInternalInterface
{
	C *Data;
	hkClassConstructor_nohash(hkaAnnotation_t<C>);
	void SwapEndian() { Data->SwapEndian(masterBuffer); }
	virtual const char *GetName() const { return Data->name.GetData(masterBuffer); };
	virtual const int GetNumAnnotations() const { return Data->GetNumAnnotations(); };
	virtual Annotation GetAnnotation(int id) const { return Data->GetAnnotation(id, masterBuffer); }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// hkaAnnotation
template<template<class C>class _ipointer>struct hkaAnnotation
{
	float time;
	_ipointer<const char> text;
};

template<template<class C>class _ipointer>struct hkaAnnotationTrack1
{
	_ipointer<const char> name;
	_ipointer<hkaAnnotation<_ipointer>> annotations;
	int numAnnotations;
};

template<template<class C>class _ipointer>struct hkaAnnotationTrack2
{
	_ipointer<const char> name;
	hkArray<hkaAnnotation<_ipointer>, _ipointer> annotations;
};

template<
	template<class C>class _ipointer,
	template<template<class C>class __ipointer> class _parent
>struct hkaAnnotation_t_shared : _parent<_ipointer>
{
	ES_FORCEINLINE const char *GetName() const { return name.GetData(masterBuffer); }

	enablePtrPair(annotations) GetNumAnnotations() const { return numAnnotations; }
	enablehkArray(annotations) GetNumAnnotations() const { return annotations.count; }

	enablePtrPairRef(annotations) GetNumAnnotations() { return numAnnotations; }
	enablehkArrayRef(annotations) GetNumAnnotations() { return annotations.count; }

	hkaAnnotationTrack::Annotation GetAnnotation(int id, char *masterBuffer) const
	{
		const hkaAnnotation<_ipointer> &ano = annotations.GetData(masterBuffer)[id];
		return hkaAnnotationTrack::Annotation{ ano.time, ano.text.GetData(masterBuffer) };
	}

	ES_FORCEINLINE void SwapEndian(char *masterBuffer)
	{
		FByteswapper(GetNumAnnotations());

		for (int a = 0; a < GetNumAnnotations(); a++)
		{
			FByteswapper(annotations.GetData(masterBuffer)[a].time);
		}
	}

};

