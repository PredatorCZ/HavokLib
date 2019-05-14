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
//#include "hkaWaveletDecompressor.h"

template<class C> struct hkaWaveletCompressedAnimation_t : hkaSkeletalAnimation_t<typename C::parentClass>//, IhkaWaveletCompressed
{
	typedef C value_type;
	hkClassConstructor(hkaWaveletCompressedAnimation_t<C>);
	void SwapEndian() { hkaSkeletalAnimation_t<typename C::parentClass>::SwapEndian(); static_cast<value_type*>(Data)->SwapEndian(masterBuffer); }
	void Process() {}//{ decomp.Assign(this); }
	//hkaWaveletDecompressor decomp;
	const int GetNumOfPoses() const { return static_cast<value_type*>(Data)->GetNumOfPoses(); }
	const int GetBlockSize() const { return static_cast<value_type*>(Data)->GetBlockSize(); }
	const int GetQuantizedDataOffset() const { return static_cast<value_type*>(Data)->GetQuantizedDataOffset(); }
	const int GetStaticMaskOffset() const { return static_cast<value_type*>(Data)->GetStaticMaskOffset(); }
	const int GetStaticDataOffset() const { return static_cast<value_type*>(Data)->GetStaticDataOffset(); }
	const char *GetData() const { return static_cast<value_type*>(Data)->GetData(masterBuffer); }
	const int GetNumDynamicTracks() const { return static_cast<value_type*>(Data)->GetNumDynamicTracks(); }
	const int GetOffsetsOffset() const { return static_cast<value_type*>(Data)->GetOffsetsOffset(); }
	const int GetBitWidthOffset() const { return static_cast<value_type*>(Data)->GetBitWidthOffset(); }
	const int GetScalesOffset() const { return static_cast<value_type*>(Data)->GetScalesOffset(); }
	const int GetNumPreserved() const { return static_cast<value_type*>(Data)->GetNumPreserved(); }
	bool IsTrackStatic(int trackID, TrackType type) const { return false; }
	void GetTrack(int trackID, int frame, TrackType type, Vector4 &out) const {}
	void GetTransform(int trackID, int frame, hkQTransform &out) const {}
};

template<class C> using hkaWaveletCompressedSkeletalAnimation_t = hkaWaveletCompressedAnimation_t<C>;

template<
	template<class C>class _ipointer,
	template<template<class C>class __ipointer> class _parent
>struct hkaWaveletCompressedSkeletalAnimation_t_shared : _parent<_ipointer>
{
	ES_FORCEINLINE const int GetNumOfPoses() const { return numberOfPoses; }
	ES_FORCEINLINE const int GetBlockSize() const { return blockSize; }
	ES_FORCEINLINE const int GetQuantizedDataOffset() const { return quantizedDataIdx; }
	ES_FORCEINLINE const int GetStaticMaskOffset() const { return staticMaskIdx; }
	ES_FORCEINLINE const int GetStaticDataOffset() const { return staticDOFsIdx; }
	ES_FORCEINLINE const char *GetData(char *masterbuffer) const { return reinterpret_cast<const char*>(dataBuffer.GetData(masterbuffer)); }
	ES_FORCEINLINE const int GetNumDynamicTracks() const { return qFormat.numD; }
	ES_FORCEINLINE const int GetOffsetsOffset() const { return qFormat.offsetIdx; }
	ES_FORCEINLINE const int GetBitWidthOffset() const { return qFormat.bitWidthIdx; }
	ES_FORCEINLINE const int GetScalesOffset() const { return qFormat.scaleIdx; }
	ES_FORCEINLINE const int GetNumPreserved() const { return qFormat.preserved; }

	void SwapEndian(char *masterBuffer)
	{
		FByteswapper(numberOfPoses);
		FByteswapper(blockSize);
		FByteswapper(staticMaskIdx);
		FByteswapper(staticDOFsIdx);
		FByteswapper(blockIndexIdx);
		FByteswapper(blockIndexSize);
		FByteswapper(quantizedDataIdx);
		FByteswapper(quantizedDataSize);
		FByteswapper(qFormat);
	}
};

template<template<class C>class _ipointer, template<template<class C>class __ipointer> class _parent>struct hkaWaveletCompressedAnimation550_tt : _parent<_ipointer>
{
	typedef _parent<_ipointer> parentClass;
	int numberOfPoses;
	int blockSize;
	QuantizationFormat qFormat;
	int staticMaskIdx;
	int staticDOFsIdx;
	int blockIndexIdx;
	int blockIndexSize;
	int quantizedDataIdx;
	int quantizedDataSize;
	_ipointer<unsigned char> dataBuffer;
	int numDataBuffer;
};

