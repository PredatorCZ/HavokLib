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

#include "base.hpp"
#include "internal/hkx_attributeholder.hpp"

#include "hkx_attribute_holder.inl"

struct hkxAttributeGroupMidInterface : hkxAttributeGroup {
  clgen::hkxAttributeGroup::Interface interface;
  IhkPackFile *header;

  hkxAttributeGroupMidInterface(clgen::LayoutLookup rules, char *data)
      : interface{data, rules} {}

  hkxAttributeGroupMidInterface(
      const clgen::hkxAttributeGroup::Interface &interface, IhkPackFile *header)
      : interface{interface}, header{header} {}

  void SwapEndian() { clgen::EndianSwap(interface); }

  size_t Size() const override { return interface.NumAttributes(); }

  const hkxAttribute At(size_t id) const override {
    auto item = interface.Attributes().Next(id);
    return {item.Name(), header->GetClass(item.Value().Object())};
  }

  std::string_view Name() const override { return safe_sw(interface.Name()); }
};

struct hkxAttributeHolderMidInterface
    : virtual hkxAttributeHolderInternalInterface {
  virtual clgen::hkxAttributeHolder::Interface AttribHolder() const = 0;

  void SwapEndian() override {
    auto holder = AttribHolder();
    clgen::EndianSwap(holder);
  }

  size_t Size() const override { return AttribHolder().NumAttributeGroups(); }

  uni::Element<const hkxAttributeGroup> At(size_t id) const override {
    return {new hkxAttributeGroupMidInterface{
                AttribHolder().AttributeGroups().Next(id), header},
            true};
  }
};
