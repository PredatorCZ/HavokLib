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
#include "hk_base.hpp"
#include "hk_settings.hpp"

struct hkRootLevelContainer;

struct HK_EXTERN IhkPackFile {
  typedef std::unique_ptr<IhkVirtualClass> VirtualClass;
  typedef std::vector<VirtualClass> VirtualClasses;
  typedef std::vector<IhkVirtualClass *> VirtualClassesRef;

  virtual VirtualClasses &GetAllClasses() = 0;
  virtual hkToolset GetToolset() const = 0;
  virtual ~IhkPackFile(){};

  VirtualClassesRef GetClasses(es::string_view hkClassName) {
    return GetClasses(JenkinsHash(hkClassName));
  }

  VirtualClassesRef GetClasses(JenHash hash);
  hkRootLevelContainer *GetRootLevelContainer();
  const IhkVirtualClass *GetClass(const void *ptr);

  static IhkPackFile *Create(const std::string &fileName,
                             bool suppressErrors = false);
  static IhkPackFile *Create(es::string_view fileName,
                             bool suppressErrors = false);

  int ToXML(es::string_view fileName, hkToolset toolset);
  int ToXML(const std::string &fileName, hkToolset toolset);

  pugi::xml_document ToXML(hkToolset toolset);

  // rule must be as hex, ie. 0x4101
  int ToPackFile(es::string_view fileName, hkToolset toolset, uint32 rule);
  int ToPackFile(const std::string &fileName, hkToolset toolset, uint32 rule);
};
