/*  Havok Format Library
    Copyright(C) 2016-2025 Lukas Cone

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
#include "spike/uni/list.hpp"

struct hkxAttribute {
  std::string_view name;
  const IhkVirtualClass *value;
};

struct hkxAttributeGroup : uni::Vector<hkxAttribute> {
  virtual std::string_view Name() const = 0;
};

struct hkxAttributeHolder : virtual IhkxVirtualClass,
                            virtual uni::List<hkxAttributeGroup> {
  DECLARE_HKCLASS(hkxAttributeHolder)
};
