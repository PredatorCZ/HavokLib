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

#pragma once
#include "hk_base.hpp"
#include "uni/virtual_iterator.hpp"

struct hkaSkeleton;
struct hkaAnimation;
struct hkaAnimationBinding;
struct hkaBoneAttachment;
struct hkaMeshBinding;

struct hkaAnimationContainer : virtual IhkVirtualClass {
  DECLARE_HKCLASS(hkaAnimationContainer)

  virtual size_t GetNumSkeletons() const = 0;
  virtual const hkaSkeleton *GetSkeleton(size_t id) const = 0;
  virtual size_t GetNumAnimations() const = 0;
  virtual const hkaAnimation *GetAnimation(size_t id) const = 0;
  virtual size_t GetNumBindings() const = 0;
  virtual const hkaAnimationBinding *GetBinding(size_t id) const = 0;
  virtual size_t GetNumAttachments() const = 0;
  virtual const hkaBoneAttachment *GetAttachment(size_t id) const = 0;
  virtual size_t GetNumSkins() const = 0;
  virtual const hkaMeshBinding *GetSkin(size_t id) const = 0;

  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumSkeletons,
      const hkaSkeleton *, &hkaAnimationContainer::GetSkeleton>
      iteratorSkeletons;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumAnimations,
      const hkaAnimation *, &hkaAnimationContainer::GetAnimation>
      iteratorAnimations;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumBindings,
      const hkaAnimationBinding *, &hkaAnimationContainer::GetBinding>
      iteratorBindings;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumAttachments,
      const hkaBoneAttachment *, &hkaAnimationContainer::GetAttachment>
      iteratorAttachments;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationContainer, &hkaAnimationContainer::GetNumSkins,
      const hkaMeshBinding *, &hkaAnimationContainer::GetSkin>
      iteratorMeshBinds;

  const iteratorSkeletons Skeletons() const { return iteratorSkeletons(this); }
  const iteratorAnimations Animations() const {
    return iteratorAnimations(this);
  }
  const iteratorBindings Bindings() const { return iteratorBindings(this); }
  const iteratorAttachments Attachments() const {
    return iteratorAttachments(this);
  }
  const iteratorMeshBinds MeshBinds() const { return iteratorMeshBinds(this); }
};
