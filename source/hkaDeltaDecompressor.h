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
#include "hkaDecompressor.h"

struct hkaDeltaDecompressor
{
	typedef hvector<unsigned char> bitWidths_type;
	typedef hvector<StaticMask> masks_type;
	typedef hvector<float> offsets_type;
	typedef hvector<float> scales_type;
private:
	hvector<unsigned char> bitWidths;
	hvector<StaticMask> masks;
	hvector<float> offsets;
	hvector<float> scales;
	std::vector<MasterTrack*> tracks;
	std::vector<ILineTrack*> floats;
public:
	void Assign(hkaDeltaCompressedAnimationInternalInterface *input);
	bool IsTrackStatic(int trackID, hkaAnimation::TrackType type) const;
	void GetTrack(int trackID, int frame, hkaAnimation::TrackType type, Vector4 &out) const;
	void GetTransform(int trackID, int frame, hkQTransform &out) const;
	int GetNumInternalFrames() const;
	~hkaDeltaDecompressor();
};


ES_INLINE bool hkaDeltaDecompressor::IsTrackStatic(int trackID, hkaAnimation::TrackType type) const
{ 
	switch (type)
	{
	case hkaAnimation::Rotation:
		return tracks[trackID]->rotation->IsStatic();
	case hkaAnimation::Position:
		return tracks[trackID]->pos->IsStatic();
	case hkaAnimation::Scale:
		return tracks[trackID]->scale->IsStatic();
	}

	return true;
}

ES_INLINE void hkaDeltaDecompressor::GetTrack(int trackID, int frame, hkaAnimation::TrackType type, Vector4 &out) const
{
	switch (type)
	{
	case hkaAnimation::Rotation:
		out = tracks[trackID]->rotation->GetVector(frame);
		break;
	case hkaAnimation::Position:
		out = reinterpret_cast<Vector4 &>(tracks[trackID]->pos->GetVector(frame));
		out.W = 1.0f;
		break;
	case hkaAnimation::Scale:
		out = reinterpret_cast<Vector4 &>(tracks[trackID]->scale->GetVector(frame));
		out.W = 0.0f;
		break;
	}
}

ES_INLINE void hkaDeltaDecompressor::GetTransform(int trackID, int frame, hkQTransform &out) const
{
	out.rotation = tracks[trackID]->rotation->GetVector(frame);
	reinterpret_cast<Vector &>(out.position) = tracks[trackID]->pos->GetVector(frame);
	reinterpret_cast<Vector &>(out.scale) = tracks[trackID]->scale->GetVector(frame);
}

inline int hkaDeltaDecompressor::GetNumInternalFrames() const
{
	for (auto &t : tracks)
	{
		int numPosFrames = t->pos->NumFrames();
		int numRotFrames = t->rotation->NumFrames();
		int numSclFrames = t->scale->NumFrames();

		if (numPosFrames > 1)
			return numPosFrames;

		if (numRotFrames > 1)
			return numRotFrames;

		if (numSclFrames > 1)
			return numSclFrames;
	}

	return 1;
}
