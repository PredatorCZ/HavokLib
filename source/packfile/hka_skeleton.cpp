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

#include "internal/hka_skeleton.hpp"
#include "base.hpp"
#include "hka_skeleton.inl"
#include "uni/list_vector.hpp"
#include <span>

struct hkaSkeletonSaver {
  const hkaSkeletonInternalInterface *in;
  const clgen::hkaSkeleton::Interface *out;

  void Save(BinWritterRef wr, hkFixups &fixups) {
    const size_t sBegin = wr.Tell();
    auto &locals = fixups.locals;
    auto &lay = *out->layout;
    using mm = clgen::hkaSkeleton::Members;

    wr.WriteBuffer(out->data, lay.totalSize);
    wr.ApplyPadding();
    locals.emplace_back(sBegin + out->m(mm::name), wr.Tell());
    wr.WriteContainer(in->Name());
    wr.Skip(1);

    const size_t numBones = in->GetNumBones();

    if (numBones) {
      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + out->m(mm::parentIndices), wr.Tell());

      for (auto i : in->BoneParentIDs()) {
        wr.Write(i);
      }

      wr.ApplyPadding();
      locals.emplace_back(sBegin + out->m(mm::transforms), wr.Tell());

      for (auto i : in->BoneTransforms()) {
        wr.Write(i);
      }

      wr.ApplyPadding(8);
      locals.emplace_back(sBegin + out->m(mm::bones), wr.Tell());
      size_t curFixup = locals.size();
      const auto boneType =
          clgen::GetLayout(clgen::hkaBone::LAYOUTS, out->lookup);

      if (out->LayoutVersion() < HK700) {
        size_t curGFixup = fixups.globals.size();

        for (size_t i = 0; i < numBones; i++) {
          fixups.globals.emplace_back(wr.Tell());
          wr.Skip(lay.ptrSize);
        }

        auto fndFinal = std::find_if(
            fixups.finals.begin(), fixups.finals.end(), [&](const hkFixup &f) {
              return f.destClass == dynamic_cast<const hkVirtualClass *>(in);
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
          wr.Skip(boneType->totalSize);
          wr.ApplyPadding(8);
          locals.emplace_back(bneBegin, wr.Tell());
          wr.WriteContainer(in->GetBoneName(i));
          wr.Skip(1);
        }
      } else {
        for (size_t i = 0; i < numBones; i++) {
          locals.emplace_back(wr.Tell());
          wr.Skip(boneType->totalSize);
        }
        for (auto i : in->BoneNames()) {
          wr.ApplyPadding(8);
          locals[curFixup++].destination = wr.Tell();
          wr.WriteContainer(i);
          wr.Skip(1);
        }
      }
    }
  }
};

class hkFullBone : public uni::Bone {
public:
  es::string_view name;
  const hkQTransform *tm;
  hkFullBone *parent;
  size_t id;

  uni::TransformType TMType() const override {
    return uni::TransformType::TMTYPE_MATRIX;
  }
  void GetTM(uni::RTSValue &out) const override {
    out = tm ? *tm : uni::RTSValue{};
  }
  const Bone *Parent() const override { return parent; }
  size_t Index() const override { return id; }
  std::string Name() const override { return name; }
  operator uni::Element<const uni::Bone>() const {
    return {static_cast<const uni::Bone *>(this), false};
  }
};

struct hkaSkeletonMidInterface : hkaSkeletonInternalInterface {
  clgen::hkaSkeleton::Interface interface;
  std::unique_ptr<hkaSkeletonSaver> saver;
  uni::VectorList<uni::Bone, hkFullBone> bones;

  hkaSkeletonMidInterface(clgen::LayoutLookup rules, char *data) : interface {
    data, rules
  } {
  }

  void SetDataPointer(void *ptr) override {
    interface.data = static_cast<char *>(ptr);
  }

  const void *GetPointer() const override { return interface.data; }

  size_t GetNumBones() const override { return interface.NumBones(); }

  void Process() override {
    size_t numBones = GetNumBones();
    bones.storage.resize(numBones);

    if (interface.LayoutVersion() >= HK700) {
      auto bonesIter = interface.BonesHK700();

      for (size_t b = 0; b < numBones; b++, bonesIter.Next()) {
        bones.storage.at(b).name = bonesIter.Name();
      }
    } else {
      auto bonesIter = interface.Bones();

      for (size_t b = 0; b < numBones; b++, bonesIter.Next()) {
        hkFullBone &bone = bones.storage.at(b);
        bone.name = (*bonesIter).Name();
      }
    }

    for (size_t b = 0; b < numBones; b++) {
      hkFullBone &bone = bones.storage.at(b);
      bone.id = b;
      int16 prentID = interface.ParentIndices()[b];
      bone.parent = prentID < 0 ? nullptr : &bones.storage[prentID];
      bone.tm = interface.Transforms() + b;
    }
  }

