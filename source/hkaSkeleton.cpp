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

#include "hkInternalInterfaces.h"
#include "hkObjectBase.h"

namespace {

enum PointerType {
  PT_NAME,
  PT_PARENT_INDICES,
  PT_BONES,
  PT_TRANSFORMS,
  PT_REFERENCE_FLOATS,
  PT_FLOAT_SLOTS,
  PT_LOCAL_FRAMES,
  PT_PARTITIONS
};

struct hkaSkeletonSaver {
  es::string_view main;
  const hkaSkeletonInternalInterface *interface;
  CRule rule;
  const size_t *offsets;

  size_t boneDataSize;

  void Save(BinWritterRef wr, hkFixups &fixups) {
    const size_t sBegin = wr.Tell();
    auto &locals = fixups.locals;
    wr.WriteContainer(main);

    wr.ApplyPadding();
    locals.emplace_back(sBegin + offsets[PT_NAME], wr.Tell());
    wr.WriteContainer(interface->Name());
    wr.Skip(1);

    const size_t numBones = interface->GetNumBones();
    const size_t ptrSize = rule.x64 ? 8 : 4;

    if (numBones) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + offsets[PT_PARENT_INDICES], wr.Tell());

      for (auto i : interface->BoneParentIDs()) {
        wr.Write(i);
      }

      wr.ApplyPadding();
      locals.emplace_back(sBegin + offsets[PT_TRANSFORMS], wr.Tell());

      for (auto i : interface->BoneTransforms()) {
        wr.Write(i);
      }

      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + offsets[PT_BONES], wr.Tell());
      size_t curFixup = locals.size();

      if (rule.version < HK700) {
        size_t curGFixup = fixups.globals.size();

        for (size_t i = 0; i < numBones; i++) {
          fixups.globals.emplace_back(wr.Tell());
          wr.Skip(ptrSize);
        }

        auto fndFinal = std::find_if(
            fixups.finals.begin(), fixups.finals.end(), [&](const hkFixup &f) {
              return f.destClass ==
                     dynamic_cast<const hkVirtualClass *>(interface);
            });

        if (es::IsEnd(fixups.finals, fndFinal)) {
          throw std::runtime_error("hkaBone final was not found");
        }

        for (size_t i = 0; i < numBones; i++) {
          wr.ApplyPadding(8);
          fixups.globals[curGFixup++].destination = wr.Tell();
          const size_t bneBegin = wr.Tell();
          fndFinal->destination = bneBegin;
          fndFinal++;
          wr.Skip(boneDataSize);
          wr.ApplyPadding(8);
          locals.emplace_back(bneBegin, wr.Tell());
          wr.WriteContainer(interface->GetBoneName(i));
          wr.Skip(1);
        }
      } else {
        for (size_t i = 0; i < numBones; i++) {
          locals.emplace_back(wr.Tell());
          wr.Skip(boneDataSize);
        }
        for (auto i : interface->BoneNames()) {
          wr.ApplyPadding(8);
          locals[curFixup++].destination = wr.Tell();
          wr.WriteContainer(i);
          wr.Skip(1);
        }
      }
    }
  }
};

