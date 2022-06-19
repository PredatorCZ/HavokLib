/*  Havok Format Library
    Copyright(C) 2016-2022 Lukas Cone

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
#include <span>

struct hkaSplineCompressedAnimationInternalInterface
    : virtual hkaAnimationInternalInterface {
  virtual char *GetData() const = 0;
  virtual std::span<const uint32> GetBlockOffsets() const = 0;
  virtual std::span<const uint32> GetFloatBlockOffsets() const = 0;
  virtual std::span<const uint32> GetTransformOffsets() const = 0;
  virtual std::span<const uint32> GetFloatOffsets() const = 0;
  virtual uint32 GetNumFrames() const = 0;
  virtual uint32 GetNumBlocks() const = 0;
  virtual uint32 GetMaxFramesPerBlock() const = 0;
  virtual float GetBlockDuration() const = 0;
  virtual float GetBlockInverseDuration() const = 0;
  virtual float GetFrameDuration() const = 0;

  // void ToXML(XMLHandle hdl) const;
  static hkVirtualClass *Create(CRule rule);
};
