/*  Havok Format Library
    Copyright(C) 2016-2025 Lukas Cone

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
#include "hk_internal_api.hpp"
#include "hklib/hkx_scene.hpp"
#include "hkx_attributeholder.hpp"

struct hkxNodeInternalInterface : hkxNode,
                                  virtual hkxAttributeHolderInternalInterface {
  operator hkVirtualClass const *() const override { return this; }
  operator hkxNode const *() const override { return this; }
  void ToXML(XMLHandle hdl) const override;
};

struct hkxNodeSelectionSetInternalInterface
    : hkxNodeSelectionSet,
      virtual hkxAttributeHolderInternalInterface {
  operator hkVirtualClass const *() const override { return this; }
  operator hkxNodeSelectionSet const *() const override { return this; }
  void ToXML(XMLHandle hdl) const override;
};

struct hkxLightInternalInterface : hkxLight, hkVirtualClass {
  operator hkVirtualClass const *() const override { return this; }
  operator hkxLight const *() const override { return this; }
  void ToXML(XMLHandle hdl) const override;
};

struct hkxTextureInplaceInternalInterface : hkxTextureInplace, hkVirtualClass {
  operator hkVirtualClass const *() const override { return this; }
  operator hkxTextureInplace const *() const override { return this; }
  void ToXML(XMLHandle hdl) const override;
};

struct hkxTextureFileInternalInterface : hkxTextureFile, hkVirtualClass {
  operator hkVirtualClass const *() const override { return this; }
  operator hkxTextureFile const *() const override { return this; }
  void ToXML(XMLHandle hdl) const override;
};

struct hkxSkinBindingInternalInterface : hkxSkinBinding, hkVirtualClass {
  operator hkVirtualClass const *() const override { return this; }
  operator hkxSkinBinding const *() const override { return this; }
  void ToXML(XMLHandle hdl) const override;
};

struct hkxCameraInternalInterface : IhkxVirtualClass, hkVirtualClass {
  operator hkVirtualClass const *() const override { return this; }
  void ToXML(XMLHandle hdl) const override;
};

struct hkxSceneInternalInterface : hkxScene, hkVirtualClass {
  operator hkxScene const *() const override { return this; }
  operator hkVirtualClass const *() const override { return this; }
  void ToXML(XMLHandle hdl) const override;
};
