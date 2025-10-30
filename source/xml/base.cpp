/*  Havok Format Library
    Copyright(C) 2016-2023 Lukas Cone

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

#include "base.hpp"
#include "hklib/hk_packfile.hpp"
#include "hklib/hk_rootlevelcontainer.hpp"
#include "internal/hk_internal_api.hpp"

const std::map<hkToolset, xmlToolsetProp> xmlToolsetProps = {
    {HK500, xmlToolsetProp{{}, 5, "Havok-5.0.0-r1"}},
    {HK510, xmlToolsetProp{{}, 5, "Havok-5.1.0-r1"}},
    {HK550, xmlToolsetProp{{}, 5, "Havok-5.5.0-r1"}},
    {HK600,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 6, "Havok-6.0.0-r1"}},
    {HK610,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 6, "Havok-6.1.0-r1"}},
    {HK650,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 6, "Havok-6.5.0-r1"}},
    {HK660,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 7, "Havok-6.6.0-r1"}},
    {HK700,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 7, "Havok-7.0.0-r1"}},
    {HK710,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 7, "Havok-7.1.0-r1"}},
    {HK2010_1,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 8, "hk_2010.1.0-r1"}},
    {HK2010_2,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 8, "hk_2010.2.0-r1"}},
    {HK2011_1,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2011.1.0-r1"}},
    {HK2011_2,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2011.2.0-r1"}},
    {HK2011_3,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2011.3.0-r1"}},
    {HK2012_1,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2012.1.0-r1"}},
    {HK2012_2,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2012.2.0-r1"}},
    {HK2013_1,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2013.1.0-r1"}},
    {HK2013_2,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2013.2.0-r1"}},
    {HK2013_3,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2013.3.0-r1"}},
    {HK2014, xmlToolsetProp{{xmlToolsetProp::TopLevelObject,
                             xmlToolsetProp::MaxPredicate},
                            11,
                            "hk_2014.1.0-r1"}},
};

void IhkPackFile::ToXML(pugi::xml_node nde, hkToolset toolsetVersion) {
  pugi::xml_node master = nde.append_child("hkpackfile");

  const xmlToolsetProp &propRef = xmlToolsetProps.at(toolsetVersion);

  master.append_attribute("classversion").set_value(propRef.version);
  master.append_attribute("contentsversion").set_value(propRef.name);

  if (propRef.flags[xmlToolsetProp::MaxPredicate]) {
    master.append_attribute("maxpredicate").set_value(21);
    master.append_attribute("predicates");
  }

  pugi::xml_node dataSection = master.append_child("hksection");
  dataSection.append_attribute(_hkName).set_value("__data__");

  const VirtualClasses &allClasses = GetAllClasses();

  for (auto &c : allClasses) {
    auto cls = checked_deref_cast<const hkVirtualClass>(c.get());
    pugi::xml_node classNode = dataSection.append_child(_hkObject);
    pugi::xml_attribute addrAttr = classNode.append_attribute(_hkName);

    std::string _buffer;
    PointerToString(c->GetPointer(), _buffer);

    addrAttr.set_value(_buffer.c_str());
    classNode.append_attribute(_hkClass).set_value(
        cls->GetClassName(toolsetVersion).data());

    if (cls->HasHash(hkRootLevelContainer::GetHash()) &&
        propRef.flags[xmlToolsetProp::TopLevelObject])
      master.append_attribute("toplevelobject").set_value(addrAttr.as_string());

    cls->ToXML({&classNode, toolsetVersion});
  }
}

void IhkPackFile::ToXML(const std::string &fileName, hkToolset toolset) {
  pugi::xml_document doc;
  ToXML(doc, toolset);
  XMLToFile(fileName, doc);
}
