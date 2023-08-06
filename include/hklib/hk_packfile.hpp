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
#pragma once
#include "hk_base.hpp"
#include "hk_settings.hpp"
#include "spike/io/bincore_fwd.hpp"
#include <vector>

struct hkRootLevelContainer;

struct HK_EXTERN IhkPackFile {
  using VirtualClass = std::unique_ptr<IhkVirtualClass>;
  using VirtualClasses = std::vector<VirtualClass>;
  using VirtualClassesRef = std::vector<IhkVirtualClass *>;
  using Ptr = std::unique_ptr<IhkPackFile>;

  virtual VirtualClasses &GetAllClasses() = 0;
  virtual hkToolset GetToolset() const = 0;
  virtual ~IhkPackFile() = default;

  VirtualClassesRef GetClasses(JenHash hash);
  const hkRootLevelContainer *GetRootLevelContainer();
  const IhkVirtualClass *GetClass(const void *ptr);

  static Ptr Create(const std::string &fileName);
  static Ptr Create(BinReaderRef_e rd);

  void ToXML(const std::string &fileName, hkToolset toolset);
  void ToXML(pugi::xml_node node, hkToolset toolset);

  // rule must be as hex, ie. 0x4101
  void ToPackFile(const std::string &fileName, hkToolset toolset, uint32 rule);
};
