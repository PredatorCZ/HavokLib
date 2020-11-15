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

#include "hklib/hk_base.hpp"
#include "datas/flags.hpp"
#include <map>

struct xmlToolsetProp {
  enum xmlToolsetPropFlags : uint8 { TopLevelObject, MaxPredicate };
  es::Flags<xmlToolsetPropFlags> flags;
  uint8 version;
  const char name[16];
};

extern const std::map<hkToolset, xmlToolsetProp> xmlToolsetProps;
