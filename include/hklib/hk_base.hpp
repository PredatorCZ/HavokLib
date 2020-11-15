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
#include "datas/jenkinshash.hpp"
#include "datas/reflector.hpp"
#include "uni/common.hpp"

#define DECLARE_HKCLASS(classname)                                             \
  static constexpr JenHash GetHash() { return JenHash(#classname); }

REFLECTOR_CREATE(hkToolset, ENUM, 1, 8, HKUNKVER, HK500, HK510, HK550, HK600,
                 HK610, HK650, HK660, HK700, HK710, HK2010_1, HK2010_2,
                 HK2011_1, HK2011_2, HK2011_3, HK2012_1, HK2012_2, HK2013,
                 HK2014, HK2015, HK2016, HK2017);

namespace pugi {
class xml_node;
class xml_document;
}; // namespace pugi
typedef pugi::xml_node XMLnode;

struct XMLHandle {
  XMLnode *node;
  hkToolset toolset;
};

struct IhkVirtualClass {
  virtual const void *GetPointer() const = 0;
  virtual ~IhkVirtualClass() {}
};

using hkQTransform = uni::RTSValue;
