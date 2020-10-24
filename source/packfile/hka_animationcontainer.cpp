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

#include "hklib/hka_animation.hpp"
#include "hklib/hka_animationbinding.hpp"
#include "hklib/hka_boneattachment.hpp"
#include "hklib/hka_meshbinding.hpp"
#include "hklib/hka_skeleton.hpp"
#include "internal/hka_animationcontainer.hpp"
#include "base.hpp"

namespace {

enum PointerType {
  PT_SKELETONS,
  PT_ANIMATIONS,
  PT_BINDINGS,
  PT_ATTACHMENTS,
  PT_SKINS,
};

struct hkaAnimationContainerSaver {
  es::string_view main;
  const hkaAnimationContainerInternalInterface *interface;
  const size_t *offsets;

  void Save(BinWritterRef wr, hkFixups &fixups) {
    const size_t sBegin = wr.Tell();
    auto &locals = fixups.locals;
    wr.WriteContainer(main);

    const size_t ptrSize = (offsets[1] - offsets[0]) / 2;

    if (interface->GetNumSkeletons()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + offsets[PT_SKELETONS], wr.Tell());

      for (auto s : interface->Skeletons()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(ptrSize);
      }
    }

    if (interface->GetNumAnimations()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + offsets[PT_ANIMATIONS], wr.Tell());

      for (auto s : interface->Animations()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(ptrSize);
      }
    }

    if (interface->GetNumBindings()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + offsets[PT_BINDINGS], wr.Tell());

      for (auto s : interface->Bindings()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(ptrSize);
      }
    }

    if (interface->GetNumAttachments()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + offsets[PT_ATTACHMENTS], wr.Tell());

      for (auto s : interface->Attachments()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(ptrSize);
      }
    }

    if (interface->GetNumSkins()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + offsets[PT_SKINS], wr.Tell());

      for (auto s : interface->MeshBinds()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(ptrSize);
      }
    }
  }
};

template <class C>
struct hkaAnimationContainer_t : hkaAnimationContainerInternalInterface {
  uni::Element<C> data;
  std::unique_ptr<hkaAnimationContainerSaver> saver;

  void SetDataPointer(void *ptr) override {
    data = {static_cast<C *>(ptr), false};
  }
  const void *GetPointer() const override { return data.get(); }

  size_t GetNumSkeletons() const override { return data->GetNumSkeletons(0); };
  const hkaSkeleton *GetSkeleton(size_t id) const override {
    return data->GetSkeleton(header, id);
  };
  size_t GetNumAnimations() const override {
    return data->GetNumAnimations(0);
  };
  const hkaAnimation *GetAnimation(size_t id) const override {
    return data->GetAnimation(header, id);
  };
  size_t GetNumBindings() const override { return data->GetNumBindings(0); };
  const hkaAnimationBinding *GetBinding(size_t id) const override {
    return data->GetBinding(header, id);
  };
  size_t GetNumAttachments() const override {
    return data->GetNumAttachments(0);
  };
  const hkaBoneAttachment *GetAttachment(size_t id) const override {
    return data->GetAttachment(header, id);
  };
  size_t GetNumSkins() const override { return data->GetNumSkins(0); };
  const hkaMeshBinding *GetSkin(size_t id) const override {
    return data->GetSkin(header, id);
  };
  void SwapEndian() override { data->SwapEndian(); }

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(C, Skeletons), offsetof(C, Animations),
        offsetof(C, Bindings),  offsetof(C, Attachments),
        offsetof(C, Skins),
    };

    return offsets;
  }

  void Reflect(IhkVirtualClass *interface) override {
    data = std::move(decltype(data){new C()});
    saver = std::move(decltype(saver){new hkaAnimationContainerSaver()});
    saver->main = {reinterpret_cast<const char *>(data.get()), sizeof(C)};
    saver->interface =
        dynamic_cast<hkaAnimationContainerInternalInterface *>(interface);
    saver->offsets = GetPtrOffsets();

    if (!saver->interface) {
      throw std::runtime_error("Incorrect interface class!");
    }

    data->GetNumSkeletons(0) = saver->interface->GetNumSkeletons();
    data->GetNumAnimations(0) = saver->interface->GetNumAnimations();
    data->GetNumBindings(0) = saver->interface->GetNumBindings();
    data->GetNumAttachments(0) = saver->interface->GetNumAttachments();
    data->GetNumSkins(0) = saver->interface->GetNumSkins();
  }

  void Save(BinWritterRef wr, hkFixups &fixups) const override {
    saver->Save(wr, fixups);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaAnimationContainer_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;

  GetNum(Skeletons);
  GetNum(Animations);
  GetNum(Bindings);
  GetNum(Attachments);
  GetNum(Skins);

  const hkaSkeleton *GetSkeleton(IhkPackFile *header, size_t id) const {
    return dynamic_cast<const hkaSkeleton *>(
        header->GetClass(this->Skeletons[id]));
  };
  const hkaAnimation *GetAnimation(IhkPackFile *header, size_t id) const {
    return dynamic_cast<const hkaAnimation *>(
        header->GetClass(this->Animations[id]));
  };
  const hkaAnimationBinding *GetBinding(IhkPackFile *header, size_t id) const {
    return dynamic_cast<const hkaAnimationBinding *>(
        header->GetClass(this->Bindings[id]));
  };
  const hkaBoneAttachment *GetAttachment(IhkPackFile *header, size_t id) const {
    return dynamic_cast<const hkaBoneAttachment *>(
        header->GetClass(this->Attachments[id]));
  };
  const hkaMeshBinding *GetSkin(IhkPackFile *header, size_t id) const {
    return dynamic_cast<const hkaMeshBinding *>(
        header->GetClass(this->Skins[id]));
  };

  void SwapEndian() {
    FByteswapper(GetNumSkeletons(0));
    FByteswapper(GetNumAnimations(0));
    FByteswapper(GetNumBindings(0));
    FByteswapper(GetNumAttachments(0));
    FByteswapper(GetNumSkins(0));
  }
};

