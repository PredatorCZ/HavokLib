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
#include "datas/binwritter.hpp"
#include "datas/macroLoop.hpp"
#include "datas/pointer.hpp"
#include "fixups.hpp"
#include "hklib/hk_packfile.hpp"
#include <unordered_map>

#ifndef _MSC_VER
#define MSC_RP_PACK(packingVal) pack()
#define GNU_PADDING(x64Pad)                                                    \
  char _padding[ES_REUSE_PADDING && sizeof(_ipointer<char>) == 8 ? x64Pad : 0]
#else
#define MSC_RP_PACK(packingVal) pack(packingVal)
#define GNU_PADDING(x64Pad)
#endif

template <template <class C> class _ipointer> struct hkReferenceObject {
  _ipointer<int> vtable;
  short memSizeAndFlags, referenceCount;

  GNU_PADDING(4);
};

template <template <class C> class _ipointer> struct hkReferenceObject2016 {
  _ipointer<int> vtable;
  _ipointer<int> unk1;
  short memSizeAndFlags, referenceCount;

  GNU_PADDING(4);
};

#pragma MSC_RP_PACK(4)
template <template <class C> class _ipointer> struct hkReferenceObject2016_rp {
  _ipointer<int> vtable;
  _ipointer<int> unk1;
  short memSizeAndFlags, referenceCount;
};

template <template <class C> class _ipointer> struct hkReferenceObject_rp {
  _ipointer<int> vtable;
  short memSizeAndFlags, referenceCount;
};
#pragma pack()

template <class C, template <class _C> class _ipointer> struct hkArray {
  typedef C value_type;

  _ipointer<C> data;
  uint32 count;
  uint32 capacityAndFlags;

  hkArray() : data(), count(0), capacityAndFlags(0x80000000) {}

  operator C *() { return data; }
  operator const C *() const { return data; }
};

template <class C, template <class _C> class _ipointer> struct hkVariant {
  _ipointer<C> object;
  _ipointer<char> classDesc;
};

#define hkClassContruct(...)                                                   \
  []() -> hkVirtualClass * { return new __VA_ARGS__{}; }
