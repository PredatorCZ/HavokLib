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

#include "internal/hk_rootlevelcontainer.hpp"
#include "base.hpp"

namespace {

struct hkRootLevelContainerSaver {
  es::string_view main;
  hkRootLevelContainerInternalInterface *interface;

  std::unique_ptr<char> variant;
  es::string_view variantData;
  const size_t *variantOffsets;

  void Save(BinWritterRef wr, hkFixups &fixups) {
    const size_t sBegin = wr.Tell();
    auto &locals = fixups.locals;
    wr.WriteContainer(main);

    if (interface->Size()) {
      wr.ApplyPadding();
      locals.emplace_back(sBegin, wr.Tell());
      size_t curFixup = locals.size();

      for (auto &v : *interface) {
        const size_t varBegin = wr.Tell();
        wr.WriteContainer(variantData);

        for (size_t i = 0; i < 3; i++) {
          locals.emplace_back(varBegin + variantOffsets[i]);
        }
      }

      for (auto &i : *interface) {
        wr.ApplyPadding(8);
        locals[curFixup++].destination = wr.Tell();
        wr.WriteBuffer(i.name.data(), i.name.size() + 1);
        wr.ApplyPadding(8);
        locals[curFixup++].destination = wr.Tell();
        wr.WriteBuffer(i.className.data(), i.className.size() + 1);
        locals[curFixup++].destClass = i.pointer;
      }
    }
  }
};

template <class C>
struct hkRootLevelContainer_t : hkRootLevelContainerInternalInterface {
  uni::Element<C> data;
  std::unique_ptr<hkRootLevelContainerSaver> saver;

  void SetDataPointer(void *ptr) override {
    data = {static_cast<C *>(ptr), false};
  }
  const void *GetPointer() const override { return data.get(); }

  void SwapEndian() override { data->SwapEndian(); }
  size_t Size() const override { return data->GetNumVariants(0); }
  const hkNamedVariant At(size_t id) const override {
    return data->GetVariant(header, id);
  }

  void Reflect(IhkVirtualClass *interface) override {
    data = std::move(decltype(data){new C()});
    saver = std::move(decltype(saver){new hkRootLevelContainerSaver()});
    saver->main = {reinterpret_cast<const char *>(data.get()), sizeof(C)};
    saver->interface =
        dynamic_cast<hkRootLevelContainerInternalInterface *>(interface);

    if (!saver->interface) {
      throw std::runtime_error("Incorrect interface class!");
    }

    saver->variantOffsets = C::parent_class::variantType::GetPtrOffsets();
    void *newVar = new typename C::parent_class::variantType();
    saver->variant =
        std::move(decltype(saver->variant){static_cast<char *>(newVar)});
    saver->variantData = {static_cast<const char *>(saver->variant.get()),
                          sizeof(typename C::parent_class::variantType)};

    data->GetNumVariants(0) = saver->interface->Size();
  }

  void Save(BinWritterRef wr, hkFixups &fixups) const override {
    saver->Save(wr, fixups);
  }
};

template <template <class C> class _ipointer> struct hkNamedVariant_t {
  _ipointer<char> name;
  _ipointer<char> classname;
  hkVariant<char, _ipointer> variant;
  const hkNamedVariant GetVariant(IhkPackFile *header) const {
    return {static_cast<const char *>(name),
            static_cast<const char *>(classname),
            header->GetClass(variant.object)};
  }

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkNamedVariant_t, name),
        offsetof(hkNamedVariant_t, classname),
        offsetof(hkNamedVariant_t, variant.object),
    };

    return offsets;
  }
};

template <template <class C> class _ipointer> struct hkNamedVariant2_t {
  _ipointer<char> name;
  _ipointer<char> classname;
  _ipointer<char> variant;
  const hkNamedVariant GetVariant(IhkPackFile *header) const {
    return {static_cast<const char *>(name),
            static_cast<const char *>(classname), header->GetClass(variant)};
  }

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkNamedVariant2_t, name),
        offsetof(hkNamedVariant2_t, classname),
        offsetof(hkNamedVariant2_t, variant),
    };

    return offsets;
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkRootLevelContainer_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;

  GetNum(Variants);

  const hkNamedVariant GetVariant(IhkPackFile *header, size_t id) const {
    return this->Variants[id].GetVariant(header);
  }

  void SwapEndian() { FByteswapper(GetNumVariants(0)); }
};

template <template <class C> class _ipointer>
struct hkRootLevelContainer660_t_sharedData {
  typedef hkNamedVariant_t<_ipointer> variantType;
  _ipointer<variantType> Variants;
  uint32 numVariants;
};

template <template <class C> class _ipointer>
using hkRootLevelContainer550 =
    hkRootLevelContainer_t_shared<_ipointer,
                                  hkRootLevelContainer660_t_sharedData>;

template <template <class C> class _ipointer>
struct hkRootLevelContainer700_t_sharedData {
  typedef hkNamedVariant2_t<_ipointer> variantType;
  hkArray<variantType, _ipointer> Variants;
};

template <template <class C> class _ipointer>
using hkRootLevelContainer700 =
    hkRootLevelContainer_t_shared<_ipointer,
                                  hkRootLevelContainer700_t_sharedData>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkRootLevelContainerTier : hkRootLevelContainer700<_ipointer> {};

hkClassTier0Alias(hkRootLevelContainer, 550, 500);
hkClassTier0Alias(hkRootLevelContainer, 550, 510);
hkClassTier0Alias(hkRootLevelContainer, 550, 550);
hkClassTier0Alias(hkRootLevelContainer, 550, 600);
hkClassTier0Alias(hkRootLevelContainer, 550, 610);
hkClassTier0Alias(hkRootLevelContainer, 550, 650);
hkClassTier0Alias(hkRootLevelContainer, 550, 660);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterFullClass(hkRootLevelContainer),
};

} // namespace

hkVirtualClass *hkRootLevelContainerInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
