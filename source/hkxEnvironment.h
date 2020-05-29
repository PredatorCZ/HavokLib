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
#include "hkInternalInterfaces.h"
#include "hkObjectBase.h"

template <template <class C> class _ipointer> struct hkxEnvironmentVariable_t {
  _ipointer<const char> name;
  _ipointer<const char> value;
};

template <class C> struct hkxEnvironment_t : hkxEnvironmentInternalInterface {
  C *Data;
  hkClassConstructor(hkxEnvironment_t);

  void SwapEndian() { Data->SwapEndian(); }
  size_t Size() const { return Data->GetNumVars(); }
  const hkxEnvironmentVariable At(size_t id) const { return Data->GetVar(id); }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkxEnvironment_t_shared : _parent<_ipointer> {
  size_t GetNumVars() const { return this->variables.count; }
  hkxEnvironmentVariable GetVar(size_t id) const {
    const hkxEnvironmentVariable_t<_ipointer> &tmpVal = this->variables[id];
    return {static_cast<const char *>(tmpVal.name),
            static_cast<const char *>(tmpVal.value)};
  }
  void SwapEndian() { FByteswapper(this->variables.count); }
};

template <template <class C> class _ipointer>
struct hkxEnvironment550_t_sharedData {
  hkArray<hkxEnvironmentVariable_t<_ipointer>, _ipointer> variables;
};

template <template <class C> class _ipointer>
using hkxEnvironment550_t =
    hkxEnvironment_t_shared<_ipointer, hkxEnvironment550_t_sharedData>;
template <template <class C> class _ipointer>
struct hkxEnvironment550_rp_t : hkxEnvironment550_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkxEnvironment660_t : hkxEnvironment550_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment660_rp_t : hkxEnvironment550_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkxEnvironment710_t_sharedData : hkReferenceObject<_ipointer> {
  hkArray<hkxEnvironmentVariable_t<_ipointer>, _ipointer> variables;
};

template <template <class C> class _ipointer>
using hkxEnvironment710_t =
    hkxEnvironment_t_shared<_ipointer, hkxEnvironment710_t_sharedData>;
template <template <class C> class _ipointer>
struct hkxEnvironment710_rp_t : hkxEnvironment710_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkxEnvironment2010_t : hkxEnvironment710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment2010_rp_t : hkxEnvironment710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment2011_t : hkxEnvironment710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment2011_rp_t : hkxEnvironment710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment2012_t : hkxEnvironment710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment2012_rp_t : hkxEnvironment710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment2013_t : hkxEnvironment710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment2013_rp_t : hkxEnvironment710_rp_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment2014_t : hkxEnvironment710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkxEnvironment2014_rp_t : hkxEnvironment710_rp_t<_ipointer> {};