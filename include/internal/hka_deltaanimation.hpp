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
#include "hka_animation.hpp"

struct hkaDeltaCompressedAnimationInternalInterface
    : virtual hkaAnimationLerpSampler {
  virtual size_t GetNumOfPoses() const = 0;
  virtual size_t GetBlockSize() const = 0;
  virtual size_t GetQuantizedDataOffset() const = 0;
  virtual size_t GetStaticMaskOffset() const = 0;
  virtual size_t GetStaticDataOffset() const = 0;
  virtual const char *GetData() const = 0;
  virtual size_t GetNumDynamicTracks() const = 0;
  virtual size_t GetOffsetsOffset() const = 0;
  virtual size_t GetBitWidthOffset() const = 0;
  virtual size_t GetScalesOffset() const = 0;
  virtual size_t GetNumPreserved() const = 0;
  // void ToXML(XMLHandle hdl) const;
  static hkVirtualClass *Create(CRule rule);
};

