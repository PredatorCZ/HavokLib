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

#include "hkHeader.hpp"
#include "datas\endian.hpp"
#include "datas\binreader.hpp"
#include "datas\MasterPrinter.hpp"
#include "datas\jenkinshash.hpp"
#include <algorithm>
#include <string>
#include <ctype.h>

const int hkxSectionHeader::PODSize = offsetof(hkxSectionHeader, bufferSize) + 4;

int hkxHeader::Load(BinReader & rd)
{
	const int podsize = getBlockSize(hkxHeader, magic1, predicateArraySizePlusPadding);
	rd.Read(magic1, podsize);

	if (magic1 != hkMagic1 || magic2 != hkMagic2)
	{
		printerror("[Havok] Invalid packfile.");
		return 1;
	}

	int currentSectionID = 0;

	layout.big_endian = 1 - layout.big_endian;

	if (layout.big_endian)
	{
		SwapEndian();
		rd.SwapEndian(true);
	}

	if (maxpredicate != -1)
		rd.Skip(predicateArraySizePlusPadding);

	char *cs = contentsVersionStripped;
	char *cc = contentsVersion;
	int numDots = 0, numChars = 0;

	while (cc)
	{
		if (*cc == '.')
			numDots++;

		if (isdigit(*cc))
		{
			*cs = *cc;
			numChars++;
			if (numDots > 1 || numChars > 3)
				break;
			cs++;
		}
		cc++;
	}


	/*
	1 = from 32 bit to 64
	-1 = from 64 bit to 32
	0 = unchanged
	*/
	swapway = !ES_X64 && layout.bytesInPointer == 8 ? -1 : (ES_X64 && layout.bytesInPointer == 4 ? 1 : 0);

	sections.resize(numSections);

	for (auto &s : sections)
	{
		s.header = this;
		rd.Read(s, s.PODSize);

		if (Version > 9)
			rd.Seek(16, SEEK_CUR);

		s.sectionID = currentSectionID;
		currentSectionID++;
	}

	for (auto &s : sections)
		s.Load(&rd);
	
	for (auto &s : sections)
		s.LoadBuffer(&rd);

	if (swapway == 1)
	{
		for (auto &s : sections)
			s.LinkBuffer86();
	}
	else
	{
		for (auto &s : sections)
			s.LinkBuffer();
	}

	return 0;
}

int hkxHeader::GetVersion()
{
	return atoi(contentsVersionStripped);
}

void hkxHeader::SwapEndian()
{
	FByteswapper(Version);
	FByteswapper(numSections);
	FByteswapper(contentsSectionIndex);
	FByteswapper(contentsSectionOffset);
	FByteswapper(contentsClassNameSectionIndex);
	FByteswapper(contentsClassNameSectionOffset);
	FByteswapper(flags);
	FByteswapper(maxpredicate);
	FByteswapper(predicateArraySizePlusPadding);
}

struct hkFixupSorter
{
	int *value;
	int type; // 0 = source, 1 = destination
};

int hkxSectionHeader::Load(BinReader * rd)
{
	rd->SetRelativeOrigin(absoluteDataStart);

	const int virtualEOF = (exportsOffset == -1 ? importsOffset : exportsOffset);
	const int circaNumLocalFixps = (globalFixupsOffset - localFixupsOffset) / sizeof(hkxLocalFixup);
	const int circaNumGlobalFixps = (virtualFixupsOffset - globalFixupsOffset) / sizeof(hkxGlobalFixup);
	const int circaNumVirtualFixps = (virtualEOF - virtualFixupsOffset) / sizeof(hkxVirtualFixup);

	localFixups.reserve(circaNumLocalFixps);
	globalFixups.reserve(circaNumGlobalFixps);
	virtualFixups.reserve(circaNumVirtualFixps);

	rd->Seek(localFixupsOffset);
	rd->ReadContainer(localFixups, circaNumLocalFixps);
	rd->Seek(globalFixupsOffset);
	rd->ReadContainer(globalFixups, circaNumGlobalFixps);
	rd->Seek(virtualFixupsOffset);
	rd->ReadContainer(virtualFixups, circaNumVirtualFixps);

	rd->ResetRelativeOrigin();

	return 0;
}