template<template<class C>class _ipointer> using hkaWaveletCompressedSkeletalAnimation550_t_sharedData = hkaWaveletCompressedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_t>;
template<template<class C>class _ipointer> using hkaWaveletCompressedSkeletalAnimation550_rp_t_sharedData = hkaWaveletCompressedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_rp_t>;

template<template<class C>class _ipointer> using hkaWaveletCompressedSkeletalAnimation550_t = hkaWaveletCompressedSkeletalAnimation_t_shared<_ipointer, hkaWaveletCompressedSkeletalAnimation550_t_sharedData>;
template<template<class C>class _ipointer> using hkaWaveletCompressedSkeletalAnimation550_rp_t = hkaWaveletCompressedSkeletalAnimation_t_shared<_ipointer, hkaWaveletCompressedSkeletalAnimation550_rp_t_sharedData>;


template<template<class C>class _ipointer, template<template<class C>class __ipointer> class _parent>struct hkaWaveletCompressedAnimation660_tt : _parent<_ipointer>
{
	typedef _parent<_ipointer> parentClass;
	int numberOfPoses;
	int blockSize;
	QuantizationFormat qFormat;
	int staticMaskIdx;
	int staticDOFsIdx;
	int numStaticTransformDOFs;
	int numDynamicTransformDOFs;
	int blockIndexIdx;
	int blockIndexSize;
	int quantizedDataIdx;
	int quantizedDataSize;
	_ipointer<unsigned char> dataBuffer;
	int numDataBuffer;
};

template<template<class C>class _ipointer> using hkaWaveletCompressedAnimation660_t_sharedData = hkaWaveletCompressedAnimation660_tt<_ipointer, hkaAnimation660_t>;
template<template<class C>class _ipointer> using hkaWaveletCompressedAnimation660_rp_t_sharedData = hkaWaveletCompressedAnimation660_tt<_ipointer, hkaAnimation660_rp_t>;

template<template<class C>class _ipointer> using hkaWaveletCompressedAnimation660_t = hkaWaveletCompressedSkeletalAnimation_t_shared<_ipointer, hkaWaveletCompressedAnimation660_t_sharedData>;
template<template<class C>class _ipointer> using hkaWaveletCompressedAnimation660_rp_t = hkaWaveletCompressedSkeletalAnimation_t_shared<_ipointer, hkaWaveletCompressedAnimation660_rp_t_sharedData>;

template<template<class C>class _ipointer, template<template<class C>class __ipointer> class _parent>struct hkaWaveletCompressedAnimation710_tt : _parent<_ipointer>
{
	typedef _parent<_ipointer> parentClass;
	int numberOfPoses;
	int blockSize;
	QuantizationFormat qFormat;
	int staticMaskIdx;
	int staticDOFsIdx;
	int numStaticTransformDOFs;
	int numDynamicTransformDOFs;
	int blockIndexIdx;
	int blockIndexSize;
	int quantizedDataIdx;
	int quantizedDataSize;
	hkArray<unsigned char, _ipointer> dataBuffer;
};

template<template<class C>class _ipointer> using hkaWaveletCompressedAnimation710_t_sharedData = hkaWaveletCompressedAnimation710_tt<_ipointer, hkaAnimation710_t>;
template<template<class C>class _ipointer> using hkaWaveletCompressedAnimation710_rp_t_sharedData = hkaWaveletCompressedAnimation710_tt<_ipointer, hkaAnimation710_rp_t>;

template<template<class C>class _ipointer> using hkaWaveletCompressedAnimation710_t = hkaWaveletCompressedSkeletalAnimation_t_shared<_ipointer, hkaWaveletCompressedAnimation710_t_sharedData>;
template<template<class C>class _ipointer> using hkaWaveletCompressedAnimation710_rp_t = hkaWaveletCompressedSkeletalAnimation_t_shared<_ipointer, hkaWaveletCompressedAnimation710_rp_t_sharedData>;

template<template<class C>class _ipointer>struct hkaWaveletCompressedAnimation2010_t : hkaWaveletCompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaWaveletCompressedAnimation2010_rp_t : hkaWaveletCompressedAnimation710_rp_t<_ipointer> {};