template <class C> struct hkaSkeleton_t : hkaSkeletonInternalInterface {
  uni::Element<C> data;
  std::unique_ptr<hkaSkeletonSaver> saver;

  void SetDataPointer(void *ptr) override {
    data = {static_cast<C *>(ptr), false};
  }
  const void *GetPointer() const override { return data.get(); }
  size_t GetNumBones() const override { return data->GetNumBones(0); }
  es::string_view GetBoneName(size_t id) const override {
    return data->GetBoneName(id);
  }
  const hkQTransform *GetBoneTM(size_t id) const override {
    if (id < data->GetNumTransforms(0))
      return data->GetBoneTM(id);

    return nullptr;
  }
  int16 GetBoneParentID(size_t id) const override {
    if (id < data->GetNumParentIndices(0))
      return data->GetBoneParentID(id);

    return -1;
  }
  es::string_view Name() const override { return data->GetSkeletonName(); };
  size_t GetNumFloatSlots() const override {
    return data->GetNumFloatSlots(0);
  };
  es::string_view GetFloatSlot(size_t id) const override {
    return data->GetFloatSlot(id);
  };
  size_t GetNumLocalFrames() const override {
    return data->GetNumLocalFrames(0);
  };
  hkLocalFrameOnBone GetLocalFrame(size_t id) const override {
    return data->GetLocalFrame(id);
  };
  size_t GetNumPartitions() const override {
    return data->GetNumPartitions(0);
  };
  hkaPartition GetPartition(size_t id) const override {
    return data->GetPartition(id);
  };
  size_t GetNumReferenceFloats() const override {
    return data->GetNumReferenceFloats(0);
  };
  float GetReferenceFloat(size_t id) const override {
    return data->GetReferenceFloat(id);
  }

  void SwapEndian() override { data->SwapEndian(); }

  void Reflect(IhkVirtualClass *interface) override {
    data = std::move(decltype(data){new C()});
    saver = std::move(decltype(saver){new hkaSkeletonSaver()});
    saver->main = {reinterpret_cast<const char *>(data.get()), sizeof(C)};
    saver->interface = dynamic_cast<hkaSkeletonInternalInterface *>(interface);
    saver->offsets = C::GetPtrOffsets();
    saver->boneDataSize = sizeof(typename C::bone_type);
    saver->rule = this->rule;

    if (!saver->interface) {
      throw std::runtime_error("Incorrect interface class!");
    }

    data->GetNumBones(0) = saver->interface->GetNumBones();
    data->GetNumParentIndices(0) = saver->interface->GetNumBones();
    data->GetNumTransforms(0) = saver->interface->GetNumBones();
    data->GetNumFloatSlots(0) = saver->interface->GetNumFloatSlots();
    data->GetNumReferenceFloats(0) = saver->interface->GetNumReferenceFloats();
    data->GetNumPartitions(0) = saver->interface->GetNumPartitions();
    data->GetNumLocalFrames(0) = saver->interface->GetNumLocalFrames();
  }

  void Save(BinWritterRef wr, hkFixups &fixups) const override {
    saver->Save(wr, fixups);
  }
};

template <template <class C> class _ipointer> struct hkLocalFrameOnBone_t {
  _ipointer<hkLocalFrame> localFrame;
  int32 boneIndex;
};

template <template <class C> class _ipointer> struct hkLocalFrameOnBone2_t {
  _ipointer<hkLocalFrame> localFrame;
  int16 boneIndex;
};

template <template <class C> class _ipointer> struct hkaBone_t {
  _ipointer<char> name;
  int32 lockTranslation;
};

