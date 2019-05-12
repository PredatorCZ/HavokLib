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
#include "datas/flags.hpp"
#include <vector>
#include "datas/allocator_hybrid.hpp"

template<typename T> using hvector = std::vector<T, std::allocator_hybrid<T>>;

struct MasterTrack;

struct QuantizationFormat
{
	unsigned char maxBitWidth,
		preserved;
	int numD,
		offsetIdx,
		scaleIdx,
		bitWidthIdx;

	ES_FORCEINLINE void SwapEndian()
	{
		FByteswapper(numD);
		FByteswapper(offsetIdx);
		FByteswapper(scaleIdx);
		FByteswapper(bitWidthIdx);
	}
};

enum TrackType
{
	TT_DYNAMIC,
	TT_STATIC,
	TT_IDENTITY
};

class StaticMask
{
	esFlags<short> data;
	ES_INLINE TrackType GetTrackType(int offset)
	{
		return static_cast<TrackType>(static_cast<int>(data[offset]) | static_cast<int>((data[offset + 1] << 1)));
	}
public:
	enum FlagOffset
	{
		ttPos = 0,
		ttRot = 2,
		ttScale = 4,
		posZ = 6,
		posY,
		posX,
		rotW,
		rotZ,
		rotY,
		rotX,
		scaleZ,
		scaleY,
		scaleX
	};
	ES_INLINE TrackType GetPosTrackType() { return GetTrackType(ttPos); }
	ES_INLINE TrackType GetRotTrackType() { return GetTrackType(ttRot); }
	ES_INLINE TrackType GetScaleTrackType() { return GetTrackType(ttScale); }
	ES_INLINE bool UseSubTrack(FlagOffset type) { return data[type]; }
};

struct ILineTrack
{
	virtual float &GetFrame(int frame) = 0;
};

struct LineDynamicTrack : ILineTrack
{
	std::vector<float> items;
	float &GetFrame(int frame) { return items[frame]; }
};

struct LineStaticTrack : ILineTrack
{
	float track;
	float &GetFrame(int frame) { return track; }
	LineStaticTrack(float input) : track(input) {}
	LineStaticTrack() : track(0.0f) {}
};

template<class C> struct IVectorTrack
{
	virtual C GetVector(int frame) = 0;
	virtual bool IsStatic() = 0;
	virtual ~IVectorTrack() {}
};

template<class C> struct VectorDynamicTrack : IVectorTrack<C>
{
	static const int numComponents = sizeof(C) / 4;
	ILineTrack *tracks[numComponents];

	C GetVector(int frame);
	VectorDynamicTrack(StaticMask &mask, StaticMask::FlagOffset offset, MasterTrack &master);
	~VectorDynamicTrack();
	bool IsStatic() { return false; }
};

template<class C> struct VectorStaticTrack : IVectorTrack<C>
{
	C item;
	C GetVector(int frame) { return item; }
	bool IsStatic() { return true; }
};

struct MasterTrack
{
	IVectorTrack<Vector> *pos;
	IVectorTrack<Vector4> *rotation;
	IVectorTrack<Vector> *scale;

	std::vector<float*> staticFloats;
	std::vector<LineDynamicTrack*> dynamicTracks;

	MasterTrack(StaticMask &mask);
	~MasterTrack();
};

template<class C>
ES_INLINE C VectorDynamicTrack<C>::GetVector(int frame)
{
	C retval;

	retval.X = tracks[0]->GetFrame(frame);
	retval.Y = tracks[1]->GetFrame(frame);
	retval.Z = tracks[2]->GetFrame(frame);

	if (numComponents > 3)
	{
		retval[3] = tracks[3]->GetFrame(frame);
	}

	return retval;
}

template<class C>
ES_INLINE VectorDynamicTrack<C>::VectorDynamicTrack(StaticMask & mask, StaticMask::FlagOffset _offset, MasterTrack &master)
{
	int offset = _offset;

	for (int t = 0; t < numComponents; t++)
	{
		ILineTrack *ctr = nullptr;

		if (!mask.UseSubTrack(static_cast<StaticMask::FlagOffset>(offset--)))
		{
			auto _ctr = new LineStaticTrack();
			ctr = _ctr;
			master.staticFloats.push_back(&_ctr->track);
		}
		else
		{
			auto _ctr = new LineDynamicTrack();
			ctr = _ctr;
			master.dynamicTracks.push_back(_ctr);
		}

		tracks[t] = ctr;
	}
}

template<class C>
ES_INLINE VectorDynamicTrack<C>::~VectorDynamicTrack()
{
	for (int t = 0; t < numComponents; t++)
		delete tracks[t];
}