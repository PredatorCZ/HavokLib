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

#include "HavokApi.hpp"
#include "hkHeader.hpp"
#include "hkNewHeader.h"
#include "datas/binreader.hpp"
#include "datas/masterprinter.hpp"

#define HashClass(cls) const JenHash cls::HASH = JenkinsHash(#cls, sizeof(#cls) - 1);

HashClass(hkaSkeleton)
HashClass(hkRootLevelContainer)
HashClass(hkaAnimationContainer)
HashClass(hkaAnnotationTrack)
HashClass(hkaAnimation)


IhkPackFile *IhkPackFile::Create(const wchar_t *fileName)
{
	BinReader rd(fileName);

	if (!rd.IsValid())
	{
		printerror("[hk] Cannot open file. ", << fileName);
		return nullptr;
	}

	struct 
	{
		int ID1,
			ID2;
	} testerStruct;
	
	rd.Read(testerStruct);
	rd.Seek(0);
	
	if (testerStruct.ID1 == hkMagic1 && testerStruct.ID2 == hkMagic2)
	{
		hkxHeader *hdr = new hkxHeader{};

		if (hdr->Load(rd))
		{
			delete hdr;
			hdr = nullptr;
		}
		return hdr;
	}
	else if (testerStruct.ID2 == hkHederTAG)
	{
		hkxNewHeader *hdr = new hkxNewHeader{};

		if (hdr->Load(&rd))
		{
			delete hdr;
			hdr = nullptr;
		}
		return hdr;
	}
	else
	{
		printerror("[Havok] Invalid packfile.");
	}

	return nullptr;
}

IhkPackFile::~IhkPackFile() {}