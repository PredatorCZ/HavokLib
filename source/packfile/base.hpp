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
#include "fixups.hpp"
#include "hklib/hk_packfile.hpp"
#include "spike/io/binwritter.hpp"
#include "spike/type/pointer.hpp"

template <class C, template <class _C> class _ipointer> struct hkArray {
  typedef C value_type;

  _ipointer<C> data;
  uint32 count;
  uint32 capacityAndFlags;

  hkArray() : data(), count(0), capacityAndFlags(0x80000000) {}

  operator C *() { return data; }
  operator const C *() const { return data; }
};
