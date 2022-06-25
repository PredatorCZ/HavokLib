/*  Havok Format Library
    Copyright(C) 2020-2022 Lukas Cone

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
#include "internal/hka_animation.hpp"

struct hkaLosslessCompressedAnimationInternalInterface
    : virtual hkaAnimationLerpSampler {
  operator hkVirtualClass const *() const override { return this; }
  static IhkVirtualClass *Create(CRule rule);
};