int hkxSectionHeader::LoadBuffer(BinReader * rd)
{

	if (!bufferSize)
		return 0;

	rd->Seek(absoluteDataStart);

	const bool containsFixups = localFixups.size() + globalFixups.size() + virtualFixups.size() > 0;

	/*if (header->swapway < 0 || !containsFixups)
	{*/
	sectionBufferSize = localFixupsOffset;
	sectionBuffer = static_cast<char*>(malloc(sectionBufferSize));
	memset(sectionBuffer, 0, sectionBufferSize);
	rd->ReadBuffer(sectionBuffer, sectionBufferSize);

	// Unfortunetly this code will not work in all cases, for exaple if middle pointer fixup is not applied, it will cause shift for structure mapping :(
	/*}
	else
	{
		const int delta = 4 * header->swapway;
		int deltaCount = 0;

		std::vector<hkFixupSorter> sortedFixups;
		sortedFixups.reserve(localFixups.size() * 2 + globalFixups.size() * 2 + virtualFixups.size() + 1);

		for (auto &lf : localFixups)
			if (lf.pointer != -1)
			{
				hkFixupSorter sr;
				sr.value = &lf.pointer;
				sr.type = 0;
				sortedFixups.push_back(sr);
				deltaCount++;

				sr.value = &lf.destination;
				sr.type = 1;
				sortedFixups.push_back(sr);
			}

		for (auto &gf : globalFixups)
			if (gf.pointer != -1)
			{
				hkFixupSorter sr;
				sr.value = &gf.pointer;
				sr.type = 0;
				sortedFixups.push_back(sr);
				deltaCount++;

				if (gf.sectionid == sectionID)
				{
					sr.value = &gf.destination;
					sr.type = 1;
					sortedFixups.push_back(sr);
				}
			}

		for (auto &vf : virtualFixups)
			if (vf.dataoffset != -1)
			{
				hkFixupSorter sr;
				sr.value = &vf.dataoffset;
				sr.type = 1;
				sortedFixups.push_back(sr);
			}

		for (auto &s : header->sections)
			if (&s != this)
				for (auto &gf : s.globalFixups)
					if (gf.sectionid == sectionID)
					{
						hkFixupSorter sr;
						sr.value = &gf.destination;
						sr.type = 1;
						sortedFixups.push_back(sr);
					}


		std::sort(sortedFixups.begin(), sortedFixups.end(), [=](hkFixupSorter &fst, hkFixupSorter &lst) {return *fst.value == *lst.value ? fst.type > lst.type : *fst.value < *lst.value; });

		const int deltaSize = deltaCount * delta;

		hkFixupSorter sr;
		sr.value = &localFixupsOffset;
		sr.type = -1;
		sortedFixups.push_back(sr);

		sectionBufferSize = localFixupsOffset + deltaSize;

		sectionBuffer = static_cast<char*>(malloc(sectionBufferSize));
		memset(sectionBuffer, 0, sectionBufferSize);
		int curDelta = 0;
		int bufferPosition = 0;

		hkFixupSorter *prevFixup = nullptr;

		for (int fid = 0; fid < sortedFixups.size(); fid++)
		{
			auto &fx1 = sortedFixups[fid];

			if (fx1.type < 1)
			{
				curDelta += delta;
				if (prevFixup == nullptr)
				{
					prevFixup = &fx1;
					continue;
				}
				else
				{

					const int readsize = *fx1.value - *prevFixup->value;
					*prevFixup->value = bufferPosition;
					bufferPosition += delta;

					rd->BaseStream->read(sectionBuffer + bufferPosition, readsize);

					bufferPosition += readsize;

					prevFixup = &fx1;
				}
			}
			else
			{
				*fx1.value += curDelta;
			}
		}
	}*/

	return 0;
}

