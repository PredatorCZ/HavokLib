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
#include "hkaAnimation.h"
#include "hkaSplineDecompressor.h"

template<class C> struct hkaSplineCompressedAnimation_t : hkaSplineCompressedAnimationInternalInterface, hkaSkeletalAnimation_t<typename C::parentClass>
{
	typedef C value_type;
	typedef hkaSkeletalAnimation_t<typename C::parentClass> parent;
	hkClassConstructor(hkaSplineCompressedAnimation_t);
	void SwapEndian() { hkaSkeletalAnimation_t<typename C::parentClass>::SwapEndian(); static_cast<value_type *>(Data)->SwapEndian(masterBuffer); }
	void Process() { decomp.Assign(this); }
	hkaSplineDecompressor decomp;

	int GetNumFrames() const { return static_cast<value_type *>(Data)->GetNumFrames(); }
	int GetNumBlocks() const { return static_cast<value_type *>(Data)->GetNumBlocks(); }
	int GetMaxFramesPerBlock() const { return static_cast<value_type *>(Data)->GetMaxFramesPerBlock(); }
	float GetBlockDuration() const { return static_cast<value_type *>(Data)->GetBlockDuration(); }
	float GetBlockInverseDuration() const { return static_cast<value_type *>(Data)->GetBlockInverseDuration(); }
	float GetFrameDuration() const { return static_cast<value_type *>(Data)->GetFrameDuration(); }
	char *GetData() const { return static_cast<value_type *>(Data)->GetData(masterBuffer); }
	hkRealArray<uint> GetBlockOffsets() const { return static_cast<value_type *>(Data)->GetBlockOffsets(masterBuffer); }
	hkRealArray<uint> GetFloatBlockOffsets() const { return static_cast<value_type *>(Data)->GetFloatBlockOffsets(masterBuffer); }
	hkRealArray<uint> GetTransformOffsets() const { return static_cast<value_type *>(Data)->GetTransformOffsets(masterBuffer); }
	hkRealArray<uint> GetFloatOffsets() const { return static_cast<value_type *>(Data)->GetFloatOffsets(masterBuffer); }
	bool IsDecoderSupported() const { return true; }
	ES_INLINE int GetLocalValues(int frame, float delta, float &localFrame) const
	{
		int blockID = frame / (GetMaxFramesPerBlock() - 1);
		
		if (blockID >= GetNumBlocks())
			blockID = GetNumBlocks() - 1;
		else if (blockID < 0)
			blockID = 0;

		localFrame = static_cast<float>(frame - (blockID * (GetMaxFramesPerBlock() - 1)));
		localFrame = (delta + localFrame) * GetFrameDuration();
		localFrame = localFrame * GetBlockInverseDuration() * (GetMaxFramesPerBlock() - 1);

		return blockID;
	}

	void GetTrack(int trackID, int frame, float delta, TrackType type, Vector4 &out) const 
	{
		float localFrame;
		int blockID = GetLocalValues(frame, delta, localFrame);

		decomp.blocks[blockID].GetTrack(trackID, localFrame, type, out);

	}
	void GetTransform(int trackID, int frame, float delta, hkQTransform &out) const 
	{
		float localFrame;
		int blockID = GetLocalValues(frame, delta, localFrame);

		decomp.blocks[blockID].GetTransform(trackID, localFrame, out);
	}
	bool IsTrackStatic(int trackID, TrackType type) const { return decomp.IsTrackStatic(trackID, type); }
	int GetNumInternalFrames() const { return GetNumFrames(); }
};

template<class C> using hkaSplineSkeletalAnimation_t = hkaSplineCompressedAnimation_t<C>;

template<
	template<class C>class _ipointer,
	template<template<class C>class __ipointer> class _parent
>struct hkaSplineCompressedSkeletalAnimation_t_shared : _parent<_ipointer>
{
	ES_FORCEINLINE int GetNumFrames() const { return numFrames; }
	ES_FORCEINLINE int GetNumBlocks() const { return numBlocks; }
	ES_FORCEINLINE int GetMaxFramesPerBlock() const { return maxFramesPerBlock; }
	ES_FORCEINLINE float GetBlockDuration() const { return blockDuration; }
	ES_FORCEINLINE float GetBlockInverseDuration() const { return blockInverseDuration; }
	ES_FORCEINLINE float GetFrameDuration() const { return frameDuration; }
	ES_FORCEINLINE char *GetData(char *masterBuffer) { return data.GetData(masterBuffer); }
	ES_FORCEINLINE hkRealArray<uint> GetBlockOffsets(char *masterBuffer) const { return { blockOffsets.GetData(masterBuffer), blockOffsets.count }; }
	ES_FORCEINLINE hkRealArray<uint> GetFloatBlockOffsets(char *masterBuffer) const { return { floatBlockOffsets.GetData(masterBuffer), floatBlockOffsets.count }; }
	ES_FORCEINLINE hkRealArray<uint> GetTransformOffsets(char *masterBuffer) const { return { transformOffsets.GetData(masterBuffer), transformOffsets.count }; }
	ES_FORCEINLINE hkRealArray<uint> GetFloatOffsets(char *masterBuffer) const { return { floatOffsets.GetData(masterBuffer), floatOffsets.count }; }

	void SwapEndian(char *masterBuffer)
	{
		FByteswapper(numFrames);
		FByteswapper(numBlocks);
		FByteswapper(maxFramesPerBlock);
		FByteswapper(maskAndQuantizationSize);
		FByteswapper(blockDuration);
		FByteswapper(blockInverseDuration);
		FByteswapper(frameDuration);
		FByteswapper(endian);
		FByteswapper(blockOffsets.count);
		FByteswapper(floatBlockOffsets.count);
		FByteswapper(transformOffsets.count);
		FByteswapper(floatOffsets.count);

		for (int i = 0; i < blockOffsets.count; i++)
			FByteswapper(blockOffsets.GetData(masterBuffer)[i]);

		for (int i = 0; i < floatBlockOffsets.count; i++)
			FByteswapper(floatBlockOffsets.GetData(masterBuffer)[i]);

		for (int i = 0; i < transformOffsets.count; i++)
			FByteswapper(transformOffsets.GetData(masterBuffer)[i]);

		for (int i = 0; i < floatOffsets.count; i++)
			FByteswapper(floatOffsets.GetData(masterBuffer)[i]);
	}
};