template <template <class C> class _ipointer>
struct hkaAnimationContainer500_t_sharedData {
  _ipointer<_ipointer<hkaSkeleton>> Skeletons;
  uint32 numSkeletons;
  _ipointer<_ipointer<hkaAnimation>> Animations;
  uint32 numAnimations;
  _ipointer<_ipointer<hkaAnimationBinding>> Bindings;
  uint32 numBindings;
  _ipointer<_ipointer<hkaBoneAttachment>> Attachments;
  uint32 numAttachemts;
  _ipointer<_ipointer<hkaMeshBinding>> Skins;
  uint32 numSkins;
};

template <template <class C> class _ipointer>
struct hkaAnimationContainer700_t_sharedData : hkReferenceObject<_ipointer> {
  hkArray<_ipointer<hkaSkeleton>, _ipointer> Skeletons;
  hkArray<_ipointer<hkaAnimation>, _ipointer> Animations;
  hkArray<_ipointer<hkaAnimationBinding>, _ipointer> Bindings;
  hkArray<_ipointer<hkaBoneAttachment>, _ipointer> Attachments;
  hkArray<_ipointer<hkaMeshBinding>, _ipointer> Skins;
};

template <template <class C> class _ipointer>
struct hkaAnimationContainer2016_t_sharedData
    : hkReferenceObject2016<_ipointer> {
  hkArray<_ipointer<hkaSkeleton>, _ipointer> Skeletons;
  hkArray<_ipointer<hkaAnimation>, _ipointer> Animations;
  hkArray<_ipointer<hkaAnimationBinding>, _ipointer> Bindings;
  hkArray<_ipointer<hkaBoneAttachment>, _ipointer> Attachments;
  hkArray<_ipointer<hkaMeshBinding>, _ipointer> Skins;
};

template <template <class C> class _ipointer>
using hkaAnimationContainer500 =
    hkaAnimationContainer_t_shared<_ipointer,
                                   hkaAnimationContainer500_t_sharedData>;
template <template <class C> class _ipointer>
using hkaAnimationContainer700 =
    hkaAnimationContainer_t_shared<_ipointer,
                                   hkaAnimationContainer700_t_sharedData>;

template <template <class C> class _ipointer>
using hkaAnimationContainer2016 =
    hkaAnimationContainer_t_shared<_ipointer,
                                   hkaAnimationContainer2016_t_sharedData>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkaAnimationContainerTier : hkaAnimationContainer700<_ipointer> {};

hkClassTier0Alias(hkaAnimationContainer, 500, 500);
hkClassTier0Alias(hkaAnimationContainer, 500, 510);
hkClassTier0Alias(hkaAnimationContainer, 500, 550);
hkClassTier0Alias(hkaAnimationContainer, 500, 600);
hkClassTier0Alias(hkaAnimationContainer, 500, 610);
hkClassTier0Alias(hkaAnimationContainer, 500, 650);
hkClassTier0Alias(hkaAnimationContainer, 500, 660);
hkClassTier0Alias(hkaAnimationContainer, 2016, 2016);
hkClassTier0Alias(hkaAnimationContainer, 2016, 2017);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterFullClass(hkaAnimationContainer)};
} // namespace

hkVirtualClass *hkaAnimationContainerInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
