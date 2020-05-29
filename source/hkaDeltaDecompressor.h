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
#include "hkInternalInterfaces.h"
#include "hkaDecompressor.h"

struct hkaDeltaDecompressor {
  typedef hvector<unsigned char> bitWidths_type;
  typedef hvector<StaticMask> masks_type;
  typedef hvector<float> offsets_type;
  typedef hvector<float> scales_type;
  typedef ITrack<float> floats_track;

private:
  hvector<unsigned char> bitWidths;
  hvector<StaticMask> masks;
  hvector<float> offsets;
  hvector<float> scales;
  std::vector<std::unique_ptr<MasterTrack>> tracks;
  std::vector<std::unique_ptr<floats_track>> floats;

public:
  void Assign(hkaDeltaCompressedAnimationInternalInterface *input);
  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const;
};

inline void hkaDeltaDecompressor::GetFrame(size_t trackID, int32 frame, hkQTransform &out) const {
  out.rotation = tracks[trackID]->rot->GetFrame(frame);
  out.translation = tracks[trackID]->pos->GetFrame(frame);
  out.scale = tracks[trackID]->scale->GetFrame(frame);
}
