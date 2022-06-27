/*  Havok2GLTF
    Copyright(C) 2022 Lukas Cone
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

#include "datas/app_context.hpp"
#include "datas/binreader.hpp"
#include "datas/binwritter.hpp"
#include "datas/fileinfo.hpp"
#include "datas/master_printer.hpp"
#include "datas/reflector.hpp"
#include "gltf.hpp"
#include "havok_api.hpp"
#include "hklib/hka_animation.hpp"
#include "hklib/hka_animationbinding.hpp"
#include "hklib/hka_skeleton.hpp"
#include "project.h"
#include <mutex>

constexpr size_t NUM_EXTS = 18;

es::string_view filters[NUM_EXTS]{
    ".hkx$",
    ".hka$",
};

struct Havok2GLTF : ReflectorBase<Havok2GLTF> {
  std::string extensionsPatterns;
  std::string skeletonPath;
  uint16 sampleRate = 60;
} settings;

REFLECT(CLASS(Havok2GLTF),
        MEMBERNAME(extensionsPatterns, "extension-patterns", "p",
                   ReflDesc{"Specify extension patterns for file detecting "
                            "separated by comma."}),
        MEMBERNAME(skeletonPath, "skeleton-path", "s",
                   ReflDesc{"Specify path to a skeleton hkx."}),
        MEMBERNAME(sampleRate, "sample-rate", "r",
                   ReflDesc{"Specify frames per second for GLTF."}), )

static AppInfo_s appInfo{
    AppInfo_s::CONTEXT_VERSION,
    AppMode_e::CONVERT,
    ArchiveLoadType::FILTERED,
    Havok2GLTF_DESC " v" Havok2GLTF_VERSION ", " Havok2GLTF_COPYRIGHT
                    "Lukas Cone",
    reinterpret_cast<ReflectorFriend *>(&settings),
    filters,
};

AppInfo_s *AppInitModule() { return &appInfo; }

bool AppInitContext(const std::string &) {
  size_t filterIndex = 2;
  if (!settings.extensionsPatterns.empty()) {
    es::string_view sv(settings.extensionsPatterns);
    size_t lastPost = 0;
    auto found = sv.find(',');

    while (found != sv.npos) {
      auto sub = sv.substr(lastPost, found - lastPost);
      if (filterIndex >= NUM_EXTS) [[unlikely]] {
        printwarning(
            "Extension pattern space exhausted (limit is 16). Skipping: "
            << sub);
      } else {
        filters[filterIndex++] = es::TrimWhitespace(sub);
      }

      lastPost = ++found;
      found = sv.find(',', lastPost);
    }

    if (lastPost < sv.size()) {
      auto sub = es::TrimWhitespace(sv.substr(lastPost));
      if (filterIndex >= NUM_EXTS) [[unlikely]] {
        printwarning(
            "Extension pattern space exhausted (limit is 16). Skipping: "
            << sub);
      } else {
        filters[filterIndex] = sub;
      }
    }
  }

  return true;
}

static struct {
  IhkPackFile::Ptr skelFile;
  BinReader skelRdb;
  AppContextStream skelStream;
  BinReaderRef skelRd;
  std::mutex mtx;

  void LoadSkeleton(AppContext *ctx) {
    std::lock_guard g(mtx);

    if (!skelFile) {
      try {
        skelRdb.Open(settings.skeletonPath);
        skelRd = skelRdb;
      } catch (es::FileNotFoundError &e) {
        skelStream = ctx->RequestFile(settings.skeletonPath);
        skelRd = *skelStream.Get();
      }

      skelFile = IhkPackFile::Create(skelRd);
    }
  }
} skeleton;

struct GLTFHK : GLTF {
  void ProcessSkeleton(const uni::Skeleton &skel) {
    const size_t startIndex = nodes.size();
    std::vector<size_t> rootIndices;

    for (auto b : skel) {
      gltf::Node bone;
      uni::RTSValue value;
      b->GetTM(value);
      memcpy(bone.translation.data(), &value.translation,
             sizeof(bone.translation));
      memcpy(bone.rotation.data(), &value.rotation, sizeof(bone.rotation));
      memcpy(bone.rotation.data(), &value.rotation, sizeof(bone.rotation));
      auto parent = b->Parent();
      bone.name = b->Name();

      if (parent) {
        nodes[startIndex + parent->Index()].children.push_back(startIndex +
                                                               b->Index());
      } else {
        rootIndices.push_back(startIndex + b->Index());
      }

      nodes.emplace_back(std::move(bone));
    }

    if (rootIndices.size() > 1) {
      gltf::Node rootNode;
      rootNode.name = skel.Name();

      for (auto i : rootIndices) {
        rootNode.children.push_back(i);
      }
      scenes.back().nodes.push_back(nodes.size());
      nodes.emplace_back(std::move(rootNode));
    } else if (nodes.size() != startIndex) {
      scenes.back().nodes.push_back(rootIndices.front());
    }
  }

  void ProcessAnimation(const hkaAnimation *anim,
                        const hkaAnimationBinding *binding) {
    gltf::Animation glanim;
    glanim.channels.reserve(anim->GetNumOfTransformTracks() * 3);
    const size_t uppderLimit =
        gltfutils::FindTimeEndIndex(times, anim->Duration());
    auto tracks = anim->Tracks();

    for (auto t : *tracks) {
      size_t componentIndex = 0;
      for (auto stripResult =
               gltfutils::StripValuesBlock(times, uppderLimit, t.get());
           auto &r : stripResult) {
        gltf::Animation::Sampler sampler;
        sampler.input = AddKeyframes(r);
        sampler.output = AddData(r, componentIndex);

        gltf::Animation::Channel chan;
        chan.target.node =
            binding ? binding->GetTransformTrackToBoneIndex(t->BoneIndex())
                    : t->BoneIndex();
        chan.sampler = glanim.samplers.size();
        glanim.samplers.emplace_back(sampler);

        switch (componentIndex) {
        case uni::MotionTrack::Position:
          chan.target.path = "translation";
          break;
        case uni::MotionTrack::Rotation:
          chan.target.path = "rotation";
          break;
        case uni::MotionTrack::Scale:
          chan.target.path = "scale";
          break;
        default:
          break;
        }

        glanim.channels.emplace_back(std::move(chan));
        componentIndex++;
      }
    }

    animations.emplace_back(std::move(glanim));
  }

  uint32 AddKeyframes(gltfutils::StripResult &r) {
    auto foundKI = keyframesToAccessorIndex.find(r.timeIndices);

    if (es::IsEnd(keyframesToAccessorIndex, foundKI)) {
      GLTFStream &stream = [&] {
        if (keyframeStream == -1) {
          auto &stream = NewStream("keyframe-data");
          keyframeStream = stream.slot;
          return std::ref(stream);
        }
        return std::ref(Stream(keyframeStream));
      }();

      auto [keyAccess, keyIndex] = NewAccessor(stream, 4);
      keyAccess.count = r.timeIndices.size();
      keyAccess.type = gltf::Accessor::Type::Scalar;
      keyAccess.componentType = gltf::Accessor::ComponentType::Float;
      keyAccess.min.push_back(0);
      keyAccess.max.push_back(times[r.timeIndices.back()]);

      for (auto i : r.timeIndices) {
        stream.wr.Write(times[i]);
      }

      keyframesToAccessorIndex.emplace(std::move(r.timeIndices), keyIndex);
      return keyIndex;
    }

    return foundKI->second;
  }

  uint32 AddData(gltfutils::StripResult &r, uint32 componentIndex) {
    auto GetStream_ = [&] {
      if (animDataStream == -1) {
        auto &stream = NewStream("anim-data");
        animDataStream = stream.slot;
        return std::ref(stream);
      }
      return std::ref(Stream(animDataStream));
    };

    auto WriteData_ = [&] {
      auto stream = GetStream_();
      auto [dataAccess, dataIndex] = NewAccessor(stream, 4);
      dataAccess.count = r.values.size();
      auto &wr = stream.get().wr;

      if (componentIndex == uni::MotionTrack::Rotation) {
        dataAccess.componentType = gltf::Accessor::ComponentType::Short;
        dataAccess.type = gltf::Accessor::Type::Vec4;
        dataAccess.normalized = true;

        for (auto v : r.values) {
          wr.Write(Vector4A16(v * 32767).Convert<int16>());
        }
      } else {
        dataAccess.componentType = gltf::Accessor::ComponentType::Float;
        dataAccess.type = gltf::Accessor::Type::Vec3;

        for (auto v : r.values) {
          wr.Write<Vector>(v);
        }
      }

      return dataIndex;
    };

    if (r.values.size() == 1) {
      auto &val = r.values.front();

      if (componentIndex % 2) {
        std::array<float, 4> retVal{val.x, val.y, val.z, val.w};
        auto foundSI = rotToAccessorIndex.find(retVal);

        if (es::IsEnd(rotToAccessorIndex, foundSI)) {
          auto idx = WriteData_();
          rotToAccessorIndex.emplace(retVal, idx);
          return idx;
        }

        return foundSI->second;
      } else {
        std::array<float, 3> retVal{val.x, val.y, val.z};
        auto foundSI = scalarToAccessorIndex.find(retVal);

        if (es::IsEnd(scalarToAccessorIndex, foundSI)) {
          auto idx = WriteData_();
          scalarToAccessorIndex.emplace(retVal, idx);
          return idx;
        }

        return foundSI->second;
      }
    }

    return WriteData_();
  }

  std::map<std::vector<uint16>, uint32> keyframesToAccessorIndex;
  std::map<std::array<float, 3>, uint32> scalarToAccessorIndex;
  std::map<std::array<float, 4>, uint32> rotToAccessorIndex;
  std::vector<float> times;
  int32 keyframeStream{-1};
  int32 animDataStream{-1};
};

void AppProcessFile(std::istream &stream, AppContext *ctx) {
  BinReaderRef rd(stream);
  auto hkFile = IhkPackFile::Create(rd);
  auto skeletons = hkFile->GetClasses(hkaSkeleton::GetHash());

  if (skeletons.empty()) {
    if (settings.skeletonPath.empty()) {
      throw std::runtime_error(
          "No skeleton data found and no skeleton path provided.");
    }

    if (!skeleton.skelFile) {
      skeleton.LoadSkeleton(ctx);
    }

    skeletons = skeleton.skelFile->GetClasses(hkaSkeleton::GetHash());

    if (skeletons.empty()) {
      throw std::runtime_error(
          "Even provided skeleton file doesn't contain skeleton data.");
    }
  }

  GLTFHK main;

  // Load skeletons
  for (auto skel_ : skeletons) {
    auto skel = checked_deref_cast<const hkaSkeleton>(skel_);
    main.ProcessSkeleton(*skel);
  }

  // Animations tier 1
  [&] {
    auto bindings = hkFile->GetClasses(hkaAnimationBinding::GetHash());
    auto anims = hkFile->GetClasses(hkaAnimation::GetHash());

    if (anims.empty()) {
      return;
    }

    main.times = [&] {
      float maxDuration = 0;

      for (auto a_ : anims) {
        auto a = checked_deref_cast<const hkaAnimation>(a_);
        maxDuration = std::max(a->Duration(), maxDuration);
      }

      return gltfutils::MakeSamples(settings.sampleRate, maxDuration);
    }();

    if (bindings.size()) {
      if (bindings.size() < anims.size()) [[unlikely]] {
        printwarning(
            "Animation bindings count mismatch with animations. This should "
            "never happen but oh well. Some animations will be skipped.")
      }

      for (auto &b_ : bindings) {
        auto b = checked_deref_cast<const hkaAnimationBinding>(b_);
        auto anim = b->GetAnimation();
        main.ProcessAnimation(anim, b);
      }
    } else {
      printwarning("Animation bindings not found, animations might be "
                   "incorrect.");
      for (auto a_ : anims) {
        auto a = checked_deref_cast<const hkaAnimation>(a_);
        main.ProcessAnimation(a, nullptr);
      }
    }
  }();

  AFileInfo outPath(ctx->outFile);
  BinWritter wr(outPath.GetFullPathNoExt().to_string() + ".glb");

  main.FinishAndSave(wr, outPath.GetFolder());
}
