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

struct hkNamedVariant {
  es::string_view name;
  es::string_view className;
  const IhkVirtualClass *pointer;

  template <class C> operator const C *() const {
    return safe_deref_cast<const C>(pointer);
  }
  operator es::string_view() const { return name; }
  operator JenHash() const { return JenHash(className); }
  bool operator==(const JenHash iHash) const {
    return iHash == JenHash(className);
  }

private:
  friend class hkRootLevelContainerInternalInterface;
  void ToXML(XMLHandle hdl) const;
};

struct hkRootLevelContainer : uni::Vector<hkNamedVariant>, virtual IhkVirtualClass {
  DECLARE_HKCLASS(hkRootLevelContainer)
};
