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

#include "hkaSplineDecompressor.h"

Vector4 Read32Quat(char *&buffer)
{
	const uint64 rMask = (1 << 10) - 1;
	const float rFrac = 1.0f / rMask;
	constexpr float fPI = 3.14159265f;
	constexpr float fPI2 = 0.5f * fPI;
	constexpr float fPI4 = 0.5f * fPI2;
	constexpr float phiFrac = fPI2 / 511.f;

	uint cVal = *reinterpret_cast<uint *>(buffer);

	float R = static_cast<float>((cVal >> 18) & rMask) * rFrac;
	R = 1.0f - (R * R);

	float phiTheta = static_cast<float>((cVal & 0x3FFFF));

	float phi = floorf(sqrtf(phiTheta));
	float theta = 0;

	if (phi > 0.0f)
	{
		theta = fPI4 * (phiTheta - (phi * phi)) / phi;
		phi = phiFrac * phi;
	}

	float magnitude = std::sqrtf(1.0f - R * R);

	Vector4 retVal;
	retVal.X = sinf(phi) * cosf(theta) * magnitude;
	retVal.Y = sinf(phi) * sinf(theta) * magnitude;
	retVal.Z = cosf(phi) * magnitude;
	retVal.W = R;

	if (cVal & 0x10000000)
		retVal.X *= -1;

	if (cVal & 0x20000000)
		retVal.Y *= -1;

	if (cVal & 0x40000000)
		retVal.Z *= -1;

	if (cVal & 0x80000000)
		retVal.W *= -1;

	buffer += 4;
	return retVal;
}

Vector4 Read40Quat(char *&buffer)
{
	const uint64 mask = (1 << 12) - 1;
	const uint64 positiveMask = mask >> 1;
	const float fractal = 0.000345436f;
	uint64 cVal = *reinterpret_cast<uint64 *>(buffer);

	IVector tempVal;
	tempVal.X = cVal & mask;
	tempVal.Y = (cVal >> 12) & mask;
	tempVal.Z = (cVal >> 24) & mask;

	int resultShift = (cVal >> 36) & 3;

	tempVal -= positiveMask;

	Vector tempValF = tempVal.Convert<float>() * fractal;

	Vector4 retval;

	for (int i = 0; i < 4; i++)
	{
		if (i < resultShift)
			retval[i] = tempValF[i];
		else if (i > resultShift)
			retval[i] = tempValF[i - 1];
	}

	retval[resultShift] = 1.0f - tempValF.X * tempValF.X - tempValF.Y * tempValF.Y - tempValF.Z * tempValF.Z;

	if (retval[resultShift] <= 0.0f)
		retval[resultShift] = 0.0f;
	else
		retval[resultShift] = std::sqrtf(retval[resultShift]);

	if ((cVal >> 38) & 1)
		retval[resultShift] *= -1;

	buffer += 5;
	return retval;
}

Vector4 Read48Quat(char *&buffer)
{
	const uint64 mask = (1 << 15) - 1;
	const float fractal = 0.000043161f;
	SVector cVal = *reinterpret_cast<SVector *>(buffer);

	char resultShift = ((cVal.Y >> 14) & 2) | ((cVal.X >> 15) & 1);
	bool rSign = (cVal.Z >> 15) != 0;

	cVal &= mask;
	cVal -= mask >> 1;

	Vector tempValF = cVal.Convert<float>() * fractal;

	Vector4 retval;

	for (int i = 0; i < 4; i++)
	{
		if (i < resultShift)
			retval[i] = tempValF[i];
		else if (i > resultShift)
			retval[i] = tempValF[i - 1];
	}

	retval[resultShift] = 1.0f - tempValF.X * tempValF.X - tempValF.Y * tempValF.Y - tempValF.Z * tempValF.Z;

	if (retval[resultShift] <= 0.0f)
		retval[resultShift] = 0.0f;
	else
		retval[resultShift] = std::sqrtf(retval[resultShift]);

	if (rSign)
		retval[resultShift] *= -1;

	buffer += 6;
	return retval;
}

