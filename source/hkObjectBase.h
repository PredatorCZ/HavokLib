/*	Havok Format Library
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
#define GNU_PADDING(x64Pad) char _padding[ES_REUSE_PADDING && sizeof(_ipointer<void>) == 8 ? x64Pad : 0]
#else
#define MSC_RP_PACK(packingVal) pack(packingVal)
#define GNU_PADDING(x64Pad)
#endif

template<template<class C>class _ipointer> struct hkReferenceObject
{
	_ipointer<int> vtable;
	short memSizeAndFlags,
		referenceCount;

	GNU_PADDING(4);
};

template<template<class C>class _ipointer> struct hkReferenceObject2016
{
	_ipointer<int> vtable;
	_ipointer<int> unk1;
	short memSizeAndFlags,
		referenceCount;

	GNU_PADDING(4);
};

#pragma MSC_RP_PACK(4)
template<template<class C>class _ipointer> struct hkReferenceObject2016_rp
{
	_ipointer<int> vtable;
	_ipointer<int> unk1;
	short memSizeAndFlags,
		referenceCount;
};

template<template<class C>class _ipointer> struct hkReferenceObject_rp
{
	_ipointer<int> vtable;
	short memSizeAndFlags,
		referenceCount;
};
#pragma pack()

template <class C> union hkPointerX64
{
	typedef C value_type;
private:
	C *pointer;
	long long bound;
public:
	ES_FORCEINLINE C *GetData(char *) { return pointer; }
	ES_FORCEINLINE const C *GetData(char *) const { return pointer; }
};

template <class C> struct hkPointerX86
{
	typedef C value_type;
private:
	uint varPtr;
public:
	template<bool enabled = ES_X64>
	ES_FORCEINLINE typename std::enable_if<enabled, C *>::type GetData(char *masterBuffer)
	{
		return reinterpret_cast<C *>(masterBuffer + varPtr);
	}

	template<bool enabled = ES_X64>
	ES_FORCEINLINE typename std::enable_if<!enabled, C *>::type GetData(char *)
	{
		return reinterpret_cast<C *>(varPtr);
	}

	template<bool enabled = ES_X64>
	ES_FORCEINLINE typename std::enable_if<enabled, const C *>::type GetData(char *masterBuffer) const
	{
		return reinterpret_cast<const C *>(masterBuffer + varPtr);
	}

	template<bool enabled = ES_X64>
	ES_FORCEINLINE typename std::enable_if<!enabled, const C *>::type GetData(char *) const
	{
		return reinterpret_cast<const C *>(varPtr);
	}
};

template <class C, template<class _C>class _ipointer> struct hkArray
{
	_ipointer<C> Data;
	typedef C value_type;
	int count;
	int capacityAndFlags;
	ES_FORCEINLINE C *GetData(char *data) { return Data.GetData(data); }
	ES_FORCEINLINE const C *GetData(char *data) const { return Data.GetData(data); }
	//ES_FORCEINLINE typename std::enable_if<std::is_const<C>::value, C*>::type GetData(char *data) const { return Data.GetData(data); }
};

template<template<class C>class _ipointer, class _testValue> 
using is_hkArray = std::is_same<_testValue, hkArray<typename _testValue::value_type, _ipointer>>;


#define enablehkArrayArg(_value, arg)template<class SC = decltype(parent_class::_value)>\
typename std::enable_if<is_hkArray<_ipointer, SC>::value, arg>::type ES_FORCEINLINE

#define enablePtrPairArg(_value, arg)template<class SC = decltype(parent_class::_value)>\
typename std::enable_if<!is_hkArray<_ipointer, SC>::value, arg>::type ES_FORCEINLINE


#define enablehkArray(_value)enablehkArrayArg(_value, const int)
#define enablePtrPair(_value)enablePtrPairArg(_value, const int)
#define enablehkArrayRef(_value)enablehkArrayArg(_value, int&)
#define enablePtrPairRef(_value)enablePtrPairArg(_value, int&)

template <class C, template<class _C>class _ipointer> struct hkVariant
{
	_ipointer<C> object;
	_ipointer<void> classDesc;
};

template<class _enum, class _storage> struct hkEnum
{
private:
	_storage storage;
public:
	operator _enum() const
	{
		return static_cast<_enum>(storage);
	}
	operator _storage() const
	{
		return storage;
	}
	void operator=(_enum input)
	{
		storage = static_cast<_storage>(input);
	}
};

#define hkClassConstructor_nohash(classname,...) void SetDataPointer(void *Ptr){this->Data = reinterpret_cast<decltype(this->Data)>(Ptr); __VA_ARGS__} const void *GetPointer() const { return this->Data; }
#define hkClassConstructor(classname,...) hkClassConstructor_nohash(classname, __VA_ARGS__) static const unsigned int HASH; classname(){this->hash = HASH;}