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

#include "internal/hkx_environment.hpp"
#include "base.hpp"

namespace {

struct hkxEnvironmentSaver {
  es::string_view main;
  const hkxEnvironmentInternalInterface *interface;

  std::unique_ptr<char> variable;
  es::string_view variableData;
  const size_t *variableOffsets;
  size_t arrayOffset;

  void Save(BinWritterRef wr, hkFixups &fixups) {
    const size_t sBegin = wr.Tell();
    auto &locals = fixups.locals;
    wr.WriteContainer(main);

    if (interface->Size()) {
      wr.ApplyPadding();
      locals.emplace_back(sBegin + arrayOffset, wr.Tell());
      size_t curFixup = locals.size();

      for (auto &v : *interface) {
        const size_t varBegin = wr.Tell();
        wr.WriteContainer(variableData);

        for (size_t i = 0; i < 2; i++) {
          locals.emplace_back(varBegin + variableOffsets[i]);
        }
      }

      for (auto &i : *interface) {
        wr.ApplyPadding(8);
        locals[curFixup++].destination = wr.Tell();
        wr.WriteBuffer(i.name.data(), i.name.size() + 1);
        wr.ApplyPadding(8);
        locals[curFixup++].destination = wr.Tell();
        wr.WriteBuffer(i.value.data(), i.value.size() + 1);
      }
    }
  }
};

template <template <class C> class _ipointer> struct hkxEnvironmentVariable_t {
  _ipointer<const char> name;
  _ipointer<const char> value;

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkxEnvironmentVariable_t, name),
        offsetof(hkxEnvironmentVariable_t, value),
    };

    return offsets;
  }
};

template <class C> struct hkxEnvironment_t : hkxEnvironmentInternalInterface {
  uni::Element<C> data;
  std::unique_ptr<hkxEnvironmentSaver> saver;

  void SetDataPointer(void *ptr) override {
    data = {static_cast<C *>(ptr), false};
  }
  const void *GetPointer() const override { return data.get(); }

  void SwapEndian() override { data->SwapEndian(); }
  size_t Size() const override { return data->GetNumVars(); }
  const hkxEnvironmentVariable At(size_t id) const override {
    return data->GetVar(id);
  }

  void Reflect(IhkVirtualClass *interface) override {
    data = std::move(decltype(data){new C()});
    saver = std::move(decltype(saver){new hkxEnvironmentSaver()});
    saver->main = {reinterpret_cast<const char *>(data.get()), sizeof(C)};
    saver->interface =
        dynamic_cast<hkxEnvironmentInternalInterface *>(interface);
    saver->arrayOffset = offsetof(C, variables);

    if (!saver->interface) {
      throw std::runtime_error("Incorrect interface class!");
    }

    saver->variableOffsets = C::var_type::GetPtrOffsets();
    void *newVar = new typename C::var_type();
    saver->variable =
        std::move(decltype(saver->variable){static_cast<char *>(newVar)});
    saver->variableData = {static_cast<const char *>(saver->variable.get()),
                           sizeof(typename C::var_type)};

    data->variables.count = saver->interface->Size();
  }

  void Save(BinWritterRef wr, hkFixups &fixups) const override {
    saver->Save(wr, fixups);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkxEnvironment_t_shared : _parent<_ipointer> {
  typedef hkxEnvironmentVariable_t<_ipointer> var_type;
  size_t GetNumVars() const { return this->variables.count; }
  hkxEnvironmentVariable GetVar(size_t id) const {
    const var_type &tmpVal = this->variables[id];
    return {static_cast<const char *>(tmpVal.name),
            static_cast<const char *>(tmpVal.value)};
  }
  void SwapEndian() { FByteswapper(this->variables.count); }
};

template <template <class C> class _ipointer>
struct hkxEnvironment550_t_sharedData {
  hkArray<hkxEnvironmentVariable_t<_ipointer>, _ipointer> variables;
};

template <template <class C> class _ipointer>
using hkxEnvironment550 =
    hkxEnvironment_t_shared<_ipointer, hkxEnvironment550_t_sharedData>;

template <template <class C> class _ipointer>
struct hkxEnvironment710_t_sharedData : hkReferenceObject<_ipointer> {
  hkArray<hkxEnvironmentVariable_t<_ipointer>, _ipointer> variables;
};

template <template <class C> class _ipointer>
using hkxEnvironment710 =
    hkxEnvironment_t_shared<_ipointer, hkxEnvironment710_t_sharedData>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkxEnvironmentTier : hkxEnvironment710<_ipointer> {};

hkClassTier0Alias(hkxEnvironment, 550, 500);
hkClassTier0Alias(hkxEnvironment, 550, 510);
hkClassTier0Alias(hkxEnvironment, 550, 550);
hkClassTier0Alias(hkxEnvironment, 550, 600);
hkClassTier0Alias(hkxEnvironment, 550, 610);
hkClassTier0Alias(hkxEnvironment, 550, 650);
hkClassTier0Alias(hkxEnvironment, 550, 660);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterVersionedClasses(hkxEnvironment, 500, 510, 550, 600, 610, 650,
                               660, 700, 710, 2010_1, 2010_2, 2011_1, 2011_2,
                               2011_3, 2012_1, 2012_2, 2013, 2014)};
} // namespace

hkVirtualClass *hkxEnvironmentInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