template <template <class C> class _ipointer> struct hkaPartition_t {
  _ipointer<char> Name;
  int16 startBoneIndex;
  uint16 numBones;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaSkeleton_t_shared : _parent<_ipointer> {
  using parent_class = _parent<_ipointer>;
  using bone_type = hkaBone_t<_ipointer>;

  GetNum(Bones);
  GetNum(ParentIndices);
  GetNum(Transforms);
  GetNum(LocalFrames);
  GetNum(FloatSlots);
  GetNum(ReferenceFloats);
  GetNum(Partitions);

  AccessMethod(LocalFrame, hkLocalFrameOnBone{}) {
    const auto &lcFrame = this->LocalFrames[id];
    return {lcFrame.localFrame, lcFrame.boneIndex};
  }

  AccessMethod(FloatSlot, (const char *)nullptr) {
    return this->FloatSlots[id];
  }

  template <class C = parent_class>
  auto GetBoneName(size_t id) const
      -> decltype(std::declval<C>().numBones, (const char *)nullptr) {
    return this->Bones[id]->name;
  }

  template <class C = parent_class>
  auto GetBoneName(size_t id) const
      -> decltype(std::declval<C>().Bones.count, (const char *)nullptr) {
    return this->Bones[id].name;
  }

  const hkQTransform *GetBoneTM(size_t id) const {
    return &this->Transforms[id];
  }

  int16 GetBoneParentID(size_t id) const { return this->ParentIndices[id]; }

  const char *GetSkeletonName() const { return this->name; };

  AccessMethod(Partition, hkaPartition{}) {
    const auto &part = this->Partitions[id];
    return {static_cast<const char *>(part.Name), part.startBoneIndex,
            part.numBones};
  }

  AccessMethod(ReferenceFloat, 0.f) { return this->ReferenceFloats[id]; }

  SwapMethod(LocalFrames) {
    FByteswapper(GetNumLocalFrames(0));
    const size_t numLocalFrames = GetNumLocalFrames(0);

    for (size_t i = 0; i < numLocalFrames; i++)
      FByteswapper(this->LocalFrames[i].boneIndex);
  }

  SwapMethod(ReferenceFloats) {
    FByteswapper(GetNumReferenceFloats(0));
    const size_t numRefFloats = GetNumReferenceFloats(0);

    for (size_t i = 0; i < numRefFloats; i++)
      FByteswapper(this->ReferenceFloats[i]);
  }

  SwapMethod(Partitions) {
    FByteswapper(GetNumPartitions(0));
    const size_t numPartitions = GetNumPartitions(0);

    for (size_t i = 0; i < numPartitions; i++) {
      FByteswapper(this->Partitions[i].startBoneIndex);
      FByteswapper(this->Partitions[i].numBones);
    }
  }

  void SwapEndian() {
    FByteswapper(GetNumBones(0));
    FByteswapper(GetNumParentIndices(0));
    FByteswapper(GetNumTransforms(0));
    FByteswapper(GetNumFloatSlots(0));

    const size_t numBones = GetNumBones(0);

    short *pData = this->ParentIndices;
    short *pDataEnd = pData + numBones;

    for (; pData != pDataEnd; pData++)
      FByteswapper(*pData);

    float *data = reinterpret_cast<float *>(
        static_cast<hkQTransform *>(this->Transforms));
    float *dataEnd = data + numBones * 12;

    for (; data != dataEnd; data++)
      FByteswapper(*data);

    SwapLocalFrames(0);
    SwapReferenceFloats(0);
    SwapPartitions(0);
  }
};

template <template <class C> class _ipointer>
struct hkaSkeleton500_t_sharedData {
  _ipointer<char> name;
  _ipointer<short> ParentIndices;
  uint32 numParentIndices;
  _ipointer<_ipointer<hkaBone_t<_ipointer>>> Bones;
  uint32 numBones;
  _ipointer<hkQTransform> Transforms;
  uint32 numTransforms;

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkaSkeleton500_t_sharedData, name),
        offsetof(hkaSkeleton500_t_sharedData, ParentIndices),
        offsetof(hkaSkeleton500_t_sharedData, Bones),
        offsetof(hkaSkeleton500_t_sharedData, Transforms),
        0,
        0,
        0,
        0,
    };

    return offsets;
  }
};

template <template <class C> class _ipointer>
struct hkaSkeleton550_t_sharedData {
  _ipointer<char> name;
  _ipointer<short> ParentIndices;
  uint32 numParentIndices;
  _ipointer<_ipointer<hkaBone_t<_ipointer>>> Bones;
  uint32 numBones;
  _ipointer<hkQTransform> Transforms;
  uint32 numTransforms;
  _ipointer<_ipointer<char>> FloatSlots;
  uint32 numFloatSlots;

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkaSkeleton550_t_sharedData, name),
        offsetof(hkaSkeleton550_t_sharedData, ParentIndices),
        offsetof(hkaSkeleton550_t_sharedData, Bones),
        offsetof(hkaSkeleton550_t_sharedData, Transforms),
        0,
        offsetof(hkaSkeleton550_t_sharedData, FloatSlots),
        0,
        0,
    };

    return offsets;
  }
};

template <template <class C> class _ipointer>
struct hkaSkeleton600_t_sharedData {
  _ipointer<char> name;
  _ipointer<short> ParentIndices;
  uint32 numParentIndices;
  _ipointer<_ipointer<hkaBone_t<_ipointer>>> Bones;
  uint32 numBones;
  _ipointer<hkQTransform> Transforms;
  uint32 numTransforms;
  _ipointer<_ipointer<char>> FloatSlots;
  uint32 numFloatSlots;
  _ipointer<hkLocalFrameOnBone_t<_ipointer>> LocalFrames;
  uint32 numLocalFrames;

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkaSkeleton600_t_sharedData, name),
        offsetof(hkaSkeleton600_t_sharedData, ParentIndices),
        offsetof(hkaSkeleton600_t_sharedData, Bones),
        offsetof(hkaSkeleton600_t_sharedData, Transforms),
        0,
        offsetof(hkaSkeleton600_t_sharedData, FloatSlots),
        offsetof(hkaSkeleton600_t_sharedData, LocalFrames),
        0,
    };

    return offsets;
  }
};