Vector4 ReadQuat(QuantizationType qType, char *&buffer)
{
	switch (qType)
	{
	case QT_32bit:
		return Read32Quat(buffer);
	case QT_40bit:
		return Read40Quat(buffer);
	case QT_48bit:
		return Read48Quat(buffer);
	}

	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

// Algorithm A2.1 The NURBS Book 2nd edition, page 68
int FindKnotSpan(int degree, float value, int cPointsSize, SplineDynamicTrackVector::knots_type &knots)
{
	if (value >= knots[cPointsSize])
		return cPointsSize - 1;

	int low = degree;
	int high = cPointsSize;
	int mid = (low + high) / 2;

	while (value < knots[mid] || value >= knots[mid + 1])
	{
		if (value < knots[mid])
			high = mid;
		else
			low = mid;

		mid = (low + high) / 2;
	}

	return mid;
}

//Basis_ITS1, GetPoint_NR1, TIME-EFFICIENT NURBS CURVE EVALUATION ALGORITHMS, pages 64 & 65
template<class C>
C GetSinglePoint(int knotSpanIndex, int degree, float frame, SplineDynamicTrackVector::knots_type &knots, std::vector<C> &cPoints)
{
	float N[5] = { 1.0f };

	for (int i = 1; i <= degree; i++)
		for (int j = i - 1; j >= 0; j--)
		{
			float A = (frame - knots[knotSpanIndex - j]) / (knots[knotSpanIndex + i - j] - knots[knotSpanIndex - j]);
			float tmp = N[j] * A;
			N[j + 1] += N[j] - tmp;
			N[j] = tmp;
		}

	C retVal;
	memset(&retVal, 0, sizeof(C));

	for (int i = 0; i <= degree; i++)
		retVal += cPoints[knotSpanIndex - i] * N[i];

	return retVal;
}

Vector4 SplineDynamicTrackQuat::GetValue(float localFrame)
{
	int knotSpan = FindKnotSpan(degree, localFrame, static_cast<int>(track.size()), knots);
	return GetSinglePoint(knotSpan, degree, localFrame, knots, track);
}

Vector SplineDynamicTrackVector::GetValue(float localFrame)
{
	Vector out;
	int knotSpan = -1;

	int cSize = static_cast<int>(tracks[0].size());

	if (cSize == 1)
		out.X = tracks[0][0];
	else
	{
		knotSpan = FindKnotSpan(degree, localFrame, cSize, knots);
		out.X = GetSinglePoint(knotSpan, degree, localFrame, knots, tracks[0]);
	}

	cSize = static_cast<int>(tracks[1].size());

	if (cSize == 1)
		out.Y = tracks[1][0];
	else
	{
		if (knotSpan < 0)
			knotSpan = FindKnotSpan(degree, localFrame, cSize, knots);

		out.Y = GetSinglePoint(knotSpan, degree, localFrame, knots, tracks[1]);
	}

	cSize = static_cast<int>(tracks[2].size());

	if (cSize == 1)
		out.Z = tracks[2][0];
	else
	{
		if (knotSpan < 0)
			knotSpan = FindKnotSpan(degree, localFrame, cSize, knots);

		out.Z = GetSinglePoint(knotSpan, degree, localFrame, knots, tracks[2]);
	}

	return out;
}


void ApplyPadding(char *&buffer, int alignment = 4)
{
	const size_t iterPos = reinterpret_cast<esIntPtr>(buffer);
	const size_t result = iterPos & (alignment - 1);

	if (!result)
		return;

	buffer += alignment - result;
}

struct TrackBBOX
{
	float min,
		max;
};

void TransformSplineBlock::Assign(char *buffer, int numTracks, int numFloatTractks)
{
	TransformMask *trackStart = reinterpret_cast<TransformMask *>(buffer);
	buffer += sizeof(TransformMask) * numTracks + numFloatTractks;
	ApplyPadding(buffer);
	int cTrack = 0;
	masks = masks_type(trackStart, trackStart + numTracks, masks_type::allocator_type(trackStart));
	tracks.resize(numTracks);

	for (auto &m : masks)
	{
		const bool usePosSpline = m.GetSubTrackType(ttPosX) == STT_DYNAMIC || m.GetSubTrackType(ttPosY) == STT_DYNAMIC || m.GetSubTrackType(ttPosZ) == STT_DYNAMIC;

		if (usePosSpline)
		{
			SplineDynamicTrackVector *pTrack = new SplineDynamicTrackVector();
			tracks[cTrack].pos = pTrack;
			short numItems = *reinterpret_cast<short *>(buffer++);
			buffer++;
			pTrack->degree = *reinterpret_cast<uchar *>(buffer++);
			const int bufferSkip = numItems + pTrack->degree + 2;
			pTrack->knots = SplineDynamicTrackVector::knots_type(buffer, buffer + bufferSkip, SplineDynamicTrackVector::knots_type::allocator_type(reinterpret_cast<uchar *>(buffer)));
			buffer += bufferSkip;
			ApplyPadding(buffer);

			TrackBBOX extremes[3] = {};

			if (m.GetSubTrackType(ttPosX) == STT_DYNAMIC)
			{
				extremes[0] = *reinterpret_cast<TrackBBOX *>(buffer);
				buffer += 8;
				pTrack->tracks[0].resize(numItems + 1);
			}
			else if (m.GetSubTrackType(ttPosX) == STT_STATIC)
			{
				pTrack->tracks[0].push_back(*reinterpret_cast<float *>(buffer));
				buffer += 4;
			}
			else
				pTrack->tracks[0].resize(1);

			if (m.GetSubTrackType(ttPosY) == STT_DYNAMIC)
			{
				extremes[1] = *reinterpret_cast<TrackBBOX *>(buffer);
				buffer += 8;
				pTrack->tracks[1].resize(numItems + 1);
			}
			else if (m.GetSubTrackType(ttPosY) == STT_STATIC)
			{
				pTrack->tracks[1].push_back(*reinterpret_cast<float *>(buffer));
				buffer += 4;
			}
			else
				pTrack->tracks[1].resize(1);

			if (m.GetSubTrackType(ttPosZ) == STT_DYNAMIC)
			{
				extremes[2] = *reinterpret_cast<TrackBBOX *>(buffer);
				buffer += 8;
				pTrack->tracks[2].resize(numItems + 1);
			}
			else if (m.GetSubTrackType(ttPosZ) == STT_STATIC)
			{
				pTrack->tracks[2].push_back(*reinterpret_cast<float *>(buffer));
				buffer += 4;
			}
			else
				pTrack->tracks[2].resize(1);

			if (m.GetPosQuantizationType() == QT_8bit)
			{
				static const float fractal = 1.0f / 255.0f;

				for (int t = 0; t <= numItems; t++)
				{
					if (m.GetSubTrackType(ttPosX) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<uchar *>(buffer++)) * fractal;
						pTrack->tracks[0][t] = extremes[0].min + (extremes[0].max - extremes[0].min) * dVar;
					}

					if (m.GetSubTrackType(ttPosY) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<uchar *>(buffer++)) * fractal;
						pTrack->tracks[1][t] = extremes[1].min + (extremes[1].max - extremes[1].min) * dVar;
					}

					if (m.GetSubTrackType(ttPosZ) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<uchar *>(buffer++)) * fractal;
						pTrack->tracks[2][t] = extremes[2].min + (extremes[2].max - extremes[2].min) * dVar;
					}
				}
			}
			else
			{
				static const float fractal = 1.0f / 0xffff;

				for (int t = 0; t <= numItems; t++)
				{
					if (m.GetSubTrackType(ttPosX) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<ushort *>(buffer++)) * fractal;
						pTrack->tracks[0][t] = extremes[0].min + (extremes[0].max - extremes[0].min) * dVar;
						buffer++;
					}

					if (m.GetSubTrackType(ttPosY) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<ushort *>(buffer++)) * fractal;
						pTrack->tracks[1][t] = extremes[1].min + (extremes[1].max - extremes[1].min) * dVar;
						buffer++;
					}

					if (m.GetSubTrackType(ttPosZ) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<ushort *>(buffer++)) * fractal;
						pTrack->tracks[2][t] = extremes[2].min + (extremes[2].max - extremes[2].min) * dVar;
						buffer++;
					}
				}
			}

			ApplyPadding(buffer);
		}
		else
		{
			SplineStaticTrack<Vector> *pTrack = new SplineStaticTrack<Vector>();
			tracks[cTrack].pos = pTrack;

			if (m.GetSubTrackType(ttPosX) == STT_STATIC)
			{
				pTrack->item.X = *reinterpret_cast<float *>(buffer);
				buffer += 4;
			}

			if (m.GetSubTrackType(ttPosY) == STT_STATIC)
			{
				pTrack->item.Y = *reinterpret_cast<float *>(buffer);
				buffer += 4;
			}

			if (m.GetSubTrackType(ttPosZ) == STT_STATIC)
			{
				pTrack->item.Z = *reinterpret_cast<float *>(buffer);
				buffer += 4;
			}
		}

		if (m.GetSubTrackType(ttRotation) == STT_DYNAMIC)
		{
			SplineDynamicTrackQuat *rTrack = new SplineDynamicTrackQuat();
			tracks[cTrack].rotation = rTrack;
			esIntPtr savePtr = reinterpret_cast<esIntPtr>(buffer);
			short numItems = *reinterpret_cast<short *>(buffer++);
			buffer++;
			rTrack->degree = *reinterpret_cast<uchar *>(buffer++);
			const int bufferSkip = numItems + rTrack->degree + 2;
			rTrack->knots = SplineDynamicTrackQuat::knots_type(buffer, buffer + bufferSkip, SplineDynamicTrackQuat::knots_type::allocator_type(reinterpret_cast<uchar *>(buffer)));
			buffer += bufferSkip;

			if (m.GetRotQuantizationType() == QT_48bit)
				ApplyPadding(buffer, 2);
			else if (m.GetRotQuantizationType() == QT_32bit)
				ApplyPadding(buffer);

			rTrack->track.resize(numItems + 1);

			for (int t = 0; t <= numItems; t++)
				rTrack->track[t] = ReadQuat(m.GetRotQuantizationType(), buffer);
		}
		else
		{
			SplineStaticTrack<Vector4> *rTrack = new SplineStaticTrack<Vector4>();
			tracks[cTrack].rotation = rTrack;

			if (m.GetSubTrackType(ttRotation) == STT_STATIC)
				rTrack->item = ReadQuat(m.GetRotQuantizationType(), buffer);
			else
				rTrack->item.W = 1.0f;
		}

		ApplyPadding(buffer);

		const bool useScaleSpline = m.GetSubTrackType(ttScaleX) == STT_DYNAMIC || m.GetSubTrackType(ttScaleY) == STT_DYNAMIC || m.GetSubTrackType(ttScaleZ) == STT_DYNAMIC;

		if (useScaleSpline)
		{
			SplineDynamicTrackVector *sTrack = new SplineDynamicTrackVector();
			tracks[cTrack].scale = sTrack;
			short numItems = *reinterpret_cast<short *>(buffer++);
			buffer++;
			sTrack->degree = *reinterpret_cast<uchar *>(buffer++);
			const int bufferSkip = numItems + sTrack->degree + 2;
			sTrack->knots = SplineDynamicTrackVector::knots_type(buffer, buffer + bufferSkip, SplineDynamicTrackVector::knots_type::allocator_type(reinterpret_cast<uchar *>(buffer)));
			buffer += bufferSkip;
			ApplyPadding(buffer);

			TrackBBOX extremes[3] = {};

			if (m.GetSubTrackType(ttScaleX) == STT_DYNAMIC)
			{
				extremes[0] = *reinterpret_cast<TrackBBOX *>(buffer);
				buffer += 8;
				sTrack->tracks[0].resize(numItems + 1);
			}
			else if (m.GetSubTrackType(ttScaleX) == STT_STATIC)
			{
				sTrack->tracks[0].push_back(*reinterpret_cast<float *>(buffer));
				buffer += 4;
			}
			else
				sTrack->tracks[0].push_back(1.0f);

			if (m.GetSubTrackType(ttScaleY) == STT_DYNAMIC)
			{
				extremes[1] = *reinterpret_cast<TrackBBOX *>(buffer);
				buffer += 8;
				sTrack->tracks[1].resize(numItems + 1);
			}
			else if (m.GetSubTrackType(ttScaleY) == STT_STATIC)
			{
				sTrack->tracks[1].push_back(*reinterpret_cast<float *>(buffer));
				buffer += 4;
			}
			else
				sTrack->tracks[1].push_back(1.0f);

			if (m.GetSubTrackType(ttScaleZ) == STT_DYNAMIC)
			{
				extremes[2] = *reinterpret_cast<TrackBBOX *>(buffer);
				buffer += 8;
				sTrack->tracks[2].resize(numItems + 1);
			}
			else if (m.GetSubTrackType(ttScaleZ) == STT_STATIC)
			{
				sTrack->tracks[2].push_back(*reinterpret_cast<float *>(buffer));
				buffer += 4;
			}
			else
				sTrack->tracks[2].push_back(1.0f);

			if (m.GetScaleQuantizationType() == QT_8bit)
			{
				static const float fractal = 1.0f / 255.0f;

				for (int t = 0; t <= numItems; t++)
				{
					if (m.GetSubTrackType(ttScaleX) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<uchar *>(buffer++)) * fractal;
						sTrack->tracks[0][t] = extremes[0].min + (extremes[0].max - extremes[0].min) * dVar;
					}

					if (m.GetSubTrackType(ttScaleY) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<uchar *>(buffer++)) * fractal;
						sTrack->tracks[1][t] = extremes[1].min + (extremes[1].max - extremes[1].min) * dVar;
					}

					if (m.GetSubTrackType(ttScaleZ) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<uchar *>(buffer++)) * fractal;
						sTrack->tracks[2][t] = extremes[2].min + (extremes[2].max - extremes[2].min) * dVar;
					}
				}
			}
			else
			{
				static const float fractal = 1.0f / 0xffff;

				for (int t = 0; t <= numItems; t++)
				{
					if (m.GetSubTrackType(ttScaleX) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<ushort *>(buffer++)) * fractal;
						sTrack->tracks[0][t] = extremes[0].min + (extremes[0].max - extremes[0].min) * dVar;
						buffer++;
					}

					if (m.GetSubTrackType(ttScaleY) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<ushort *>(buffer++)) * fractal;
						sTrack->tracks[1][t] = extremes[1].min + (extremes[1].max - extremes[1].min) * dVar;
						buffer++;
					}

					if (m.GetSubTrackType(ttScaleZ) == STT_DYNAMIC)
					{
						float dVar = static_cast<float>(*reinterpret_cast<ushort *>(buffer++)) * fractal;
						sTrack->tracks[2][t] = extremes[2].min + (extremes[2].max - extremes[2].min) * dVar;
						buffer++;
					}
				}
			}

			ApplyPadding(buffer);
		}
		else
		{
			SplineStaticTrack<Vector> *sTrack = new SplineStaticTrack<Vector>();
			tracks[cTrack].scale = sTrack;

			if (m.GetSubTrackType(ttScaleX) == STT_STATIC)
			{
				sTrack->item.X = *reinterpret_cast<float *>(buffer);
				buffer += 4;
			}
			else
				sTrack->item.X = 1.0f;

			if (m.GetSubTrackType(ttScaleY) == STT_STATIC)
			{
				sTrack->item.Y = *reinterpret_cast<float *>(buffer);
				buffer += 4;
			}
			else
				sTrack->item.Y = 1.0f;

			if (m.GetSubTrackType(ttScaleZ) == STT_STATIC)
			{
				sTrack->item.Z = *reinterpret_cast<float *>(buffer);
				buffer += 4;
			}
			else
				sTrack->item.Z = 1.0f;
		}

		cTrack++;
	}
}

void hkaSplineDecompressor::Assign(hkaSplineCompressedAnimationInternalInterface *input)
{
	hkRealArray<uint> blockOffsets = input->GetBlockOffsets();
	char *data = input->GetData();
	blocks.resize(blockOffsets.count);
	int cBlock = 0;

	for (auto &b : blocks)
	{
		b.Assign(data + *(blockOffsets.data + cBlock), input->GetNumOfTransformTracks(), input->GetNumOfFloatTracks());
		cBlock++;
	}

}

TransformTrack::~TransformTrack()
{
	if (pos)
		delete pos;

	if (scale)
		delete scale;

	if (rotation)
		delete rotation;
}