ES_FORCEINLINE std::string _hkGenerateClassname(hkxHeader *header, std::string classname)
{
	std::string compiledClassname = classname + "_t<" + classname + header->contentsVersionStripped;

	if (header->layout.reusePaddingOptimization)
		compiledClassname.append("_rp");

	compiledClassname.append("_t<");

	switch (header->swapway)
	{
	case -1:
		compiledClassname.append("hkFakePointer");
		break;

	case 1:
		compiledClassname.append("hkStripPointer");
		break;
	default:
		compiledClassname.append("hkRealPointer");
		break;
	}

	compiledClassname.append(">>");

	return compiledClassname;
}

int hkxSectionHeader::LinkBuffer()
{
	for (auto &lf : localFixups)
		if (lf.pointer != -1)
			*reinterpret_cast<intptr_t*>(sectionBuffer + lf.pointer) = reinterpret_cast<intptr_t>(sectionBuffer + lf.destination);	

	for (auto &gf : globalFixups)
		if (gf.pointer != -1)
			*reinterpret_cast<intptr_t*>(sectionBuffer + gf.pointer) = reinterpret_cast<intptr_t>(header->sections[gf.sectionid].sectionBuffer + gf.destination);

	for (auto &vf : virtualFixups)
		if (vf.dataoffset != -1)
		{
			const char *clName = header->sections[vf.sectionid].sectionBuffer + vf.classnameoffset;
			std::string classname = _hkGenerateClassname(header, clName);

			const JenHash _chash = JenkinsHash(classname.c_str(), static_cast<int>(classname.size()));

			hkVirtualClass *cls = IhkPackFile::ConstructClass(_chash);

			if (cls)
			{
				cls->SetDataPointer(sectionBuffer + vf.dataoffset);
				cls->namePtr = clName;
				cls->superHash = JenkinsHash(clName, static_cast<int>(strlen(clName)));
				cls->masterBuffer = sectionBuffer;
				cls->header = header;
				if (header->layout.big_endian)
					cls->SwapEndian();
				virtualClasses.push_back(cls);
				cls->Process();
			}
		}

	localFixups.~vector();
	globalFixups.~vector();
	virtualFixups.~vector();

	return 0;
}

int hkxSectionHeader::LinkBuffer86()
{
	for (auto &lf : localFixups)
		if (lf.pointer != -1)
			*reinterpret_cast<int*>(sectionBuffer + lf.pointer) = lf.destination;

	for (auto &gf : globalFixups)
		if (gf.pointer != -1)
			*reinterpret_cast<int*>(sectionBuffer + gf.pointer) = gf.destination;

	for (auto &vf : virtualFixups)
		if (vf.dataoffset != -1)
		{
			const char *clName = header->sections[vf.sectionid].sectionBuffer + vf.classnameoffset;
			std::string classname = _hkGenerateClassname(header, clName);

			const JenHash _chash = JenkinsHash(classname.c_str(), static_cast<int>(classname.size()));

			hkVirtualClass *cls = IhkPackFile::ConstructClass(_chash);

			if (cls)
			{
				cls->SetDataPointer(sectionBuffer + vf.dataoffset);
				cls->namePtr = clName;
				cls->superHash = JenkinsHash(clName, static_cast<int>(strlen(clName)));
				cls->masterBuffer = sectionBuffer;
				cls->header = header;
				if (header->layout.big_endian)
					cls->SwapEndian();
				virtualClasses.push_back(cls);
				cls->Process();
			}
		}

	localFixups.~vector();
	globalFixups.~vector();
	virtualFixups.~vector();

	return 0;
}

void hkxSectionHeader::SwapEndian()
{
	FByteswapper(absoluteDataStart);
	FByteswapper(localFixupsOffset);
	FByteswapper(globalFixupsOffset);
	FByteswapper(virtualFixupsOffset);
	FByteswapper(exportsOffset);
	FByteswapper(importsOffset);
	FByteswapper(bufferSize);
}

hkxSectionHeader::~hkxSectionHeader()
{
	if (sectionBuffer)
		free(sectionBuffer);

	for (auto &v : virtualClasses)
		delete v;
}