  es::string_view GetBoneName(size_t id) const override {
    return bones.storage.at(id).name;
  }
  const hkQTransform *GetBoneTM(size_t id) const override {
    return bones.storage.at(id).tm;
  }
  int16 GetBoneParentID(size_t id) const override {
    if (auto parent = bones.storage.at(id).parent) {
      return parent->id;
    }

    return -1;
  }
  std::string Name() const override { return interface.Name(); };
  uni::SkeletonBonesConst Bones() const override {
    return uni::SkeletonBonesConst(
        static_cast<const uni::List<uni::Bone> *>(&bones), false);
  }

  size_t GetNumFloatSlots() const override {
    return interface.NumFloatSlots();
  };
  es::string_view GetFloatSlot(size_t id) const override {
    return *interface.FloatSlots().Next(id);
  };
  size_t GetNumLocalFrames() const override {
    return interface.NumLocalFrames();
  };
  hkLocalFrameOnBone GetLocalFrame(size_t id) const override {
    auto item = interface.LocalFrames().Next(id);
    return {item.LocalFrame(), item.BoneIndex()};
  };
  size_t GetNumPartitions() const override {
    return interface.NumPartitions();
  };
  hkaPartition GetPartition(size_t id) const override {
    auto item = interface.Partitions().Next(id);
    return {item.Name(), item.StartBoneIndex(), item.NumBones()};
  };
  size_t GetNumReferenceFloats() const override {
    return interface.NumReferenceFloats();
  };
  float GetReferenceFloat(size_t id) const override {
    return interface.ReferenceFloats()[id];
  }

  void SwapEndian() override {
    clgen::EndianSwap(interface);

    for (std::span<int16> indices(interface.ParentIndices(),
                                  interface.NumParentIndices());
         auto &i : indices) {
      FByteswapper(i);
    }

    for (std::span<hkQTransform> tms(interface.Transforms(),
                                     interface.NumTransforms());
         auto &i : tms) {
      FByteswapper(i.rotation);
      FByteswapper(i.scale);
      FByteswapper(i.translation);
    }

    for (std::span<float> refs(interface.ReferenceFloats(),
                               interface.NumReferenceFloats());
         auto &i : refs) {
      FByteswapper(i);
    }

    {
      size_t numParts = interface.NumPartitions();
      auto parts = interface.Partitions();

      for (size_t i = 0; i < numParts; i++, parts.Next()) {
        clgen::EndianSwap(parts);
      }
    }

    {
      size_t numParts = interface.NumLocalFrames();
      auto parts = interface.LocalFrames();

      for (size_t i = 0; i < numParts; i++, parts.Next()) {
        clgen::EndianSwap(parts);
      }
    }
  }

  void Reflect(const IhkVirtualClass *other) override {
    interface.data = static_cast<char *>(malloc(interface.layout->totalSize));
    saver = std::make_unique<hkaSkeletonSaver>();
    saver->in = static_cast<const hkaSkeletonInternalInterface *>(
        checked_deref_cast<const hkaSkeleton>(other));
    saver->out = &interface;
    interface.NumBones(saver->in->GetNumBones());
    interface.NumParentIndices(saver->in->GetNumBones());
    interface.NumTransforms(saver->in->GetNumBones());
    interface.NumFloatSlots(saver->in->GetNumFloatSlots());
    interface.NumReferenceFloats(saver->in->GetNumReferenceFloats());
    interface.NumPartitions(saver->in->GetNumPartitions());
    interface.NumLocalFrames(saver->in->GetNumLocalFrames());
  }

  void Save(BinWritterRef wr, hkFixups &fixups) const override {
    saver->Save(wr, fixups);
  }

  ~hkaSkeletonMidInterface() {
    if (saver) {
      free(interface.data);
    }
  }
};

hkVirtualClass *hkaSkeletonInternalInterface::Create(CRule rule) {
  return new hkaSkeletonMidInterface{
      clgen::LayoutLookup{rule.version, rule.x64, rule.reusePadding}, nullptr};
}
