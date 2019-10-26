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

template <class C> union hkPointerX64 {
  typedef C value_type;

private:
  C *pointer;
  uint64 bound;

public:
  operator C *() { return pointer; }
  C &operator*() { return *pointer; }
  C *operator->() { return pointer; }

  operator const C *() const { return pointer; }
  const C &operator*() const { return *pointer; }
  const C *operator->() const { return pointer; }
};

template <class C> struct _hkPointerX86 {
  typedef C value_type;

private:
  uint varPtr;

public:
  operator C *() {
    return reinterpret_cast<C *>(reinterpret_cast<char *>(&varPtr) + varPtr);
  }

  C &operator*() { return *static_cast<C *>(*this); }
  C *operator->() { return *this; }

  operator const C *() const {
    return reinterpret_cast<const C *>(reinterpret_cast<const char *>(&varPtr) +
                                       varPtr);
  }
  const C &operator*() const { return *static_cast<C *>(*this); }
  const C *operator->() const { return *this; }
};

template <class C> union _hkPointer {
  typedef C value_type;

private:
  C *pointer;

public:
  operator C *() { return pointer; }
  C &operator*() { return *pointer; }
  C *operator->() { return pointer; }

  operator const C *() const { return pointer; }
  const C &operator*() const { return *pointer; }
  const C *operator->() const { return pointer; }
};

template <class C, bool x64> struct ___hkPointerX86 {
  typedef _hkPointerX86<C> value_type;
};
template <class C> struct ___hkPointerX86<C, false> {
  typedef _hkPointer<C> value_type;
};

template <class C>
using hkPointerX86 = typename ___hkPointerX86<C, ES_X64>::value_type;

template <class C, template <class _C> class _ipointer> struct hkArray {
  typedef C value_type;

  _ipointer<C> data;
  int count;
  int capacityAndFlags;

  operator C *() { return data; }
  operator const C *() const { return data; }
};

template <template <class C> class _ipointer, class _testValue>
using is_hkArray =
    std::is_same<_testValue,
                 hkArray<typename _testValue::value_type, _ipointer>>;

#define enablehkArrayArg(_value, arg)                                          \
  template <class SC = decltype(parent_class::_value)>                         \
  typename std::enable_if<is_hkArray<_ipointer, SC>::value, arg>::type         \
      ES_FORCEINLINE

#define enablePtrPairArg(_value, arg)                                          \
  template <class SC = decltype(parent_class::_value)>                         \
  typename std::enable_if<!is_hkArray<_ipointer, SC>::value, arg>::type        \
      ES_FORCEINLINE

#define enablehkArray(_value) enablehkArrayArg(_value, const int)
#define enablePtrPair(_value) enablePtrPairArg(_value, const int)
#define enablehkArrayRef(_value) enablehkArrayArg(_value, int &)
#define enablePtrPairRef(_value) enablePtrPairArg(_value, int &)

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
