/*  Havok Format Library
    Copyright(C) 2016-2022 Lukas Cone

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

#include "datas/except.hpp"
#include "datas/master_printer.hpp"
#include "datas/pugiex.hpp"
#include "datas/vectors_string.hpp"
#include "hklib/hk_packfile.hpp"
#include "toolset.hpp"
#include <cinttypes>

static const char *_hkName = "name", *_hkObject = "hkobject",
                  *_hkClass = "class", *_hkNumElements = "numelements",
                  *_hkParam = "hkparam";

static void PointerToString(const void *ptr, std::string &str) {
  if (!ptr) {
    str.append("null");
    return;
  }

  str.append("0x");
  char buffer[20];
  snprintf(buffer, 20, "%" PRIXPTR, reinterpret_cast<uintptr_t>(ptr));
  str.append(buffer);
}

template <class C> static void ExportReflectedClass(C &input, XMLnode &parent) {
  ReflectorWrap<const C> refl(input);

  for (size_t t = 0; t < refl.GetNumReflectedValues(); t++) {
    Reflector::KVPair pair = refl.GetReflectedPair(t);
    XMLnode nameNode = parent.append_child(_hkParam);
    nameNode.append_attribute(_hkName).set_value(pair.name.data());
    nameNode.append_buffer(pair.value.c_str(), pair.value.size());
  }
}

std::string to_string(const hkQTransform &tm) {
  std::string retVal;
  retVal.push_back('(');
  retVal
      .append(std::to_string(reinterpret_cast<const Vector &>(tm.translation)))
      .append(")(")
      .append(std::to_string(tm.rotation))
      .append(")(")
      .append(std::to_string(reinterpret_cast<const Vector &>(tm.scale)))
      .push_back(')');
  return retVal;
}

#include "hk_rootlevelcontainer.inl"
#include "hka_animation.inl"
#include "hka_animationbinding.inl"
#include "hka_animationcontainer.inl"
#include "hka_annotationtrack.inl"
#include "hka_interleavedanimation.inl"
#include "hka_skeleton.inl"
#include "hkx_environment.inl"

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
    {HK2013,
     xmlToolsetProp{xmlToolsetProp::TopLevelObject, 9, "hk_2013.1.0-r1"}},
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
