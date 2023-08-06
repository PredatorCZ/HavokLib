/*  Havok Format Library
    Copyright(C) 2016-2023 Lukas Cone

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
#include "spike/uni/skeleton.hpp"

struct hkLocalFrame;

struct hkLocalFrameOnBone {
  const hkLocalFrame *localFrame;
  int boneIndex;
};

struct hkaPartition {
  std::string name;
  int16 startBoneIndex;
  uint16 numBones;

  hkaPartition() = default;
  hkaPartition(std::string_view iName, int16 stboneid, uint16 iNumBones)
      : name(iName), startBoneIndex(stboneid), numBones(iNumBones) {}
};

struct hkaSkeleton : IhkVirtualClass, uni::Skeleton {
  DECLARE_HKCLASS(hkaSkeleton)
  virtual size_t GetNumFloatSlots() const = 0;
  virtual std::string_view GetFloatSlot(size_t id) const = 0;
  virtual size_t GetNumLocalFrames() const = 0;
  virtual hkLocalFrameOnBone GetLocalFrame(size_t id) const = 0;
  virtual size_t GetNumPartitions() const = 0;
  virtual hkaPartition GetPartition(size_t id) const = 0;
  virtual size_t GetNumBones() const = 0;
  virtual std::string_view GetBoneName(size_t id) const = 0;
  virtual const hkQTransform *GetBoneTM(size_t id) const = 0;
  virtual int16 GetBoneParentID(size_t id) const = 0;
  virtual size_t GetNumReferenceFloats() const = 0;
  virtual float GetReferenceFloat(size_t id) const = 0;

  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumFloatSlots,
                                    std::string_view, &hkaSkeleton::GetFloatSlot>
      iteratorFloatSlots;
  typedef uni::VirtualIteratorProxy<
      hkaSkeleton, &hkaSkeleton::GetNumLocalFrames, hkLocalFrameOnBone,
      &hkaSkeleton::GetLocalFrame>
      iteratorLocalFrames;
  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumPartitions,
                                    hkaPartition, &hkaSkeleton::GetPartition>
      iteratorPartitions;
  typedef uni::VirtualIteratorProxy<hkaSkeleton,
                                    &hkaSkeleton::GetNumReferenceFloats, float,
                                    &hkaSkeleton::GetReferenceFloat>
      iteratorReferenceFloat;
  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumBones,
                                    std::string_view, &hkaSkeleton::GetBoneName>
      iteratorBoneNames;
  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumBones,
                                    const hkQTransform *,
                                    &hkaSkeleton::GetBoneTM>
      iteratorBoneTMs;
  typedef uni::VirtualIteratorProxy<hkaSkeleton, &hkaSkeleton::GetNumBones,
                                    int16, &hkaSkeleton::GetBoneParentID>
      iteratorBoneParentIDs;

  iteratorFloatSlots FloatSlots() const { return iteratorFloatSlots(this); }
  iteratorLocalFrames LocalFrames() const { return iteratorLocalFrames(this); }
  iteratorPartitions Partitions() const { return iteratorPartitions(this); }
  iteratorReferenceFloat ReferenceFloats() const {
    return iteratorReferenceFloat(this);
  }
  iteratorBoneNames BoneNames() const { return iteratorBoneNames(this); }
  iteratorBoneTMs BoneTransforms() const { return iteratorBoneTMs(this); }
  iteratorBoneParentIDs BoneParentIDs() const {
    return iteratorBoneParentIDs(this);
  }
};
