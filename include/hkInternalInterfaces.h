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

struct hkVirtualClass : IhkVirtualClass
{
	JenHash hash;
	JenHash superHash;
	const char *namePtr;
	char *masterBuffer;
	IhkPackFile *header;

	virtual void SwapEndian() = 0;
	virtual void Process() {};
	virtual void SetDataPointer(void *Ptr) = 0;
	virtual void ToXML(XMLHandle hdl) const {};
};


struct hkRootLevelContainerInternalInterface : hkRootLevelContainer, hkVirtualClass
{
	void ToXML(XMLHandle hdl) const;
};

struct hkaAnimationContainerInternalInterface : hkaAnimationContainer, hkVirtualClass
{
	void ToXML(XMLHandle hdl) const;
};

struct hkaSkeletonInternalInterface : hkaSkeleton, hkVirtualClass
{
	void ToXML(XMLHandle hdl) const;
};

struct hkaAnnotationTrackInternalInterface : hkaAnnotationTrack, hkVirtualClass
{
	//void ToXML(XMLHandle hdl) const;
};

struct hkaAnimationInternalInterface : hkaAnimation, hkVirtualClass
{
	//void ToXML(XMLHandle hdl) const;
};

struct hkaDeltaCompressedAnimationInternalInterface : virtual hkaAnimationInternalInterface
{
	virtual const int GetNumOfPoses() const = 0;
	virtual const int GetBlockSize() const = 0;
	virtual const int GetQuantizedDataOffset() const = 0;
	virtual const int GetStaticMaskOffset() const = 0;
	virtual const int GetStaticDataOffset() const = 0;
	virtual const char *GetData() const = 0;
	virtual const int GetNumDynamicTracks() const = 0;
	virtual const int GetOffsetsOffset() const = 0;
	virtual const int GetBitWidthOffset() const = 0;
	virtual const int GetScalesOffset() const = 0;
	virtual const int GetNumPreserved() const = 0;

	//void ToXML(XMLHandle hdl) const;
};

