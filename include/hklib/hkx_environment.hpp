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
#include "hk_base.hpp"
#include "uni/list.hpp"

struct hkxEnvironmentVariable {
  es::string_view name;
  es::string_view value;
};

struct hkxEnvironment : IhkVirtualClass,
                        virtual uni::Vector<hkxEnvironmentVariable> {
  DECLARE_HKCLASS(hkxEnvironment)
};