template<template<class C>class _ipointer, template<template<class C>class __ipointer> class _parent>struct hkaSplineCompressedAnimation550_tt : _parent<_ipointer>
{
	typedef _parent<_ipointer> parentClass;
	int numFrames;
	int numBlocks;
	int maxFramesPerBlock;
	int maskAndQuantizationSize;
	float blockDuration;
	float blockInverseDuration;
	float frameDuration;
	hkArray<uint, _ipointer> blockOffsets,
		floatBlockOffsets,
		transformOffsets,
		floatOffsets;
	hkArray<char, _ipointer> data;
	int endian;	
};

template<template<class C>class _ipointer> using hkaSplineCompressedSkeletalAnimation550_t_sharedData = hkaSplineCompressedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_t>;
template<template<class C>class _ipointer> using hkaSplineCompressedSkeletalAnimation550_rp_t_sharedData = hkaSplineCompressedAnimation550_tt<_ipointer, hkaSkeletalAnimation550_rp_t>;

template<template<class C>class _ipointer> using hkaSplineSkeletalAnimation550_t = hkaSplineCompressedSkeletalAnimation_t_shared<_ipointer, hkaSplineCompressedSkeletalAnimation550_t_sharedData>;
template<template<class C>class _ipointer> using hkaSplineSkeletalAnimation550_rp_t = hkaSplineCompressedSkeletalAnimation_t_shared<_ipointer, hkaSplineCompressedSkeletalAnimation550_rp_t_sharedData>;

template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation660_t : hkaSplineSkeletalAnimation550_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation660_rp_t : hkaSplineSkeletalAnimation550_rp_t<_ipointer> {};

template<template<class C>class _ipointer> using hkaSplineCompressedAnimation710_t_sharedData = hkaSplineCompressedAnimation550_tt<_ipointer, hkaAnimation710_t>;
template<template<class C>class _ipointer> using hkaSplineCompressedAnimation710_rp_t_sharedData = hkaSplineCompressedAnimation550_tt<_ipointer, hkaAnimation710_rp_t>;

template<template<class C>class _ipointer> using hkaSplineCompressedAnimation710_t = hkaSplineCompressedSkeletalAnimation_t_shared<_ipointer, hkaSplineCompressedAnimation710_t_sharedData>;
template<template<class C>class _ipointer> using hkaSplineCompressedAnimation710_rp_t = hkaSplineCompressedSkeletalAnimation_t_shared<_ipointer, hkaSplineCompressedAnimation710_rp_t_sharedData>;

template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2010_t : hkaSplineCompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2010_rp_t : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2011_t : hkaSplineCompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2011_rp_t : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2012_t : hkaSplineCompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2012_rp_t : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2013_t : hkaSplineCompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2013_rp_t : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2014_t : hkaSplineCompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2014_rp_t : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2015_t : hkaSplineCompressedAnimation710_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2015_rp_t : hkaSplineCompressedAnimation710_rp_t<_ipointer> {};

template<template<class C>class _ipointer> using hkaSplineCompressedAnimation2016_t_sharedData = hkaSplineCompressedAnimation550_tt<_ipointer, hkaAnimation2016_t>;
template<template<class C>class _ipointer> using hkaSplineCompressedAnimation2016_rp_t_sharedData = hkaSplineCompressedAnimation550_tt<_ipointer, hkaAnimation2016_rp_t>;

template<template<class C>class _ipointer> using hkaSplineCompressedAnimation2016_t = hkaSplineCompressedSkeletalAnimation_t_shared<_ipointer, hkaSplineCompressedAnimation2016_t_sharedData>;
template<template<class C>class _ipointer> using hkaSplineCompressedAnimation2016_rp_t = hkaSplineCompressedSkeletalAnimation_t_shared<_ipointer, hkaSplineCompressedAnimation2016_rp_t_sharedData>;

template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2017_t : hkaSplineCompressedAnimation2016_t<_ipointer> {};
template<template<class C>class _ipointer>struct hkaSplineCompressedAnimation2017_rp_t : hkaSplineCompressedAnimation2016_rp_t<_ipointer> {};