template <template <class C> class _ipointer>
struct hkaSkeleton700_t_sharedData : hkReferenceObject<_ipointer> {
  _ipointer<char> name;
  hkArray<int16, _ipointer> ParentIndices;
  hkArray<hkaBone_t<_ipointer>, _ipointer> Bones;
  hkArray<hkQTransform, _ipointer> Transforms;
  hkArray<_ipointer<char>, _ipointer> FloatSlots;
  hkArray<hkLocalFrameOnBone_t<_ipointer>, _ipointer> LocalFrames;

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkaSkeleton700_t_sharedData, name),
        offsetof(hkaSkeleton700_t_sharedData, ParentIndices),
        offsetof(hkaSkeleton700_t_sharedData, Bones),
        offsetof(hkaSkeleton700_t_sharedData, Transforms),
        0,
        offsetof(hkaSkeleton700_t_sharedData, FloatSlots),
        offsetof(hkaSkeleton700_t_sharedData, LocalFrames),
        0,
    };

    return offsets;
  }
};

template <template <class C> class _ipointer>
struct hkaSkeleton2010_t_sharedData : hkReferenceObject<_ipointer> {
  _ipointer<char> name;
  hkArray<int16, _ipointer> ParentIndices;
  hkArray<hkaBone_t<_ipointer>, _ipointer> Bones;
  hkArray<hkQTransform, _ipointer> Transforms;
  hkArray<float, _ipointer> ReferenceFloats;
  hkArray<_ipointer<char>, _ipointer> FloatSlots;
  hkArray<hkLocalFrameOnBone_t<_ipointer>, _ipointer> LocalFrames;

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkaSkeleton2010_t_sharedData, name),
        offsetof(hkaSkeleton2010_t_sharedData, ParentIndices),
        offsetof(hkaSkeleton2010_t_sharedData, Bones),
        offsetof(hkaSkeleton2010_t_sharedData, Transforms),
        offsetof(hkaSkeleton2010_t_sharedData, ReferenceFloats),
        offsetof(hkaSkeleton2010_t_sharedData, FloatSlots),
        offsetof(hkaSkeleton2010_t_sharedData, LocalFrames),
        0,
    };

    return offsets;
  }
};

template <template <class C> class _ipointer>
struct hkaSkeleton2011_t_sharedData : hkReferenceObject<_ipointer> {
  _ipointer<char> name;
  hkArray<int16, _ipointer> ParentIndices;
  hkArray<hkaBone_t<_ipointer>, _ipointer> Bones;
  hkArray<hkQTransform, _ipointer> Transforms;
  hkArray<float, _ipointer> ReferenceFloats;
  hkArray<_ipointer<char>, _ipointer> FloatSlots;
  hkArray<hkLocalFrameOnBone2_t<_ipointer>, _ipointer> LocalFrames;

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkaSkeleton2011_t_sharedData, name),
        offsetof(hkaSkeleton2011_t_sharedData, ParentIndices),
        offsetof(hkaSkeleton2011_t_sharedData, Bones),
        offsetof(hkaSkeleton2011_t_sharedData, Transforms),
        offsetof(hkaSkeleton2011_t_sharedData, ReferenceFloats),
        offsetof(hkaSkeleton2011_t_sharedData, FloatSlots),
        offsetof(hkaSkeleton2011_t_sharedData, LocalFrames),
        0,
    };

    return offsets;
  }
};

template <template <class C> class _ipointer>
struct hkaSkeleton2012_t_sharedData : hkReferenceObject<_ipointer> {
  _ipointer<char> name;
  hkArray<int16, _ipointer> ParentIndices;
  hkArray<hkaBone_t<_ipointer>, _ipointer> Bones;
  hkArray<hkQTransform, _ipointer> Transforms;
  hkArray<float, _ipointer> ReferenceFloats;
  hkArray<_ipointer<char>, _ipointer> FloatSlots;
  hkArray<hkLocalFrameOnBone2_t<_ipointer>, _ipointer> LocalFrames;
  hkArray<hkaPartition_t<_ipointer>, _ipointer> Partitions;

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkaSkeleton2012_t_sharedData, name),
        offsetof(hkaSkeleton2012_t_sharedData, ParentIndices),
        offsetof(hkaSkeleton2012_t_sharedData, Bones),
        offsetof(hkaSkeleton2012_t_sharedData, Transforms),
        offsetof(hkaSkeleton2012_t_sharedData, ReferenceFloats),
        offsetof(hkaSkeleton2012_t_sharedData, FloatSlots),
        offsetof(hkaSkeleton2012_t_sharedData, LocalFrames),
        offsetof(hkaSkeleton2012_t_sharedData, Partitions),
    };

    return offsets;
  }
};

