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
#include "datas/jenkinshash.hpp"
#include "datas/pugi_fwd.hpp"
#include "datas/reflector.hpp"
#include "uni/common.hpp"
#include "uni/rts.hpp"

#define DECLARE_HKCLASS(classname)                                             \
  static constexpr JenHash GetHash() { return JenHash(#classname); }

MAKE_ENUM(ENUMSCOPE(hkToolset : uint8, hkToolset), EMEMBER(HKUNKVER), EMEMBER(HK500), EMEMBER(HK510),
          EMEMBER(HK550), EMEMBER(HK600), EMEMBER(HK610), EMEMBER(HK650),
          EMEMBER(HK660), EMEMBER(HK700), EMEMBER(HK710), EMEMBER(HK2010_1),
          EMEMBER(HK2010_2), EMEMBER(HK2011_1), EMEMBER(HK2011_2),
          EMEMBER(HK2011_3), EMEMBER(HK2012_1), EMEMBER(HK2012_2),
          EMEMBER(HK2013), EMEMBER(HK2014), EMEMBER(HK2015), EMEMBER(HK2016),
          EMEMBER(HK2017));

using XMLnode = pugi::xml_node;

struct XMLHandle {
  XMLnode *node;
  hkToolset toolset;
};

struct IhkVirtualClass {
  virtual const void *GetPointer() const = 0;
  virtual ~IhkVirtualClass() {}
};

using hkQTransform = uni::RTSValue;
