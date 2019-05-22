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
#include "hkInternalInterfaces.h"
#include "datas/flags.hpp"
#include <vector>
#include "datas/allocator_hybrid.hpp"

enum SplineTrackType
{
	STT_DYNAMIC,
	STT_STATIC,
	STT_IDENTITY
};

enum QuantizationType
{
	QT_8bit,
	QT_16bit,
	QT_32bit,
	QT_40bit,
	QT_48bit,
};

enum TransformType
{
	ttPosX,
	ttPosY,
	ttPosZ,
	ttRotation,
	ttScaleX,
	ttScaleY,
	ttScaleZ
};

struct TransformMask
{
	enum FlagOffset
	{
		staticX,
		staticY,
		staticZ,
		staticW,
		splineX,
		splineY,
		splineZ,
		splineW
	};

	uchar quantizationTypes;
	esFlags<uchar, FlagOffset> positionTypes;
	uchar rotationTypes;
	esFlags<uchar, FlagOffset> scaleTypes;

	ES_INLINE QuantizationType GetPosQuantizationType() const { return static_cast<QuantizationType>(quantizationTypes & 3); }
	ES_INLINE QuantizationType GetRotQuantizationType() { return static_cast<QuantizationType>(((quantizationTypes >> 2) & 0xf) + 2); }
	ES_INLINE QuantizationType GetScaleQuantizationType() { return static_cast<QuantizationType>((quantizationTypes >> 6) & 3); }

	ES_INLINE SplineTrackType GetSubTrackType(TransformType type)
	{
		switch (type)
		{
		case ttPosX:
			return positionTypes[staticX] ? STT_STATIC : (positionTypes[splineX] ? STT_DYNAMIC : STT_IDENTITY);
		case ttPosY:
			return positionTypes[staticY] ? STT_STATIC : (positionTypes[splineY] ? STT_DYNAMIC : STT_IDENTITY);
		case ttPosZ:
			return positionTypes[staticZ] ? STT_STATIC : (positionTypes[splineZ] ? STT_DYNAMIC : STT_IDENTITY);
		case ttRotation:
			return rotationTypes & 0xf0 ? STT_DYNAMIC : (rotationTypes & 0xf ? STT_STATIC : STT_IDENTITY);
		case ttScaleX:
			return scaleTypes[staticX] ? STT_STATIC : (scaleTypes[splineX] ? STT_DYNAMIC : STT_IDENTITY);
		case ttScaleY:
			return scaleTypes[staticY] ? STT_STATIC : (scaleTypes[splineY] ? STT_DYNAMIC : STT_IDENTITY);
		case ttScaleZ:
			return scaleTypes[staticZ] ? STT_STATIC : (scaleTypes[splineZ] ? STT_DYNAMIC : STT_IDENTITY);
		}
	}
};

template<class C> struct ISplineTrack
{
	virtual bool IsStatic() = 0;
	virtual C GetValue(float localFrame) = 0;
	virtual ~ISplineTrack() {}
};

template<class C>
struct SplineStaticTrack : ISplineTrack<C>
{
	C item;
	C GetValue( float) { return item; }
	bool IsStatic() { return true; }
};

struct SplineDynamicTrackVector : ISplineTrack<Vector>
{
	typedef std::vector<uchar, std::allocator_hybrid<uchar>> knots_type;
	std::vector<float> tracks[3];
	knots_type knots;
	uchar degree;

	Vector GetValue(float localFrame);
	bool IsStatic() { return !knots.size(); }
};

struct SplineDynamicTrackQuat : ISplineTrack<Vector4>
{
	typedef std::vector<uchar, std::allocator_hybrid<uchar>> knots_type;
	std::vector<Vector4> track;
	knots_type knots;
	uchar degree;

	Vector4 GetValue(float localFrame);
	bool IsStatic() { return false; }
};
struct TransformTrack
{
	ISplineTrack<Vector> *pos;
	ISplineTrack<Vector4> *rotation;
	ISplineTrack<Vector> *scale;
};

struct TransformSplineBlock
{
	typedef std::vector<TransformMask, std::allocator_hybrid<TransformMask>> masks_type;
private:
	masks_type masks;
	std::vector<TransformTrack> tracks;
public:
	void Assign(char *buffer, int numTracks);
	bool IsTrackStatic(int trackID, hkaAnimation::TrackType type) const;
	void GetTrack(int trackID, float localFrame, hkaAnimation::TrackType type, Vector4 &out) const;
	void GetTransform(int trackID, float localFrame, hkQTransform &out) const;
};

struct hkaSplineDecompressor
{
	std::vector<TransformSplineBlock> blocks;
	//TODO floats
	void Assign(hkaSplineCompressedAnimationInternalInterface *input);
	bool IsTrackStatic(int trackID, hkaAnimation::TrackType type) const;
};

ES_INLINE bool TransformSplineBlock::IsTrackStatic(int trackID, hkaAnimation::TrackType type) const
{
	switch (type)
	{
	case hkaAnimation::Rotation:
		return tracks[trackID].rotation->IsStatic();
	case hkaAnimation::Position:
		return tracks[trackID].pos->IsStatic();
	case hkaAnimation::Scale:
		return tracks[trackID].scale->IsStatic();
	}

	return true;
}

ES_INLINE bool hkaSplineDecompressor::IsTrackStatic(int trackID, hkaAnimation::TrackType type) const
{
	for (auto &b : blocks)
	{
		if (!b.IsTrackStatic(trackID, type))
			return false;
	}

	return true;
}

ES_INLINE void TransformSplineBlock::GetTrack(int trackID, float localFrame, hkaAnimation::TrackType type, Vector4 &out) const
{
	switch (type)
	{
	case hkaAnimation::Rotation:
		out = tracks[trackID].rotation->GetValue(localFrame);
		break;
	case hkaAnimation::Position:
		out = reinterpret_cast<Vector4 &>(tracks[trackID].pos->GetValue(localFrame));
		out.W = 1.0f;
		break;
	case hkaAnimation::Scale:
		out = reinterpret_cast<Vector4 &>(tracks[trackID].scale->GetValue(localFrame));
		out.W = 0.0f;
		break;
	}
}

ES_INLINE void TransformSplineBlock::GetTransform(int trackID, float localFrame, hkQTransform &out) const
{
	out.rotation = tracks[trackID].rotation->GetValue(localFrame);
	reinterpret_cast<Vector &>(out.position) = tracks[trackID].pos->GetValue(localFrame);
	reinterpret_cast<Vector &>(out.scale) = tracks[trackID].scale->GetValue(localFrame);
}