template <template <class C> class _ipointer>
struct hkaSkeleton2016_t_sharedData : hkReferenceObject2016<_ipointer> {
  _ipointer<char> name;
  hkArray<int16, _ipointer> ParentIndices;
  hkArray<hkaBone_t<_ipointer>, _ipointer> Bones;
  hkArray<hkQTransform, _ipointer> Transforms;
  hkArray<float, _ipointer> ReferenceFloats;
  hkArray<_ipointer<char>, _ipointer> FloatSlots;
  hkArray<hkLocalFrameOnBone2_t<_ipointer>, _ipointer> LocalFrames;
  hkArray<hkaPartition_t<_ipointer>, _ipointer> Partitions;

  static const size_t *GetPtrOffsets() {
    static const size_t offsets[] = {
        offsetof(hkaSkeleton2016_t_sharedData, name),
        offsetof(hkaSkeleton2016_t_sharedData, ParentIndices),
        offsetof(hkaSkeleton2016_t_sharedData, Bones),
        offsetof(hkaSkeleton2016_t_sharedData, Transforms),
        offsetof(hkaSkeleton2016_t_sharedData, ReferenceFloats),
        offsetof(hkaSkeleton2016_t_sharedData, FloatSlots),
        offsetof(hkaSkeleton2016_t_sharedData, LocalFrames),
        offsetof(hkaSkeleton2016_t_sharedData, Partitions),
    };

    return offsets;
  }
};

template <template <class C> class _ipointer>
using hkaSkeleton500 =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton500_t_sharedData>;

template <template <class C> class _ipointer>
using hkaSkeleton550 =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton550_t_sharedData>;

template <template <class C> class _ipointer>
using hkaSkeleton600 =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton600_t_sharedData>;

template <template <class C> class _ipointer>
using hkaSkeleton700 =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton700_t_sharedData>;

template <template <class C> class _ipointer>
using hkaSkeleton2010 =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton2010_t_sharedData>;

template <template <class C> class _ipointer>
using hkaSkeleton2011 =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton2011_t_sharedData>;

template <template <class C> class _ipointer>
using hkaSkeleton2012 =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton2012_t_sharedData>;

template <template <class C> class _ipointer>
using hkaSkeleton2016 =
    hkaSkeleton_t_shared<_ipointer, hkaSkeleton2016_t_sharedData>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkaSkeletonTier : hkaSkeleton2016<_ipointer> {};

hkClassTier0Alias(hkaSkeleton, 500, 500);
hkClassTier0Alias(hkaSkeleton, 500, 510);
hkClassTier0Alias(hkaSkeleton, 550, 550);
hkClassTier0Alias(hkaSkeleton, 600, 600);
hkClassTier0Alias(hkaSkeleton, 600, 610);
hkClassTier0Alias(hkaSkeleton, 600, 650);
hkClassTier0Alias(hkaSkeleton, 600, 660);
hkClassTier0Alias(hkaSkeleton, 700, 700);
hkClassTier0Alias(hkaSkeleton, 700, 710);
hkClassTier0Alias(hkaSkeleton, 2010, 2010_1);
hkClassTier0Alias(hkaSkeleton, 2010, 2010_2);
hkClassTier0Alias(hkaSkeleton, 2011, 2011_1);
hkClassTier0Alias(hkaSkeleton, 2011, 2011_2);
hkClassTier0Alias(hkaSkeleton, 2011, 2011_3);
hkClassTier0Alias(hkaSkeleton, 2012, 2012_1);
hkClassTier0Alias(hkaSkeleton, 2012, 2012_2);
hkClassTier0Alias(hkaSkeleton, 2012, 2013);
hkClassTier0Alias(hkaSkeleton, 2012, 2014);
hkClassTier0Alias(hkaSkeleton, 2012, 2015);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterFullClass(hkaSkeleton)};
} // namespace

hkVirtualClass *hkaSkeletonInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
