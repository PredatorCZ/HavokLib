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
#include "HavokApi.hpp"

#define hkHederTAG 0x30474154

class BinReader;
struct hkxNewHeader;

union chunkCC
{
	char fourCC[4];
	uint hash;
	ES_FORCEINLINE bool operator==(uint input) { return input == hash; }
	ES_FORCEINLINE bool operator!=(uint input) { return input != hash; }
};

class hkChunk
{
	int sizeAndFlags;
public:
	chunkCC tag;
	ES_FORCEINLINE void Reorder() 
	{
		FByteswapper(sizeAndFlags);
		sizeAndFlags = ((sizeAndFlags & 0xffffff) - 8) | sizeAndFlags & 0xff000000;
	}
	ES_FORCEINLINE int Size() const { return sizeAndFlags & 0xffffff; }
	ES_FORCEINLINE bool IsSubChunk() const { return (sizeAndFlags & 0x40000000) != 0; }
	int Read(BinReader *rd, hkxNewHeader *root);
};

typedef std::vector<const char*> _clVec;
struct ClassName;

struct ClassTemplateArgument
{
	const char *argName;
	ClassName *argType;
};

struct ClassName
{
	const char *className;
	std::vector<ClassTemplateArgument> templateArguments;
};

struct classEntryFixup : hkChunk
{
	int count;
};

struct hkxNewHeader : IhkPackFile, hkChunk
{
	char contentsVersionStripped[5];
	char *dataBuffer;
	char *classNamesBuffer;
	char *memberNamesBuffer;

	_clVec classNames;
	_clVec memberNames;
	std::vector<ClassName> weldedClassNames;
	std::vector<classEntryFixup> classEntries;

	std::vector<hkVirtualClass*> virtualClasses;

	std::vector<hkVirtualClass*> GetClasses(const char* hkClassName);
	const hkVirtualClass *GetClass(const void *ptr);
	std::vector<hkVirtualClass*> &GetAllClasses() { return virtualClasses; }
	int GetVersion();
	int Load(BinReader *rd);
	~hkxNewHeader();
	void DumpClassNames(std::ostream &str);

};