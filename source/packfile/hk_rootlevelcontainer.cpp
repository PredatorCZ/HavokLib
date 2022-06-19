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

#include "internal/hk_rootlevelcontainer.hpp"
#include "base.hpp"
#include "hk_rootlevelcontainer.inl"

struct hkRootLevelContainerSaver {
  const hkRootLevelContainerInternalInterface *in;
  const clgen::hkRootLevelContainer::Interface *out;

  void Save(BinWritterRef wr, hkFixups &fixups) {
    const size_t sBegin = wr.Tell();
    auto &locals = fixups.locals;
    auto &lay = *out->layout;

    wr.WriteBuffer(out->data, lay.totalSize);

    if (in->Size()) {
      wr.ApplyPadding();
      locals.emplace_back(sBegin, wr.Tell());
      size_t curFixup = locals.size();
      const auto varType =
          clgen::GetLayout(clgen::hkNamedVariant::LAYOUTS, out->lookup);
      using vm = clgen::hkNamedVariant::Members;

      for (auto &v : *in) {
        const size_t varBegin = wr.Tell();
        wr.Skip(varType->totalSize);

        for (auto v : varType->vtable) {
          locals.emplace_back(varBegin + (v * varType->ptrSize));
        }
      }

      for (auto &i : *in) {
        wr.ApplyPadding(8);
        locals[curFixup + vm::name].destination = wr.Tell();
        wr.WriteBuffer(i.name.data(), i.name.size() + 1);
        wr.ApplyPadding(8);
        locals[curFixup + vm::className].destination = wr.Tell();
        wr.WriteBuffer(i.className.data(), i.className.size() + 1);
        locals[curFixup + vm::variant].destClass = i.pointer;
      }
    }
  }
};

struct hkRootLevelContainerMidInterface
    : hkRootLevelContainerInternalInterface {
  clgen::hkRootLevelContainer::Interface interface;
  std::unique_ptr<hkRootLevelContainerSaver> saver;

  hkRootLevelContainerMidInterface(clgen::LayoutLookup rules, char *data)
      : interface {
    data, rules
  } {
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  size_t Size() const override { return interface.NumVariants(); }
  const hkNamedVariant At(size_t id) const override {
    auto item = interface.Variants().Next(id);
    if (interface.LayoutVersion() >= HK700) {
      return {item.Name(), item.ClassName(),
              header->GetClass(item.VariantHK700())};
    }

    return {item.Name(), item.ClassName(),
            header->GetClass(item.Variant().Object())};
  }

  void Reflect(const IhkVirtualClass *other) override {
    interface.data = static_cast<char *>(malloc(interface.layout->totalSize));
    saver = std::make_unique<hkRootLevelContainerSaver>();
    saver->in = static_cast<const hkRootLevelContainerInternalInterface *>(
        checked_deref_cast<const hkRootLevelContainer>(other));
    saver->out = &interface;
    interface.NumVariants(saver->in->Size());
  }

  void Save(BinWritterRef wr, hkFixups &fixups) const override {
    saver->Save(wr, fixups);
  }

  void SwapEndian() override { clgen::EndianSwap(interface); }

  ~hkRootLevelContainerMidInterface() {
    if (saver) {
      free(interface.data);
    }
  }
};

hkVirtualClass *hkRootLevelContainerInternalInterface::Create(CRule rule) {
  return new hkRootLevelContainerMidInterface{
      clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};
}
