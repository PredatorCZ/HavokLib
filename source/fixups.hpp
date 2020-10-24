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
#include <vector>

class IhkVirtualClass;

struct hkFixup {
  const IhkVirtualClass *destClass;
  size_t strOffset;
  size_t destination;

  hkFixup(size_t offset, size_t dest)
      : strOffset(offset), destination(dest), destClass() {}
  hkFixup(size_t offset, const IhkVirtualClass *dest)
      : strOffset(offset), destClass(dest), destination() {}
  hkFixup(size_t offset) : strOffset(offset), destClass(), destination() {}
};

struct hkFixups {
  std::vector<hkFixup> locals;
  std::vector<hkFixup> finals;
  std::vector<hkFixup> globals;
};
