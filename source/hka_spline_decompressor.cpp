/*  Havok Format Library
    Copyright(C) 2016-2023 Lukas Cone

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

#include "hka_spline_decompressor.hpp"
#include <cmath>
#include <cstring>

Vector4A16 Read32Quat(const char *&buffer) {
  constexpr uint64 rMask = (1 << 10) - 1;
  constexpr float rFrac = GetFraction(10);
  constexpr float fPI = 3.14159265f;
  constexpr float fPI2 = 0.5f * fPI;
  constexpr float fPI4 = 0.5f * fPI2;
  constexpr float phiFrac = fPI2 / 511.f;

  const uint32 cVal = *reinterpret_cast<const uint32 *>(buffer);

  float R = static_cast<float>((cVal >> 18) & rMask) * rFrac;
  R = 1.0f - (R * R);

  const float phiTheta = static_cast<float>((cVal & 0x3FFFF));

  float phi = floorf(sqrtf(phiTheta));
  float theta = 0;

  if (phi > 0.0f) {
    theta = fPI4 * (phiTheta - (phi * phi)) / phi;
    phi = phiFrac * phi;
  }

  const float magnitude = sqrtf(1.0f - R * R);
  const float sPhi = sinf(phi);
  const float cPhi = cosf(phi);
  const float sTheta = sinf(theta);
  const float cTheta = cosf(theta);

  const Vector4A16 retVal0 = Vector4A16(sPhi, sPhi, cPhi, R) *
                             Vector4A16(cTheta, sTheta, 1.f, 1.f) *
                             Vector4A16(magnitude, magnitude, magnitude, 1.f);
  const IVector4A16 signMask(0x10000000, 0x20000000, 0x40000000, 0x80000000);
  const auto blendMask =
      _mm_cmpeq_epi32((IVector4A16(cVal) & signMask)._data, signMask._data);

  const auto retVal =
      _mm_blendv_ps(retVal0._data, (-retVal0)._data,
                    reinterpret_cast<const __m128 &>(blendMask));

  buffer += 4;
  return retVal;
}

Vector4A16 Read40Quat(const char *&buffer) {
  constexpr float fractal = 0.000345436f;
  const Vector4A16 fract(fractal, fractal, fractal, 0);

  const uint64 cVal0 = *reinterpret_cast<const uint64 *>(buffer);
  const uint32 cVal1 = cVal0 >> 24;
  const auto tmpVal = (UIVector4A16(cVal0, cVal0, cVal1, 0) *
                       UIVector4A16(1 << 20, 1 << 8, 1 << 20, 0)) >>
                      20;
  const auto tmpVal1 = IVector4A16(tmpVal) - (1 << 11) - 1;
  const auto tmpVal2 = (Vector4A16(tmpVal1) * fract).QComputeElement();
  const size_t resultShift = (cVal0 >> 36) & 3;
  const Vector4A16 wmul(1.f, 1.f, 1.f, (cVal0 >> 38) & 1 ? -1.f : 1.f);
  const auto retVal = (wmul * tmpVal2)._data;
  buffer += 5;

  switch (resultShift) {
  case 0:
    return _mm_shuffle_ps(retVal, retVal, _MM_SHUFFLE(2, 1, 0, 3));
  case 1:
    return _mm_shuffle_ps(retVal, retVal, _MM_SHUFFLE(2, 1, 3, 0));
  case 2:
    return _mm_shuffle_ps(retVal, retVal, _MM_SHUFFLE(2, 3, 1, 0));
  default:
    return retVal;
  }
}

Vector4A16 Read48Quat(const char *&buffer) {
  constexpr uint64 mask = (1 << 15) - 1;
  constexpr float fractal = 0.000043161f;
  const Vector4A16 fract(fractal, fractal, fractal, 0);
  const SVector cVal = *reinterpret_cast<const SVector *>(buffer);

  const char resultShift = ((cVal.Y >> 14) & 2) | ((cVal.X >> 15) & 1);
  const bool rSign = (cVal.Z >> 15) != 0;
  const IVector4A16 retVal0(cVal.X, cVal.Y, cVal.Z, 0.f);
  auto retVal1 = Vector4A16((retVal0 & mask) - (mask >> 1)) * fract;
  retVal1.QComputeElement();
  const Vector4A16 wmul(1.f, 1.f, 1.f, rSign ? -1.f : 1.f);
  const auto retVal = (wmul * retVal1)._data;
  buffer += 6;

  switch (resultShift) {
  case 0:
    return _mm_shuffle_ps(retVal, retVal, _MM_SHUFFLE(2, 1, 0, 3));
  case 1:
    return _mm_shuffle_ps(retVal, retVal, _MM_SHUFFLE(2, 1, 3, 0));
  case 2:
    return _mm_shuffle_ps(retVal, retVal, _MM_SHUFFLE(2, 3, 1, 0));
  default:
    return retVal;
  }
}

Vector4A16 ReadQuat(QuantizationType qType, const char *&buffer) {
  switch (qType) {
  case QT_32bit:
    return Read32Quat(buffer);
  case QT_40bit:
    return Read40Quat(buffer);
  case QT_48bit:
    return Read48Quat(buffer);
  case QT_Uncompressed: {
    Vector4A16 retVal = *reinterpret_cast<const Vector4 *>(buffer);
    buffer += 16;
    return retVal;
  }
  default:
    return {0.0f, 0.0f, 0.0f, 1.0f};
  }
}

// Algorithm A2.1 The NURBS Book 2nd edition, page 68
int FindKnotSpan(int degree, float value, int cPointsSize,
                 std::span<uint8> &knots) {
  if (value >= knots[cPointsSize])
    return cPointsSize - 1;

  int low = degree;
  int high = cPointsSize;
  int mid = (low + high) / 2;

  while (value < knots[mid] || value >= knots[mid + 1]) {
    if (value < knots[mid])
      high = mid;
    else
      low = mid;

    mid = (low + high) / 2;
  }

  return mid;
}

// Basis_ITS1, GetPoint_NR1, TIME-EFFICIENT NURBS CURVE EVALUATION ALGORITHMS,
// pages 64 & 65
template <class C>
C GetSinglePoint(int knotSpanIndex, int degree, float frame,
                 std::span<uint8> &knots, std::vector<C> &cPoints) {
  float N[5] = {1.0f};

  for (int i = 1; i <= degree; i++)
    for (int j = i - 1; j >= 0; j--) {
      float A = (frame - knots[knotSpanIndex - j]) /
                (knots[knotSpanIndex + i - j] - knots[knotSpanIndex - j]);
      float tmp = N[j] * A;
      N[j + 1] += N[j] - tmp;
      N[j] = tmp;
    }

  C retVal;
  memset(static_cast<void *>(&retVal), 0, sizeof(C));

  for (int i = 0; i <= degree; i++)
    retVal += cPoints[knotSpanIndex - i] * N[i];

  return retVal;
}

Vector4A16 SplineDynamicTrackQuat::GetValue(float localFrame) {
  int knotSpan =
      FindKnotSpan(degree, localFrame, static_cast<int>(track.size()), knots);
  return GetSinglePoint(knotSpan, degree, localFrame, knots, track);
}

Vector SplineDynamicTrackVector::GetValue(float localFrame) {
  Vector out;
  int knotSpan = -1;

  int cSize = static_cast<int>(tracks[0].size());

  if (cSize == 1)
    out.X = tracks[0][0];
  else {
    knotSpan = FindKnotSpan(degree, localFrame, cSize, knots);
    out.X = GetSinglePoint(knotSpan, degree, localFrame, knots, tracks[0]);
  }

  cSize = static_cast<int>(tracks[1].size());

  if (cSize == 1)
    out.Y = tracks[1][0];
  else {
    if (knotSpan < 0)
      knotSpan = FindKnotSpan(degree, localFrame, cSize, knots);

    out.Y = GetSinglePoint(knotSpan, degree, localFrame, knots, tracks[1]);
  }

  cSize = static_cast<int>(tracks[2].size());

  if (cSize == 1)
    out.Z = tracks[2][0];
  else {
    if (knotSpan < 0)
      knotSpan = FindKnotSpan(degree, localFrame, cSize, knots);

    out.Z = GetSinglePoint(knotSpan, degree, localFrame, knots, tracks[2]);
  }

  return out;
}

void ApplyPadding(char *&buffer, int alignment = 4) {
  const size_t iterPos = reinterpret_cast<intptr_t>(buffer);
  const size_t result = iterPos & (alignment - 1);

  if (!result)
    return;

  buffer += alignment - result;
}

struct TrackBBOX {
  float min, max;
};

void TransformSplineBlock::Assign(char *buffer, size_t numTracks,
                                  size_t numFloatTractks) {
  auto trackStart = reinterpret_cast<TransformMask *>(buffer);
  buffer += sizeof(TransformMask) * numTracks + numFloatTractks;
  ApplyPadding(buffer);
  int cTrack = 0;
  masks = {trackStart, numTracks};
  tracks.resize(numTracks);

  for (auto &m : masks) {
    auto MakeTrack = [&](QuantizationType qtype, float defVal,
                         auto subTypes) -> TransformTrack::TrackType<Vector> {
      using stype = decltype(subTypes);
      const bool useSpline =
          m.GetSubTrackType(static_cast<TransformType>(stype::X)) ==
              STT_DYNAMIC ||
          m.GetSubTrackType(static_cast<TransformType>(stype::Y)) ==
              STT_DYNAMIC ||
          m.GetSubTrackType(static_cast<TransformType>(stype::Z)) ==
              STT_DYNAMIC;

      if (useSpline) {
        auto sTrack = std::make_unique<SplineDynamicTrackVector>();
        uint16 numItems = *reinterpret_cast<const uint16 *>(buffer++);
        buffer++;
        sTrack->degree = *reinterpret_cast<const uint8 *>(buffer++);
        const size_t bufferSkip = numItems + sTrack->degree + 2;
        sTrack->knots = {reinterpret_cast<uint8 *>(buffer), bufferSkip};
        buffer += bufferSkip;
        ApplyPadding(buffer);

        TrackBBOX extremes[3] = {};

        auto MakeSubTrack = [&](auto type, size_t id) {
          const auto ttype =
              m.GetSubTrackType(static_cast<TransformType>(type));

          if (ttype == STT_DYNAMIC) {
            extremes[id] = *reinterpret_cast<const TrackBBOX *>(buffer);
            buffer += 8;
            sTrack->tracks[id].resize(numItems + 1);
          } else if (ttype == STT_STATIC) {
            sTrack->tracks[id].push_back(
                *reinterpret_cast<const float *>(buffer));
            buffer += 4;
          } else {
            sTrack->tracks[id].push_back(defVal);
          }
        };

        MakeSubTrack(stype::X, 0);
        MakeSubTrack(stype::Y, 1);
        MakeSubTrack(stype::Z, 2);

        auto UnpackPoints8 = [&](auto type, size_t id, size_t sid) {
          constexpr float fractal = 1.0f / 255.0f;
          const auto ttype =
              m.GetSubTrackType(static_cast<TransformType>(type));
          if (ttype == STT_DYNAMIC) {
            float dVar =
                static_cast<float>(*reinterpret_cast<const uint8 *>(buffer++)) *
                fractal;
            sTrack->tracks[id][sid] =
                extremes[id].min + (extremes[id].max - extremes[id].min) * dVar;
          }
        };

        auto UnpackPoints16 = [&](auto type, size_t id, size_t sid) {
          constexpr float fractal = 1.0f / 0xffff;
          const auto ttype =
              m.GetSubTrackType(static_cast<TransformType>(type));
          if (ttype == STT_DYNAMIC) {
            float dVar = static_cast<float>(
                             *reinterpret_cast<const uint16 *>(buffer++)) *
                         fractal;
            sTrack->tracks[id][sid] =
                extremes[id].min + (extremes[id].max - extremes[id].min) * dVar;
            buffer++;
          }
        };

        if (qtype == QT_8bit) {
          for (int t = 0; t <= numItems; t++) {
            UnpackPoints8(stype::X, 0, t);
            UnpackPoints8(stype::Y, 1, t);
            UnpackPoints8(stype::Z, 2, t);
          }
        } else {
          for (int t = 0; t <= numItems; t++) {
            UnpackPoints16(stype::X, 0, t);
            UnpackPoints16(stype::Y, 1, t);
            UnpackPoints16(stype::Z, 2, t);
          }
        }

        ApplyPadding(buffer);
        return sTrack;
      }

      auto sTrack = std::make_unique<SplineStaticTrack<Vector>>();
      auto MakeSubTrack = [&](auto type, size_t id) {
        const auto ttype = m.GetSubTrackType(static_cast<TransformType>(type));

        if (ttype == STT_STATIC) {
          sTrack->item[id] = *reinterpret_cast<const float *>(buffer);
          buffer += 4;
        } else {
          sTrack->item[id] = defVal;
        }
      };

      MakeSubTrack(stype::X, 0);
      MakeSubTrack(stype::Y, 1);
      MakeSubTrack(stype::Z, 2);

      return sTrack;
    };

    tracks[cTrack].pos =
        MakeTrack(m.GetPosQuantizationType(), 0.f, TransformTypePos{});

    if (m.GetSubTrackType(ttRotation) == STT_DYNAMIC) {
      auto rTrack = new SplineDynamicTrackQuat();
      tracks[cTrack].rotation = TransformTrack::TrackType<Vector4A16>(rTrack);
      uint16 numItems = *reinterpret_cast<const uint16 *>(buffer++);
      buffer++;
      rTrack->degree = *reinterpret_cast<const uint8 *>(buffer++);
      const size_t bufferSkip = numItems + rTrack->degree + 2;
      rTrack->knots = {reinterpret_cast<uint8 *>(buffer), bufferSkip};
      buffer += bufferSkip;

      QuantizationType quantType = m.GetRotQuantizationType();

      if (quantType == QT_48bit || quantType == QT_16bitQuat)
        ApplyPadding(buffer, 2);
      else if (quantType == QT_32bit || quantType == QT_Uncompressed)
        ApplyPadding(buffer);

      rTrack->track.resize(numItems + 1);

      for (int t = 0; t <= numItems; t++) {
        rTrack->track[t] = ReadQuat(quantType, (const char *&)buffer);
      }
    } else {
      auto rTrack = std::make_unique<SplineStaticTrack<Vector4A16>>();

      if (m.GetSubTrackType(ttRotation) == STT_STATIC) {
        rTrack->item =
            ReadQuat(m.GetRotQuantizationType(), (const char *&)buffer);
      } else {
        rTrack->item.W = 1.0f;
      }

      tracks[cTrack].rotation = std::move(rTrack);
    }

    ApplyPadding(buffer);

    tracks[cTrack].scale =
        MakeTrack(m.GetScaleQuantizationType(), 1.f, TransformTypeScale{});

    cTrack++;
  }
}

void hkaSplineDecompressor::Assign(
    hkaSplineCompressedAnimationInternalInterface *input) {
  auto blockOffsets = input->GetBlockOffsets();
  char *data = input->GetData();
  blocks.resize(blockOffsets.size());
  int cBlock = 0;

  for (auto &b : blocks) {
    b.Assign(data + *(blockOffsets.data() + cBlock),
             input->GetNumOfTransformTracks(), input->GetNumOfFloatTracks());
    cBlock++;
  }
}
