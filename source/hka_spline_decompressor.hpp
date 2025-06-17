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

#pragma once
#include "internal/hka_splineanimation.hpp"
#include "spike/type/flags.hpp"
#include <vector>

enum SplineTrackType { STT_DYNAMIC, STT_STATIC, STT_IDENTITY };

enum QuantizationType {
  QT_8bit,
  QT_16bit,
  QT_32bit,
  QT_40bit,
  QT_48bit,
  QT_24bit,
  QT_16bitQuat,
  QT_Uncompressed,
};

enum TransformType {
  ttPosX,
  ttPosY,
  ttPosZ,
  ttRotation,
  ttScaleX,
  ttScaleY,
  ttScaleZ
};

struct TransformTypePos {
  enum dummy {
    X = ttPosX,
    Y,
    Z,
  };
};

struct TransformTypeScale {
  enum dummy {
    X = ttScaleX,
    Y,
    Z,
  };
};

struct TransformMask {
  enum FlagOffset : uint8 {
    staticX,
    staticY,
    staticZ,
    staticW,
    splineX,
    splineY,
    splineZ,
    splineW
  };

  uint8 quantizationTypes;
  es::Flags<FlagOffset> positionTypes;
  uint8 rotationTypes;
  es::Flags<FlagOffset> scaleTypes;

  QuantizationType GetPosQuantizationType() const {
    return static_cast<QuantizationType>(quantizationTypes & 3);
  }
  QuantizationType GetRotQuantizationType() const {
    return static_cast<QuantizationType>(((quantizationTypes >> 2) & 0xf) + 2);
  }
  QuantizationType GetScaleQuantizationType() const {
    return static_cast<QuantizationType>((quantizationTypes >> 6) & 3);
  }

  SplineTrackType GetSubTrackType(TransformType type) const {
    switch (type) {
    case ttPosX:
      return positionTypes[staticX]
                 ? STT_STATIC
                 : (positionTypes[splineX] ? STT_DYNAMIC : STT_IDENTITY);
    case ttPosY:
      return positionTypes[staticY]
                 ? STT_STATIC
                 : (positionTypes[splineY] ? STT_DYNAMIC : STT_IDENTITY);
    case ttPosZ:
      return positionTypes[staticZ]
                 ? STT_STATIC
                 : (positionTypes[splineZ] ? STT_DYNAMIC : STT_IDENTITY);
    case ttRotation:
      return rotationTypes & 0xf0
                 ? STT_DYNAMIC
                 : (rotationTypes & 0xf ? STT_STATIC : STT_IDENTITY);
    case ttScaleX:
      return scaleTypes[staticX]
                 ? STT_STATIC
                 : (scaleTypes[splineX] ? STT_DYNAMIC : STT_IDENTITY);
    case ttScaleY:
      return scaleTypes[staticY]
                 ? STT_STATIC
                 : (scaleTypes[splineY] ? STT_DYNAMIC : STT_IDENTITY);
    case ttScaleZ:
      return scaleTypes[staticZ]
                 ? STT_STATIC
                 : (scaleTypes[splineZ] ? STT_DYNAMIC : STT_IDENTITY);
    }

    return STT_IDENTITY; // Warning fodder
  }
};

template <class C> struct ISplineTrack {
  virtual bool IsStatic() = 0;
  virtual C GetValue(float localFrame) = 0;
  virtual ~ISplineTrack() {}
};

template <class C> struct SplineStaticTrack : ISplineTrack<C> {
  C item;
  C GetValue(float) { return item; }
  bool IsStatic() { return true; }
};

struct SplineDynamicTrackVector : ISplineTrack<Vector> {
  std::vector<float> tracks[3];
  std::span<uint8> knots;
  uint8 degree;

  Vector GetValue(float localFrame);
  bool IsStatic() { return !knots.size(); }
};

struct SplineDynamicTrackQuat : ISplineTrack<Vector4A16> {
  std::vector<Vector4A16> track;
  std::span<uint8> knots;
  uint8 degree;

  Vector4A16 GetValue(float localFrame);
  bool IsStatic() { return false; }
};
struct TransformTrack {
  template <class C> using TrackType = std::unique_ptr<ISplineTrack<C>>;
  TrackType<Vector> pos;
  TrackType<Vector4A16> rotation;
  TrackType<Vector> scale;
};

struct TransformSplineBlock {
  void Assign(char *buffer, size_t numTracks, size_t numFloatTractks);
  void GetValue(size_t trackID, float time, hkQTransform &out) const;

private:
  std::span<TransformMask> masks;
  std::vector<TransformTrack> tracks;
};

struct hkaSplineDecompressor {
  std::vector<TransformSplineBlock> blocks;
  // TODO floats
  void Assign(hkaSplineCompressedAnimationInternalInterface *input);
};

inline void TransformSplineBlock::GetValue(size_t trackID, float time,
                                           hkQTransform &out) const {
  out.rotation = tracks[trackID].rotation->GetValue(time);
  out.translation = tracks[trackID].pos->GetValue(time);
  out.scale = tracks[trackID].scale->GetValue(time);
}
