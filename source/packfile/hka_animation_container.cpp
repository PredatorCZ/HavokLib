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

#include "base.hpp"
#include "hklib/hka_animation.hpp"
#include "hklib/hka_animationbinding.hpp"
#include "hklib/hka_boneattachment.hpp"
#include "hklib/hka_meshbinding.hpp"
#include "hklib/hka_skeleton.hpp"
#include "internal/hka_animationcontainer.hpp"

#include "hka_animation_container.inl"

struct hkaAnimationContainerSaver {
  const hkaAnimationContainerInternalInterface *in;
  const clgen::hkaAnimationContainer::Interface *out;

  void Save(BinWritterRef wr, hkFixups &fixups) {
    const size_t sBegin = wr.Tell();
    auto &locals = fixups.locals;
    auto &lay = *out->layout;
    using mm = clgen::hkaAnimationContainer::Members;

    wr.WriteBuffer(out->data, lay.totalSize);

    if (in->GetNumSkeletons()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + out->m(mm::skeletons), wr.Tell());

      for (auto s : in->Skeletons()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(lay.ptrSize);
      }
    }

    if (in->GetNumAnimations()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + out->m(mm::animations), wr.Tell());

      for (auto s : in->Animations()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(lay.ptrSize);
      }
    }

    if (in->GetNumBindings()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + out->m(mm::bindings), wr.Tell());

      for (auto s : in->Bindings()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(lay.ptrSize);
      }
    }

    if (in->GetNumAttachments()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + out->m(mm::attachments), wr.Tell());

      for (auto s : in->Attachments()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(lay.ptrSize);
      }
    }

    if (in->GetNumSkins()) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + out->m(mm::skins), wr.Tell());

      for (auto s : in->MeshBinds()) {
        locals.emplace_back(wr.Tell(), dynamic_cast<const hkVirtualClass *>(s));
        wr.Skip(lay.ptrSize);
      }
    }
  }
};

struct hkaAnimationContainerMidInterface
    : hkaAnimationContainerInternalInterface {
  clgen::hkaAnimationContainer::Interface interface;
  std::unique_ptr<hkaAnimationContainerSaver> saver;

  hkaAnimationContainerMidInterface(clgen::LayoutLookup rules, char *data)
      : interface {
    data, rules
  } {
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  size_t GetNumSkeletons() const override { return interface.NumSkeletons(); };
  const hkaSkeleton *GetSkeleton(size_t id) const override {
    auto item = interface.Skeletons().Next(id);
    return dynamic_cast<const hkaSkeleton *>(header->GetClass(item.data));
  };
  size_t GetNumAnimations() const override {
    return interface.NumAnimations();
  };
  const hkaAnimation *GetAnimation(size_t id) const override {
    auto item = interface.Animations().Next(id);
    return dynamic_cast<const hkaAnimation *>(header->GetClass(item.data));
  };
  size_t GetNumBindings() const override { return interface.NumBindings(); };
  const hkaAnimationBinding *GetBinding(size_t id) const override {
    auto item = interface.Bindings().Next(id);
    return dynamic_cast<const hkaAnimationBinding *>(
        header->GetClass(item.data));
  };
  size_t GetNumAttachments() const override {
    return interface.NumAttachments();
  };
  const hkaBoneAttachment *GetAttachment(size_t id) const override {
    auto item = interface.Attachments().Next(id);
    return dynamic_cast<const hkaBoneAttachment *>(header->GetClass(item.data));
  };
  size_t GetNumSkins() const override { return interface.NumSkins(); };
  const hkaMeshBinding *GetSkin(size_t id) const override {
    auto item = interface.Skins().Next(id);
    return dynamic_cast<const hkaMeshBinding *>(header->GetClass(item.data));
  };
  void SwapEndian() override { clgen::EndianSwap(interface); }

  void Reflect(IhkVirtualClass *other) override {
    interface.data = static_cast<char *>(malloc(interface.layout->totalSize));
    saver = std::make_unique<hkaAnimationContainerSaver>();
    saver->in = dynamic_cast<hkaAnimationContainerInternalInterface *>(other);
    saver->out = &interface;

    if (!saver->in) {
      throw std::runtime_error("Incorrect interface class!");
    }

    interface.NumSkeletons(saver->in->GetNumSkeletons());
    interface.NumAnimations(saver->in->GetNumAnimations());
    interface.NumBindings(saver->in->GetNumBindings());
    interface.NumAttachments(saver->in->GetNumAttachments());
    interface.NumSkins(saver->in->GetNumSkins());
  }

  void Save(BinWritterRef wr, hkFixups &fixups) const override {
    saver->Save(wr, fixups);
  }

  ~hkaAnimationContainerMidInterface() {
    if (saver) {
      free(interface.data);
    }
  }
};

hkVirtualClass *hkaAnimationContainerInternalInterface::Create(CRule rule) {
  return new hkaAnimationContainerMidInterface{
      clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};
}
