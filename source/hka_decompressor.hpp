/*  Havok Format Library
    Copyright(C) 2016-2020 Lukas Cone

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
#include "datas/allocator_hybrid.hpp"
#include "datas/endian.hpp"
#include "datas/flags.hpp"
#include "datas/vectors_simd.hpp"
#include <vector>
#include <memory>

template <typename T> using hvector = std::vector<T, es::allocator_hybrid<T>>;

struct MasterTrack;

struct QuantizationFormat {
  uint8 maxBitWidth, preserved;
  uint32 numD, offsetIdx, scaleIdx, bitWidthIdx;

  void SwapEndian() {
    FByteswapper(numD);
    FByteswapper(offsetIdx);
    FByteswapper(scaleIdx);
    FByteswapper(bitWidthIdx);
  }
};

enum TrackType { TT_DYNAMIC, TT_STATIC, TT_IDENTITY };

class StaticMask {
public:
  enum MaskType {
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

private:
  union {
    es::Flags<uint16> data;
    uint16 dataRaw;
  };
  TrackType GetTrackType(uint32 offset) const {
    return static_cast<TrackType>((dataRaw >> offset) & 3);
  }

public:
  TrackType GetPosTrackType() const { return GetTrackType(ttPos); }
  TrackType GetRotTrackType() const { return GetTrackType(ttRot); }
  TrackType GetScaleTrackType() const { return GetTrackType(ttScale); }
  bool UseSubTrack(MaskType type) const { return data[type]; }
};

template <class C> struct ITrack {
  virtual C &GetFrame(int32 frame) = 0;
  virtual size_t NumFrames() const { return 2; }
  virtual ~ITrack() {}
};

template <class C> struct DynamicTrack : ITrack<C> {
  std::vector<C> items;
  C &GetFrame(int32 frame) { return items[frame]; }
  size_t NumFrames() const { return items.size(); }
};

template <class C> struct StaticTrack : ITrack<C> {
  C track;
  C &GetFrame(int32 frame) { return track; }
  StaticTrack(C input) : track(input) {}
  StaticTrack() : track() {}
};

struct MasterTrack {
  typedef ITrack<Vector4A16> VectorTrack;
  typedef std::unique_ptr<VectorTrack> VectorTrackPtr;
  VectorTrackPtr pos, rot, scale;

  MasterTrack(const StaticMask &mask);
};
