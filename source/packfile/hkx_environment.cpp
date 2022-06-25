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

#include "internal/hkx_environment.hpp"
#include "base.hpp"
#include "hkx_environment.inl"

struct hkxEnvironmentSaver {
  const hkxEnvironmentInternalInterface *in;
  const clgen::hkxEnvironment::Interface *out;

  void Save(BinWritterRef wr, hkFixups &fixups) {
    const size_t sBegin = wr.Tell();
    auto &locals = fixups.locals;
    auto &lay = *out->layout;

    wr.WriteBuffer(out->data, lay.totalSize);
    using mm = clgen::hkxEnvironment::Members;

    if (in->Size()) {
      wr.ApplyPadding();
      locals.emplace_back(sBegin + out->m(mm::variables), wr.Tell());
      size_t curFixup = locals.size();
      const auto varType =
          clgen::GetLayout(clgen::hkxEnvironmentVariable::LAYOUTS, out->lookup);
      using vm = clgen::hkxEnvironmentVariable::Members;

      for ([[maybe_unused]] auto &v : *in) {
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
        locals[curFixup + vm::value].destination = wr.Tell();
        wr.WriteBuffer(i.value.data(), i.value.size() + 1);
      }
    }
  }
};

struct hkxEnvironmentMidInterface : hkxEnvironmentInternalInterface {
  clgen::hkxEnvironment::Interface interface;
  std::unique_ptr<hkxEnvironmentSaver> saver;

  hkxEnvironmentMidInterface(clgen::LayoutLookup rules, char *data)
      : interface {
    data, rules
  } {
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  void SwapEndian() override { clgen::EndianSwap(interface); }

  size_t Size() const override { return interface.NumVariables(); }

  const hkxEnvironmentVariable At(size_t id) const override {
    auto item = interface.Variables().Next(id);
    return {item.Name(), item.Value()};
  }

  void Reflect(const IhkVirtualClass *other) override {
    interface.data = static_cast<char *>(malloc(interface.layout->totalSize));
    saver = std::make_unique<hkxEnvironmentSaver>();
    saver->in = static_cast<const hkxEnvironmentInternalInterface *>(
        checked_deref_cast<const hkxEnvironment>(other));
    saver->out = &interface;
    interface.NumVariables(saver->in->Size());
  }

  void Save(BinWritterRef wr, hkFixups &fixups) const override {
    saver->Save(wr, fixups);
  }

  ~hkxEnvironmentMidInterface() {
    if (saver) {
      free(interface.data);
    }
  }
};

IhkVirtualClass *hkxEnvironmentInternalInterface::Create(CRule rule) {
  return new hkxEnvironmentMidInterface{
      clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};
}
