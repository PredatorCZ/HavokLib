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
#include "datas/pointer.hpp"

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

  operator C *() { return data; }
  operator const C *() const { return data; }
};

template <template <class C> class _ipointer, class _testValue>
using is_hkArray =
    std::is_same<_testValue,
                 hkArray<typename _testValue::value_type, _ipointer>>;

#define enablehkArrayArg(_value, arg)                                          \
  template <class SC = decltype(parent_class::_value)>                         \
  typename std::enable_if<is_hkArray<_ipointer, SC>::value, arg>::type

#define enablePtrPairArg(_value, arg)                                          \
  template <class SC = decltype(parent_class::_value)>                         \
  typename std::enable_if<!is_hkArray<_ipointer, SC>::value, arg>::type

#define enablehkArray(_value) enablehkArrayArg(_value, uint32)
#define enablePtrPair(_value) enablePtrPairArg(_value, uint32)
#define enablehkArrayRef(_value) enablehkArrayArg(_value, uint32 &)
#define enablePtrPairRef(_value) enablePtrPairArg(_value, uint32 &)

template <class C, template <class _C> class _ipointer> struct hkVariant {
  _ipointer<C> object;
  _ipointer<char> classDesc;
};

#define hkClassConstructor_nohash(classname, ...)                              \
  void SetDataPointer(void *Ptr) {                                             \
    this->Data = reinterpret_cast<decltype(this->Data)>(Ptr);                  \
    __VA_ARGS__                                                                \
  }                                                                            \
  const void *GetPointer() const { return this->Data; }
#define hkClassConstructor(classname, ...)                                     \
  hkClassConstructor_nohash(classname,                                         \
                            __VA_ARGS__) static const unsigned int HASH;       \
  classname() { this->hash = HASH; }