// clang-format off
#define hkRegisterVersionedClass(clsnm, version)\
  {{version, false, false}, hkClassContruct(clsnm##_t<clsnm##Tier<version, esPointerX86, false>>)},\
  {{version, true, false}, hkClassContruct(clsnm##_t<clsnm##Tier<version, esPointerX86, true>>)},\
  {{version, false, true}, hkClassContruct(clsnm##_t<clsnm##Tier<version, esPointerX64, false>>)},\
  {{version, true, true}, hkClassContruct(clsnm##_t<clsnm##Tier<version, esPointerX64, true>>)}

#define hkRegisterFullClass(classname)\
  hkRegisterVersionedClass(classname, HK500),\
  hkRegisterVersionedClass(classname, HK510),\
  hkRegisterVersionedClass(classname, HK550),\
  hkRegisterVersionedClass(classname, HK610),\
  hkRegisterVersionedClass(classname, HK650),\
  hkRegisterVersionedClass(classname, HK660),\
  hkRegisterVersionedClass(classname, HK700),\
  hkRegisterVersionedClass(classname, HK710),\
  hkRegisterVersionedClass(classname, HK2010_1),\
  hkRegisterVersionedClass(classname, HK2010_2),\
  hkRegisterVersionedClass(classname, HK2011_1),\
  hkRegisterVersionedClass(classname, HK2011_2),\
  hkRegisterVersionedClass(classname, HK2011_3),\
  hkRegisterVersionedClass(classname, HK2012_1),\
  hkRegisterVersionedClass(classname, HK2012_2),\
  hkRegisterVersionedClass(classname, HK2013),\
  hkRegisterVersionedClass(classname, HK2014),\
  hkRegisterVersionedClass(classname, HK2015),\
  hkRegisterVersionedClass(classname, HK2016),\
  hkRegisterVersionedClass(classname, HK2017)

// clang-format on

#define _hkRegVerClass(classname, id, value)                                   \
  hkRegisterVersionedClass(classname, HK##value),
#define hkRegisterVersionedClasses(classname, ...)                             \
  StaticForArgID(_hkRegVerClass, classname, __VA_ARGS__)

#define hkClassTier0Alias(classname, sversion, oversion)                       \
  template <template <class C> class _ipointer, bool rp>                       \
  struct classname##Tier<HK##oversion, _ipointer, rp>                          \
      : classname##sversion<_ipointer> {};

#define hkClassTier1Alias(classname, sversion, oversion)                       \
  template <template <class C> class _ipointer, bool rp>                       \
  struct classname##Tier<HK##oversion, _ipointer, rp>                          \
      : classname##sversion<_ipointer, rp> {};

#define GetCountConst(varName)                                                 \
  template <class C = parent_class>                                            \
  auto GetNum##varName(int) const->decltype(std::declval<C>().num##varName) {  \
    return this->num##varName;                                                 \
  }                                                                            \
  template <class C = parent_class>                                            \
  auto GetNum##varName(int) const->decltype(std::declval<C>().varName.count) { \
    return this->varName.count;                                                \
  }                                                                            \
  template <class C = parent_class> size_t GetNum##varName(...) const {        \
    return 0;                                                                  \
  }

#define GetCountRef(varName)                                                   \
  template <class C = parent_class>                                            \
  auto GetNum##varName(int)                                                    \
      ->typename std::add_lvalue_reference<decltype(                           \
          std::declval<C>().num##varName)>::type {                             \
    return this->num##varName;                                                 \
  }                                                                            \
  template <class C = parent_class>                                            \
  auto GetNum##varName(int)                                                    \
      ->typename std::add_lvalue_reference<decltype(                           \
          std::declval<C>().varName.count)>::type {                            \
    return this->varName.count;                                                \
  }                                                                            \
  template <class C = parent_class> uint32 &GetNum##varName(...) {             \
    static uint32 dummy = 0;                                                   \
    return dummy;                                                              \
  }

#define GetNum(varName) GetCountRef(varName) GetCountConst(varName)

#define AccessMethodSingle(varName, defValue)                                  \
  template <class C = parent_class>                                            \
  auto Get##varName(...) const->decltype(defValue) {                           \
    return defValue;                                                           \
  }                                                                            \
  template <class C = parent_class>                                            \
  auto Get##varName(size_t id)                                                 \
      const->decltype(std::declval<C>().varName, defValue)

#define AccessMethod(varName, defValue)                                        \
  template <class C = parent_class>                                            \
  auto Get##varName(...) const->decltype(defValue) {                           \
    return defValue;                                                           \
  }                                                                            \
  template <class C = parent_class>                                            \
  auto Get##varName(size_t id)                                                 \
      const->decltype(std::declval<C>().varName##s, defValue)

#define AccessMethod2(varName, defValue)                                       \
  template <class C = parent_class>                                            \
  auto Get##varName(...) const->decltype(defValue) {                           \
    return defValue;                                                           \
  }                                                                            \
  template <class C = parent_class>                                            \
  auto Get##varName(size_t id)                                                 \
      const->decltype(std::declval<C>().num##varName##s, defValue)

#define AccessMethod2Arr(varName, defValue)                                    \
  template <class C = parent_class>                                            \
  auto Get##varName(size_t id)                                                 \
      const->decltype(std::declval<C>().varName##s.count, defValue)

#define SwapMethod(varName)                                                    \
  template <class C = parent_class> void Swap##varName(...) {}                 \
  template <class C = parent_class>                                            \
  auto Swap##varName(int)->decltype(std::declval<C>().varName, void())

#define SwapMethod2(varName)                                                   \
  template <class C = parent_class> void Swap##varName(...) {}                 \
  template <class C = parent_class>                                            \
  auto Swap##varName(int)->decltype(std::declval<C>().num##varName, void())

#define SwapMethod2Arr(varName)                                                \
  template <class C = parent_class>                                            \
  auto Swap##varName(int)->decltype(std::declval<C>().varName.count, void())
