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
#include "hkInternalInterfaces.h"
#include "datas/disabler.hpp"

template<class C> struct hkaAnimationBinding_t : hkaAnimationBindingInternalInterface
{
	C *Data;
	hkClassConstructor(hkaAnimationBinding_t);
	void SwapEndian() { Data->SwapEndian(masterBuffer); }
	const char *GetSkeletonName() const { return Data->GetSkeletonName(masterBuffer); }
	const hkaAnimation *GetAnimation() const { return Data->GetAnimation(header, masterBuffer); }
	BlendHint GetBlendHint() const { return Data->GetBlendHint(); }
	const int GetNumTransformTrackToBoneIndices() const { return Data->GetNumTransformTrackToBoneIndices(); }
	const short GetTransformTrackToBoneIndex(int id) const { return Data->GetTransformTrackToBoneIndex(masterBuffer, id); }
	const int GetNumFloatTrackToFloatSlotIndices() const { return Data->GetNumFloatTrackToFloatSlotIndices(); }
	const short GetFloatTrackToFloatSlotIndex(int id) const { return Data->GetFloatTrackToFloatSlotIndex(masterBuffer, id); }
	const int GetNumPartitionIndices() const { return Data->GetNumPartitionIndices(); }
	const short GetPartitionIndex(int id) const { return Data->GetPartitionIndex(masterBuffer, id); }
};

template<
	template<class C>class _ipointer,
	template<template<class C>class __ipointer> class _parent
>struct hkaAnimationBinding_t_shared : _parent<_ipointer>
{
	ES_FORCEINLINE const hkaAnimation *GetAnimation(IhkPackFile *header, char *masterBuffer) const { return dynamic_cast<const hkaAnimation *>(header->GetClass(animation.GetData(masterBuffer))); }
	ES_FORCEINLINE BlendHint GetBlendHint() const { return blendHint; }
	ES_FORCEINLINE const short GetTransformTrackToBoneIndex(char *masterBuffer, int id) const { return transformTrackToBoneIndices.GetData(masterBuffer)[id]; }
	ES_FORCEINLINE const short GetFloatTrackToFloatSlotIndex(char *masterBuffer, int id) const { return floatTrackToFloatSlotIndices.GetData(masterBuffer)[id]; }

	ADD_DISABLERS(_parent<_ipointer>, noPartitions, noSkeletonName);

	enablePtrPair(transformTrackToBoneIndices) GetNumTransformTrackToBoneIndices() const { return numTransformTrackToBoneIndices; }
	enablehkArray(transformTrackToBoneIndices) GetNumTransformTrackToBoneIndices() const { return transformTrackToBoneIndices.count; }
	enablePtrPairRef(transformTrackToBoneIndices) GetNumTransformTrackToBoneIndices() { return numTransformTrackToBoneIndices; }
	enablehkArrayRef(transformTrackToBoneIndices) GetNumTransformTrackToBoneIndices() { return transformTrackToBoneIndices.count; }

	enablePtrPair(floatTrackToFloatSlotIndices) GetNumFloatTrackToFloatSlotIndices() const { return numFloatTrackToFloatSlotIndices; }
	enablehkArray(floatTrackToFloatSlotIndices) GetNumFloatTrackToFloatSlotIndices() const { return floatTrackToFloatSlotIndices.count; }
	enablePtrPairRef(floatTrackToFloatSlotIndices) GetNumFloatTrackToFloatSlotIndices() { return numFloatTrackToFloatSlotIndices; }
	enablehkArrayRef(floatTrackToFloatSlotIndices) GetNumFloatTrackToFloatSlotIndices() { return floatTrackToFloatSlotIndices.count; }

	disabledFunction(noPartitions, const int) ES_FORCEINLINE GetNumPartitionIndices() const { return 0; }
	enabledFunction(noPartitions, const int) ES_FORCEINLINE GetNumPartitionIndices() const { return GetNumPartitionIndices(); }
	enabledFunction(noPartitions, int &) ES_FORCEINLINE GetNumPartitionIndices() { return partitionIndices.count; }

	disabledFunction(noPartitions, const short) ES_FORCEINLINE GetPartitionIndex(char *, int) const { return -1; }
	enabledFunction(noPartitions, const short) ES_FORCEINLINE GetPartitionIndex(char *masterBuffer, int id) const { return partitionIndices.GetData(masterBuffer)[id]; }

	disabledFunction(noSkeletonName, const char*) ES_FORCEINLINE GetSkeletonName(char *) const { return nullptr; }
	enabledFunction(noSkeletonName, const char *) ES_FORCEINLINE GetSkeletonName(char *masterBuffer) const { return skeletonName.GetData(masterBuffer); }

	disabledFunction(noPartitions, void) ES_FORCEINLINE SwapPartitionIndices(char *) {}
	enabledFunction(noPartitions, void) ES_FORCEINLINE SwapPartitionIndices(char *masterBuffer)
	{
		FByteswapper(GetNumPartitionIndices());
		const int numPartitions = GetNumPartitionIndices();

		for (int i = 0; i < numPartitions; i++)
			FByteswapper(partitionIndices.GetData(masterBuffer)[i]);
	}

	void SwapEndian(char *masterBuffer)
	{
		FByteswapper(GetNumTransformTrackToBoneIndices());
		const int numTransformTrackToBoneIndices = GetNumTransformTrackToBoneIndices();

		for (int i = 0; i < numTransformTrackToBoneIndices; i++)
			FByteswapper(transformTrackToBoneIndices.GetData(masterBuffer)[i]);

		FByteswapper(GetNumFloatTrackToFloatSlotIndices());
		const int numFloatTrackToFloatSlotIndices = GetNumFloatTrackToFloatSlotIndices();

		for (int i = 0; i < numFloatTrackToFloatSlotIndices; i++)
			FByteswapper(floatTrackToFloatSlotIndices.GetData(masterBuffer)[i]);

		SwapPartitionIndices(masterBuffer);
	}
};

