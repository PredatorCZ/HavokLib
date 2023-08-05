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

struct hkaAnimation;

// ADDITIVE_DEPRECATED since 2010
MAKE_ENUM(ENUMSCOPE(BlendHint
                    : uint8, BlendHint),
          EMEMBER(NORMAL), EMEMBER(ADDITIVE_DEPRECATED), EMEMBER(ADDITIVE))

struct hkaAnimationBinding : IhkVirtualClass {
  DECLARE_HKCLASS(hkaAnimationBinding)

  virtual std::string_view GetSkeletonName() const = 0;
  virtual const hkaAnimation *GetAnimation() const = 0;
  virtual BlendHint GetBlendHint() const = 0;
  virtual size_t GetNumTransformTrackToBoneIndices() const = 0;
  virtual int16 GetTransformTrackToBoneIndex(size_t id) const = 0;
  virtual size_t GetNumFloatTrackToFloatSlotIndices() const = 0;
  virtual int16 GetFloatTrackToFloatSlotIndex(size_t id) const = 0;
  virtual size_t GetNumPartitionIndices() const = 0;
  virtual int16 GetPartitionIndex(size_t id) const = 0;

  typedef uni::VirtualIteratorProxy<
      hkaAnimationBinding,
      &hkaAnimationBinding::GetNumTransformTrackToBoneIndices, int16,
      &hkaAnimationBinding::GetTransformTrackToBoneIndex>
      iteratorTransformTrackToBoneIndices;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationBinding,
      &hkaAnimationBinding::GetNumFloatTrackToFloatSlotIndices, int16,
      &hkaAnimationBinding::GetFloatTrackToFloatSlotIndex>
      iteratorFloatTrackToFloatSlotIndices;
  typedef uni::VirtualIteratorProxy<
      hkaAnimationBinding, &hkaAnimationBinding::GetNumPartitionIndices, int16,
      &hkaAnimationBinding::GetPartitionIndex>
      iteratorNumPartitionIndices;

  const iteratorTransformTrackToBoneIndices
  TransformTrackToBoneIndices() const {
    return iteratorTransformTrackToBoneIndices(this);
  }
  const iteratorFloatTrackToFloatSlotIndices
  FloatTrackToFloatSlotIndices() const {
    return iteratorFloatTrackToFloatSlotIndices(this);
  }
  const iteratorNumPartitionIndices PartitionIndices() const {
    return iteratorNumPartitionIndices(this);
  }
};
