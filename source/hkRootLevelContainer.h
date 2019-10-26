/*  Havok Format Library
    Copyright(C) 2016-2019 Lukas Cone

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

template <class C>
struct hkRootLevelContainer_t : hkRootLevelContainerInternalInterface {
  C *Data;
  hkClassConstructor(hkRootLevelContainer_t<C>);

  void SwapEndian() override { Data->SwapEndian(); }
  const int GetNumVariants() const override { return Data->GetNumVariants(); }
  const hkNamedVariant GetVariant(int id) const override {
    return Data->GetVariant(header, id);
  }
};

template <template <class C> class _ipointer> struct hkNamedVariant_t {
  _ipointer<char> name;
  _ipointer<char> classname;
  hkVariant<char, _ipointer> variant;
  ES_FORCEINLINE const hkNamedVariant GetVariant(IhkPackFile *header) const {
    return hkNamedVariant{name, classname, header->GetClass(variant.object)};
  }
};

template <template <class C> class _ipointer> struct hkNamedVariant2_t {
  _ipointer<char> name;
  _ipointer<char> classname;
  _ipointer<char> variant;
  ES_FORCEINLINE const hkNamedVariant GetVariant(IhkPackFile *header) const {
    return hkNamedVariant{name, classname, header->GetClass(variant)};
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkRootLevelContainer_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;
  enablePtrPair(variants) GetNumVariants() const { return this->numVariants; }
  enablehkArray(variants) GetNumVariants() const {
    return this->variants.count;
  }
  enablePtrPairRef(variants) GetNumVariants() { return this->numVariants; }
  enablehkArrayRef(variants) GetNumVariants() { return this->variants.count; }

  ES_FORCEINLINE const hkNamedVariant GetVariant(IhkPackFile *header,
                                                 int id) const {
    return parent_class::variants[id].GetVariant(header);
  }

  ES_FORCEINLINE void SwapEndian() { FByteswapper(GetNumVariants()); }
};

template <template <class C> class _ipointer>
struct hkRootLevelContainer660_t_sharedData {
  _ipointer<hkNamedVariant_t<_ipointer>> variants;
  int numVariants;
};

template <template <class C> class _ipointer>
using hkRootLevelContainer660_t =
    hkRootLevelContainer_t_shared<_ipointer,
                                  hkRootLevelContainer660_t_sharedData>;
template <template <class C> class _ipointer>
struct hkRootLevelContainer660_rp_t : hkRootLevelContainer660_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkRootLevelContainer710_t_sharedData {
  hkArray<hkNamedVariant2_t<_ipointer>, _ipointer> variants;
};

template <template <class C> class _ipointer>
using hkRootLevelContainer710_t =
    hkRootLevelContainer_t_shared<_ipointer,
                                  hkRootLevelContainer710_t_sharedData>;
template <template <class C> class _ipointer>
struct hkRootLevelContainer710_rp_t : hkRootLevelContainer710_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkRootLevelContainer2010_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2010_rp_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2011_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2011_rp_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2012_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2012_rp_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2013_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2013_rp_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2014_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2014_rp_t : hkRootLevelContainer710_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkRootLevelContainer550_t : hkRootLevelContainer660_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer550_rp_t : hkRootLevelContainer660_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkRootLevelContainer2015_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2015_rp_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2016_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2016_rp_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2017_t : hkRootLevelContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkRootLevelContainer2017_rp_t : hkRootLevelContainer710_t<_ipointer> {};