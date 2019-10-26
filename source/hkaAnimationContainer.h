/*  Havok Format Library
    Copyright(C) 2016-2019 Lukas Cone

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
#include "hkInternalInterfaces.h"
#include "hkObjectBase.h"

template <class C>
struct hkaAnimationContainer_t : hkaAnimationContainerInternalInterface {
  C *Data;
  hkClassConstructor(hkaAnimationContainer_t<C>);
  const int GetNumSkeletons() const { return Data->GetNumSkeletons(); };
  const hkaSkeleton *GetSkeleton(int id) const {
    return Data->GetSkeleton(header, id);
  };
  const int GetNumAnimations() const { return Data->GetNumAnimations(); };
  const hkaAnimation *GetAnimation(int id) const {
    return Data->GetAnimation(header, id);
  };
  const int GetNumBindings() const { return Data->GetNumBindings(); };
  const hkaAnimationBinding *GetBinding(int id) const {
    return Data->GetBinding(header, id);
  };
  const int GetNumAttachments() const { return Data->GetNumAttachments(); };
  const hkaBoneAttachment *GetAttachment(int id) const {
    return Data->GetAttachment(header, id);
  };
  const int GetNumSkins() const { return Data->GetNumSkins(); };
  const hkaMeshBinding *GetSkin(int id) const {
    return Data->GetSkin(header, id);
  };
  void SwapEndian() { Data->SwapEndian(); }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaAnimationContainer_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;

  enablePtrPair(skeletons) GetNumSkeletons() const {
    return this->numSkeletons;
  }
  enablehkArray(skeletons) GetNumSkeletons() const {
    return this->skeletons.count;
  }
  enablePtrPairRef(skeletons) GetNumSkeletons() { return this->numSkeletons; }
  enablehkArrayRef(skeletons) GetNumSkeletons() {
    return this->skeletons.count;
  }

  enablePtrPair(animations) GetNumAnimations() const {
    return this->numAnimations;
  }
  enablehkArray(animations) GetNumAnimations() const {
    return this->animations.count;
  }
  enablePtrPairRef(animations) GetNumAnimations() {
    return this->numAnimations;
  }
  enablehkArrayRef(animations) GetNumAnimations() {
    return this->animations.count;
  }

  enablePtrPair(bindings) GetNumBindings() const { return this->numBindings; }
  enablehkArray(bindings) GetNumBindings() const {
    return this->bindings.count;
  }
  enablePtrPairRef(bindings) GetNumBindings() { return this->numBindings; }
  enablehkArrayRef(bindings) GetNumBindings() { return this->bindings.count; }

  enablePtrPair(attachments) GetNumAttachments() const {
    return this->numAttachemts;
  }
  enablehkArray(attachments) GetNumAttachments() const {
    return this->attachments.count;
  }
  enablePtrPairRef(attachments) GetNumAttachments() {
    return this->numAttachemts;
  }
  enablehkArrayRef(attachments) GetNumAttachments() {
    return this->attachments.count;
  }

  enablePtrPair(skins) GetNumSkins() const { return this->numSkins; }
  enablehkArray(skins) GetNumSkins() const { return this->skins.count; }
  enablePtrPairRef(skins) GetNumSkins() { return this->numSkins; }
  enablehkArrayRef(skins) GetNumSkins() { return this->skins.count; }

  const hkaSkeleton *GetSkeleton(IhkPackFile *header, int id) const {
    return dynamic_cast<const hkaSkeleton *>(
        header->GetClass(this->skeletons[id]));
  };
  const hkaAnimation *GetAnimation(IhkPackFile *header, int id) const {
    return dynamic_cast<const hkaAnimation *>(
        header->GetClass(this->animations[id]));
  };
  const hkaAnimationBinding *GetBinding(IhkPackFile *header, int id) const {
    return dynamic_cast<const hkaAnimationBinding *>(
        header->GetClass(this->bindings[id]));
  };
  const hkaBoneAttachment *GetAttachment(IhkPackFile *header, int id) const {
    return dynamic_cast<const hkaBoneAttachment *>(
        header->GetClass(this->attachments[id]));
  };
  const hkaMeshBinding *GetSkin(IhkPackFile *header, int id) const {
    return dynamic_cast<const hkaMeshBinding *>(
        header->GetClass(this->skins[id]));
  };

  void SwapEndian() {
    FByteswapper(GetNumSkeletons());
    FByteswapper(GetNumAnimations());
    FByteswapper(GetNumBindings());
    FByteswapper(GetNumAttachments());
    FByteswapper(GetNumSkins());
  }
};

template <template <class C> class _ipointer>
struct hkaAnimationContainer660_t_sharedData {
  _ipointer<_ipointer<hkaSkeleton>> skeletons;
  int numSkeletons;
  _ipointer<_ipointer<hkaAnimation>> animations;
  int numAnimations;
  _ipointer<_ipointer<hkaAnimationBinding>> bindings;
  int numBindings;
  _ipointer<_ipointer<hkaBoneAttachment>> attachments;
  int numAttachemts;
  _ipointer<_ipointer<hkaMeshBinding>> skins;
  int numSkins;
};

template <template <class C> class _ipointer>
using hkaAnimationContainer660_t =
    hkaAnimationContainer_t_shared<_ipointer,
                                   hkaAnimationContainer660_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaAnimationContainer660_rp_t : hkaAnimationContainer660_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimationContainer710_t_sharedData : hkReferenceObject<_ipointer> {
  hkArray<_ipointer<hkaSkeleton>, _ipointer> skeletons;
  hkArray<_ipointer<hkaAnimation>, _ipointer> animations;
  hkArray<_ipointer<hkaAnimationBinding>, _ipointer> bindings;
  hkArray<_ipointer<hkaBoneAttachment>, _ipointer> attachments;
  hkArray<_ipointer<hkaMeshBinding>, _ipointer> skins;
};

template <template <class C> class _ipointer>
using hkaAnimationContainer710_t =
    hkaAnimationContainer_t_shared<_ipointer,
                                   hkaAnimationContainer710_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaAnimationContainer710_rp_t : hkaAnimationContainer710_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimationContainer2010_t : hkaAnimationContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2010_rp_t : hkaAnimationContainer710_t<_ipointer> {
};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2011_t : hkaAnimationContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2011_rp_t : hkaAnimationContainer710_t<_ipointer> {
};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2012_t : hkaAnimationContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2012_rp_t : hkaAnimationContainer710_t<_ipointer> {
};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2013_t : hkaAnimationContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2013_rp_t : hkaAnimationContainer710_t<_ipointer> {
};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2014_t : hkaAnimationContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2014_rp_t : hkaAnimationContainer710_t<_ipointer> {
};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2015_t : hkaAnimationContainer710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2015_rp_t : hkaAnimationContainer710_t<_ipointer> {
};

template <template <class C> class _ipointer>
struct hkaAnimationContainer550_t : hkaAnimationContainer660_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaAnimationContainer550_rp_t : hkaAnimationContainer660_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimationContainer2016_t_sharedData
    : hkReferenceObject2016<_ipointer> {
  hkArray<_ipointer<hkaSkeleton>, _ipointer> skeletons;
  hkArray<_ipointer<hkaAnimation>, _ipointer> animations;
  hkArray<_ipointer<hkaAnimationBinding>, _ipointer> bindings;
  hkArray<_ipointer<hkaBoneAttachment>, _ipointer> attachments;
  hkArray<_ipointer<hkaMeshBinding>, _ipointer> skins;
};

template <template <class C> class _ipointer>
using hkaAnimationContainer2016_t =
    hkaAnimationContainer_t_shared<_ipointer,
                                   hkaAnimationContainer2016_t_sharedData>;
template <template <class C> class _ipointer>
struct hkaAnimationContainer2016_rp_t : hkaAnimationContainer710_t<_ipointer> {
};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2017_t
    : hkaAnimationContainer_t_shared<_ipointer,
                                     hkaAnimationContainer2016_t_sharedData> {};
template <template <class C> class _ipointer>
struct hkaAnimationContainer2017_rp_t : hkaAnimationContainer710_t<_ipointer> {
};