template<template<class C>class _ipointer>struct hkaAnimationBinding550_t_data
{
	_ipointer<hkaAnimation> animation;
	_ipointer<short> transformTrackToBoneIndices;
	int numTransformTrackToBoneIndices;
	_ipointer<short> floatTrackToFloatSlotIndices;
	int numFloatTrackToFloatSlotIndices;
	hkEnum<BlendHint, char> blendHint;
	void noPartitions();
	void noSkeletonName();
};

template<template<class C>class _ipointer>using hkaAnimationBinding550_t = hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding550_t_data>;
template<template<class C>class _ipointer>struct hkaAnimationBinding550_rp_t : hkaAnimationBinding550_t<_ipointer> {};

template<template<class C>class _ipointer>struct hkaAnimationBinding660_t_data
{
	_ipointer<char> skeletonName;
	_ipointer<hkaAnimation> animation;
	_ipointer<short> transformTrackToBoneIndices;
	int numTransformTrackToBoneIndices;
	_ipointer<short> floatTrackToFloatSlotIndices;
	int numFloatTrackToFloatSlotIndices;
	hkEnum<BlendHint, char> blendHint;
	void noPartitions();
};

template<template<class C>class _ipointer>using hkaAnimationBinding660_t = hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding660_t_data>;
template<template<class C>class _ipointer>struct hkaAnimationBinding660_rp_t : hkaAnimationBinding660_t<_ipointer> {};

template<template<class C>class _ipointer>struct hkaAnimationBinding710_t_data : hkReferenceObject<_ipointer>
{
	_ipointer<char> skeletonName;
	_ipointer<hkaAnimation> animation;
	hkArray<short, _ipointer> transformTrackToBoneIndices;
	hkArray<short, _ipointer> floatTrackToFloatSlotIndices;
	hkEnum<BlendHint, char> blendHint;
	void noPartitions();
};

template<template<class C>class _ipointer>using hkaAnimationBinding710_t = hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding710_t_data>;
template<template<class C>class _ipointer>struct hkaAnimationBinding710_rp_t : hkaAnimationBinding710_t<_ipointer> {};

template<template<class C>class _ipointer>struct hkaAnimationBinding2010_t : hkaAnimationBinding710_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimationBinding2010_rp_t : hkaAnimationBinding710_rp_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimationBinding2011_t : hkaAnimationBinding710_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimationBinding2011_rp_t : hkaAnimationBinding710_rp_t<_ipointer> {};

template<template<class C>class _ipointer>struct hkaAnimationBinding2012_t_data : hkReferenceObject<_ipointer>
{
	_ipointer<char> skeletonName;
	_ipointer<hkaAnimation> animation;
	hkArray<short, _ipointer> transformTrackToBoneIndices;
	hkArray<short, _ipointer> floatTrackToFloatSlotIndices;
	hkArray<short, _ipointer> partitionIndices;
	hkEnum<BlendHint, char> blendHint;
};

template<template<class C>class _ipointer>using hkaAnimationBinding2012_t = hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding2012_t_data>;
template<template<class C>class _ipointer>struct hkaAnimationBinding2012_rp_t : hkaAnimationBinding2012_t<_ipointer> {};

template<template<class C>class _ipointer>struct hkaAnimationBinding2013_t : hkaAnimationBinding2012_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimationBinding2013_rp_t : hkaAnimationBinding2012_rp_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimationBinding2014_t : hkaAnimationBinding2012_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimationBinding2014_rp_t : hkaAnimationBinding2012_rp_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimationBinding2015_t : hkaAnimationBinding2012_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimationBinding2015_rp_t : hkaAnimationBinding2012_rp_t<_ipointer> {};

template<template<class C>class _ipointer>struct hkaAnimationBinding2016_t_data : hkReferenceObject2016<_ipointer>
{
	_ipointer<char> skeletonName;
	_ipointer<hkaAnimation> animation;
	hkArray<short, _ipointer> transformTrackToBoneIndices;
	hkArray<short, _ipointer> floatTrackToFloatSlotIndices;
	hkArray<short, _ipointer> partitionIndices;
	hkEnum<BlendHint, char> blendHint;
};

template<template<class C>class _ipointer>using hkaAnimationBinding2016_t = hkaAnimationBinding_t_shared<_ipointer, hkaAnimationBinding2016_t_data>;
template<template<class C>class _ipointer>struct hkaAnimationBinding2016_rp_t : hkaAnimationBinding2016_t<_ipointer> {};

template<template<class C>class _ipointer>struct hkaAnimationBinding2017_t : hkaAnimationBinding2016_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaAnimationBinding2017_rp_t : hkaAnimationBinding2016_rp_t<_ipointer> {};