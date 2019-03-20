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
#include "hkInternalInterfaces.h"

#define hkMagic1	0x57e0e057
#define hkMagic2	0x10c0c010

class BinReader;
struct hkxHeader;

struct hkxHeaderlayout
{
	/*
	This is basically pointer size, so 4 bytes for x86 and 8 bytes for x64 architectures
	*/
	char bytesInPointer,
		big_endian,
		/*
		derived classes start instead of base class end padding, rather after, very usefull thing
		this technique is applied only on non POD classes
		eg: struct Base {}; we must create base class  for A or A will be handled as POD class, hence compiler will have forbidden access to padding
		struct A : Base {int i; char c;}; sizeof (A) == 8
		struct B : A {short s;}; sizeof (B) == 8
		so compiled struct looks like this:
		struct B
		{
			int i;
			char c;
			char pad;
			short s;
		}
		instead of:
		struct B
		{
			int i;
			char c;
			char pad[3];
			short s;
			char pad[2];
		}
		this technique is used by many compilers like gcc, clang, mac compilers, this is not used by MSVC, or any MS like compilers
		*/
		reusePaddingOptimization,
		/*
		empty base class have sizeof only 1 byte, however this byte is not passed to dervied classes, when optimization is applied
		eg: struct A {}; sizeof(A) == 1
		struct B:A{int i;}; sizeof(B) == 4
		this does not apply if A is member of struct, in that case, it will be handled as char?
		thankfully this technique doesn't use sn and gcc v2 compilers (I cannot event find SN compiler reference, lol)
		*/
		emptyBaseClassOptimization;
};


struct hkxSectionHeader
{

	struct hkxLocalFixup
	{
		int pointer,
			destination;
		ES_FORCEINLINE void SwapEndian()
		{
			FByteswapper(pointer);
			FByteswapper(destination);
		}
	};

	struct hkxGlobalFixup
	{
		int pointer,
			sectionid,
			destination;
		ES_FORCEINLINE void SwapEndian()
		{
			FByteswapper(pointer);
			FByteswapper(destination);
			FByteswapper(sectionid);
		}
	};

	struct hkxVirtualFixup
	{
		int dataoffset,
			sectionid,
			classnameoffset;
		ES_FORCEINLINE void SwapEndian()
		{
			FByteswapper(dataoffset);
			FByteswapper(sectionid);
			FByteswapper(classnameoffset);
		}
	};

	char sectionTag[20];
	int	absoluteDataStart,
		localFixupsOffset,
		globalFixupsOffset,
		virtualFixupsOffset,
		exportsOffset,
		importsOffset,
		bufferSize;
	static const int PODSize;

	int sectionID;
	int sectionBufferSize;
	char *sectionBuffer;
	std::vector<hkxLocalFixup> localFixups;
	std::vector<hkxGlobalFixup> globalFixups;
	std::vector<hkxVirtualFixup> virtualFixups;
	IhkPackFile::VirtualClasses virtualClasses;
	hkxHeader *header;
	int Load(BinReader *rd);
	int LoadBuffer(BinReader *rd);
	int LinkBuffer();
	int LinkBuffer86();

	~hkxSectionHeader();
	ES_FORCEINLINE void SwapEndian();
};

struct hkxHeader : IhkPackFile
{
	int magic1,
		magic2,
		userTag,
		Version;
	hkxHeaderlayout layout;
	int numSections,
		contentsSectionIndex,
		contentsSectionOffset,
		contentsClassNameSectionIndex,
		contentsClassNameSectionOffset;
	char contentsVersion[16];
	int flags;
	short maxpredicate,
		predicateArraySizePlusPadding;

	char contentsVersionStripped[5];
	/*
	1 = from 32 bit to 64
	-1 = from 64 bit to 32
	0 = unchanged
	*/
	int swapway;
	std::vector<hkxSectionHeader> sections;

	int Load(BinReader &rd);
	ES_FORCEINLINE hkxSectionHeader *GetDataSection() { return &sections[contentsSectionIndex]; }
	ES_FORCEINLINE VirtualClasses &GetAllClasses() { return GetDataSection()->virtualClasses; }
	int GetVersion();
	ES_FORCEINLINE void SwapEndian();
	~hkxHeader() {}
};
