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

#include <map>
#include "HavokXMLApi.hpp"
#include "hkHeader.hpp"
#include "hkNewHeader.h"
#include "datas/binreader.hpp"
#include "datas/masterprinter.hpp"
#include "datas/flags.hpp"
#include "pugixml.hpp"
#include "hkXML.h"

REFLECTOR_START_WNAMES(hkaPartition, name, startBoneIndex, numBones);

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

struct xmlToolsetProp
{
	enum xmlToolsetPropFlags
	{
		TopLevelObject,
		MaxPredicate
	};
	EnumFlags<uchar, xmlToolsetPropFlags> flags;
	char version[3];
	const char name[16];	
};

static const std::map<hkXMLToolsets, xmlToolsetProp> xmlToolsetProps =
{
	{HK550, {{}, "5", "Havok-5.5.0-r1"}},
	{HK660, {xmlToolsetProp::TopLevelObject, "7", "Havok-6.6.0-r1"}},
	{HK710, {xmlToolsetProp::TopLevelObject, "7", "Havok-7.1.0-r1"}},
	{HK2010_2, {xmlToolsetProp::TopLevelObject, "8", "hk_2010.2.0-r1"}},
	{HK2011, {xmlToolsetProp::TopLevelObject, "9", "hk_2011.1.0-r1"}},
	{HK2011_2, {xmlToolsetProp::TopLevelObject, "9", "hk_2011.2.0-r1"}},
	{HK2012_2, {xmlToolsetProp::TopLevelObject, "9", "hk_2012.2.0-r1"}},
	{HK2013, {xmlToolsetProp::TopLevelObject, "9", "hk_2013.1.0-r1"}},
	{HK2014, {EnumFlags<uchar, xmlToolsetProp::xmlToolsetPropFlags>(xmlToolsetProp::TopLevelObject, xmlToolsetProp::MaxPredicate), "11", "hk_2014.1.0-r1"}},
};

int IhkPackFile::ExportXML(const wchar_t *fileName, hkXMLToolsets toolsetVersion)
{
	pugi::xml_document doc;
	pugi::xml_node &master = doc.append_child("hkpackfile");

	const xmlToolsetProp &propRef = xmlToolsetProps.at(toolsetVersion);

	master.append_attribute("classversion").set_value(propRef.version);
	master.append_attribute("contentsversion").set_value(propRef.name);

	if (propRef.flags[xmlToolsetProp::MaxPredicate])
	{
		master.append_attribute("maxpredicate").set_value(21);
		master.append_attribute("predicates");
	}

	pugi::xml_node &dataSection = master.append_child("hksection");
	dataSection.append_attribute(_hkName).set_value("__data__");

	VirtualClasses allClasses = GetAllClasses();

	for (auto &c : allClasses)
	{
		hkVirtualClass *cls = dynamic_cast<hkVirtualClass*>(c);
		pugi::xml_node &classNode = dataSection.append_child(_hkObject);
		pugi::xml_attribute &addrAttr = classNode.append_attribute(_hkName);
		
		std::string _buffer;
		PointerToString(cls->GetPointer(), _buffer);

		addrAttr.set_value(_buffer.c_str());
		classNode.append_attribute(_hkClass).set_value(cls->namePtr);

		if (cls->superHash == hkRootLevelContainer::HASH && propRef.flags[xmlToolsetProp::TopLevelObject])
			master.append_attribute("toplevelobject").set_value(addrAttr.as_string());

		cls->ToXML({ &classNode, toolsetVersion });
	}

	return !doc.save_file(fileName);
}

const IhkVirtualClass *IhkPackFile::GetClass(const void *ptr)
{
	VirtualClasses &classes = GetAllClasses();

	for (auto &c : classes)
	{
		hkVirtualClass *cls = static_cast<hkVirtualClass *>(c);

		if (cls->GetPointer() == ptr)
			return c;
	}

	return nullptr;
}

IhkPackFile::VirtualClasses IhkPackFile::GetClasses(JenHash hash)
{
	VirtualClasses &classes = GetAllClasses();
	VirtualClasses buffa;

	for (auto &c : classes)
	{
		hkVirtualClass *cls = static_cast<hkVirtualClass *>(c);

		if (cls->superHash == hash)
			buffa.push_back(c);
	}

	return buffa;
}

xmlHavokFile::~xmlHavokFile()
{
	for (auto &c : classes)
		delete c;
}
