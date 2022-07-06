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
#include "datas/matrix44.hpp"
#include "datas/reflector.hpp"
#include "gltf.hpp"
#include "havok_api.hpp"
#include "hklib/hka_animation.hpp"
#include "hklib/hka_animationbinding.hpp"
#include "hklib/hka_skeleton.hpp"
#include "project.h"
#include <glm/gtx/quaternion.hpp>
#include <mutex>
#include <set>

constexpr size_t NUM_EXTS = 18;

es::string_view filters[NUM_EXTS]{
    ".hkx$",
    ".hka$",
};

MAKE_ENUM(ENUMSCOPE(class BlendHintOverride
                    : uint8, BlendHintOverride),
          EMEMBER(AUTO), EMEMBER(NORMAL), EMEMBER(ADDITIVE_DEPRECATED),
          EMEMBER(ADDITIVE));

MAKE_ENUM(
    ENUMSCOPE(class ScaleType
              : uint8, ScaleType),
    EMEMBER(NONE, "Ignore scale tracks."),
    /*EMEMBER(MATRICIAL,
            "Input scale tracks are from matricial decomposition.\nIt's common "
            "animaion pipeline, where scale motion affects positions of child "
            "nodes,\nbut introduces unwanted shearing in cases of both "
            "scale and rotation tracks being present."),*/
    EMEMBER(INDEPENDENT,
            "Input scale tracks does not affect child nodes positions.\n"
            "They also doesn't introduce shearing like in MATRICIAL case.\n"
            "They however require modified skeleton. See SkeletonGeneration."))

MAKE_ENUM(ENUMSCOPE(class SkeletonGeneration
                    : uint8, SkeletonGeneration),
          EMEMBER(DEFAULT, "Do not modify skeleton in any way."),
          EMEMBER(AUTO, "Automatically detect nodes with children where scale "
                        "animation might occur."),
          EMEMBER(MANUAL, "Manually select bones (in gen-cbones) "
                          "where scale motion occurs and have child nodes."))

struct AnimationSettings {
  uint8 sampleRate = 60;
  BlendHintOverride blendOverride = BlendHintOverride::AUTO;
  ScaleType scaleType = ScaleType::NONE;
  void ReflectorTag();
};

struct SkeletonSettings {
  std::string path;
  SkeletonGeneration generation;
  bool visualize = true;
  std::string generateControlBones;
  void ReflectorTag();
};

constexpr float INCH = 0.0254f;
constexpr float FEET = 0.3048f;
constexpr float MILE = 1609.344f;

MAKE_ENUM(ENUMSCOPE(class Unit
                    : uint8, Unit),
          EMEMBER(MM), EMEMBER(CM), EMEMBER(DM), EMEMBER(METER), EMEMBER(KM),
          EMEMBER(INCH), EMEMBER(FEET), EMEMBER(MILE), EMEMBER(CUSTOM))

MAKE_ENUM(ENUMSCOPE(class Axis
                    : uint8, Axis),
          EMEMBERNAME(Xn, "X-"), EMEMBERNAME(Yn, "Y-"), EMEMBERNAME(Zn, "Z-"),
          EMEMBERNAME(Xp, "X+"), EMEMBERNAME(Yp, "Y+"), EMEMBERNAME(Zp, "Z+"))

struct Scene {
  float customScale = 1.f;
  Unit units = Unit::METER;
  Axis upAxis = Axis::Yp;
  Axis forwardAxis = Axis::Zp;
  bool rightHanded = true;
  void ReflectorTag();
};

struct Havok2GLTF : ReflectorBase<Havok2GLTF> {
  std::string extensionsPatterns;
  AnimationSettings animation;
  SkeletonSettings skeleton;
  Scene scene;

  std::set<std::string_view> generateControlBones;
  float sceneScale = 0.f;
  es::Matrix44 corMat;
} settings;

REFLECT(
    CLASS(AnimationSettings),
    MEMBERNAME(blendOverride, "blend-override", "b",
               ReflDesc{"Override blend hint."}),
    MEMBERNAME(sampleRate, "sample-rate", "r",
               ReflDesc{"Specify frames per second for GLTF."}),
    MEMBERNAME(scaleType, "scale-type", "S",
               ReflDesc{
                   "Select desired processing mode for node scale tracks."}), )

REFLECT(CLASS(SkeletonSettings),
        MEMBERNAME(path, "skeleton-path", "s",
                   ReflDesc{"Specify path to a skeleton hkx."}),
        MEMBER(generation, "G",
               ReflDesc{"Set additional skeleton modifications."}),
        MEMBERNAME(generateControlBones, "gen-cbones", "B",
                   ReflDesc{"Comma separated bone names where should be "
                            "applied scale tracks."}),
        MEMBER(visualize, "V",
               ReflDesc{"Create visualization mesh for skeletons. (Enforces "
                        "armature object for Blender)"}), )

REFLECT(CLASS(Scene), MEMBER(units, "u", ReflDesc{"Input scene units."}),
        MEMBERNAME(customScale, "custom-scale", "C",
                   ReflDesc{"Set unit scale in case of CUSTOM units."}),
        MEMBERNAME(upAxis, "up-axis", "U", ReflDesc{"Input scene up axis."}),
        MEMBERNAME(forwardAxis, "forward-axis", "F",
                   ReflDesc{"Input scene forward axis."}),
        MEMBERNAME(rightHanded, "right-handed", "R",
                   ReflDesc{
                       "Input scene uses right handed coordiante system. (Finicky with root motion rotations)"}), )

REFLECT(CLASS(Havok2GLTF),
        MEMBERNAME(extensionsPatterns, "extension-patterns", "p",
                   ReflDesc{"Specify extension patterns for file detecting "
                            "separated by comma."}),
        MEMBER(animation), MEMBER(skeleton), MEMBER(scene))

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
  if (uint8(settings.scene.upAxis) % 3 ==
      uint8(settings.scene.forwardAxis) % 3) {
    throw std::runtime_error("Scene UP axis cannot be same as FORWARD axis!");
  }

  switch (settings.scene.units) {
  case Unit::MM:
    settings.sceneScale = 0.001f;
    break;
  case Unit::CM:
    settings.sceneScale = 0.01f;
    break;
  case Unit::DM:
    settings.sceneScale = 0.1f;
    break;
  case Unit::METER:
    settings.sceneScale = 1.f;
    break;
  case Unit::KM:
    settings.sceneScale = 1000.f;
    break;
  case Unit::INCH:
    settings.sceneScale = INCH;
    break;
  case Unit::FEET:
    settings.sceneScale = FEET;
    break;
  case Unit::MILE:
    settings.sceneScale = MILE;
    break;
  case Unit::CUSTOM:
    settings.sceneScale = settings.scene.customScale;
    break;

  default:
    throw std::logic_error("Unhandled unit type");
  }

  auto SetAxis = [](Axis axis) -> Vector4A16 {
    switch (axis) {
    case Axis::Xp:
      return {1, 0, 0, 0};
    case Axis::Xn:
      return {-1, 0, 0, 0};
    case Axis::Yp:
      return {0, 1, 0, 0};
    case Axis::Yn:
      return {0, -1, 0, 0};
    case Axis::Zp:
      return {0, 0, 1, 0};
    case Axis::Zn:
      return {0, 0, -1, 0};
    }
    throw std::logic_error("Unhandled axis type");
  };

  settings.corMat.r3() = SetAxis(settings.scene.forwardAxis);
  settings.corMat.r2() = SetAxis(settings.scene.upAxis);
  settings.corMat.r1() = settings.corMat.r2().Cross(settings.corMat.r3());

  if (!settings.scene.rightHanded) {
    settings.corMat.r1() *= -1;
  }

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

      lastPost = found++;
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

  if (!settings.skeleton.generateControlBones.empty()) {
    std::string_view sv(settings.skeleton.generateControlBones);
    size_t lastPost = 0;
    auto found = sv.find(',');

    while (found != sv.npos) {
      auto sub = sv.substr(lastPost, found - lastPost);
      settings.generateControlBones.emplace(es::TrimWhitespace(sub));

      lastPost = found++;
      found = sv.find(',', lastPost);
    }

    if (lastPost < sv.size()) {
      auto sub = es::TrimWhitespace(sv.substr(lastPost));
      settings.generateControlBones.emplace(sub);
    }
  }

  return true;
}

static const glm::quat &AsQuat(const Vector4A16 &in) {
  return reinterpret_cast<const glm::quat &>(in);
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
        skelRdb.Open(settings.skeleton.path);
        skelRd = skelRdb;
      } catch (es::FileNotFoundError &e) {
        skelStream = ctx->RequestFile(settings.skeleton.path);
        skelRd = *skelStream.Get();
      }

      skelFile = IhkPackFile::Create(skelRd);
    }
  }
} skeleton;

struct RootMixer : uni::MotionTrack {
  const uni::MotionTrack *rootNode;
  const uni::MotionTrack *reference;

  TrackType_e TrackType() const override {
    return uni::MotionTrack::PositionRotationScale;
  }
  size_t BoneIndex() const override { return {}; }
  void GetValue(uni::RTSValue &output, float time) const override {
    uni::RTSValue rootValue;
    rootNode->GetValue(rootValue, time);
    uni::RTSValue refValue;
    reference->GetValue(refValue, time);

    es::Matrix44 rootMtx;
    rootMtx.Compose(rootValue.translation, rootValue.rotation, {1, 1, 1, 0});
    es::Matrix44 refMtx;
    refMtx.Compose(refValue.translation, refValue.rotation, {1, 1, 1, 0});
    auto result = refMtx * rootMtx;
    result.Decompose(output.translation, output.rotation, output.scale);
    output.scale = rootValue.scale;
  }
};

struct ResampledScale : uni::MotionTrack {
  std::map<float, Vector4A16> values;

  TrackType_e TrackType() const override { return uni::MotionTrack::Scale; }
  size_t BoneIndex() const override { return {}; }
  void GetValue(Vector4A16 &output, float time) const override {
    output = values.at(time);
  }
};

struct GLTFHK : GLTF {
  void ControlBoneWalker(const std::map<size_t, Vector4A16> &controlBones,
                         size_t index, std::array<float, 3> scale) {
    auto &cBone = nodes.at(index);
    int32 numChildren = cBone.children.size();

    if (controlBones.contains(index)) {
      numChildren--;
      // The last pushed index is always a scale bone
      auto &sBone = nodes.at(cBone.children.back());
      reinterpret_cast<Vector &>(sBone.scale) *=
          reinterpret_cast<const Vector &>(scale);
      scale = sBone.scale;
    }

    for (int32 c = 0; c < numChildren; c++) {
      ControlBoneWalker(controlBones, cBone.children.at(c), scale);
    }
  };

  void ProcessSkeleton(const uni::Skeleton &skel) {
    const size_t startIndex = nodes.size();
    std::set<size_t> rootIndices;
    std::map<size_t, Vector4A16> controlBones;
    std::vector<std::pair<size_t, es::Matrix44>> boneLookupTMs;
    std::map<size_t, float> boneLens;

    for (auto b : skel) {
      gltf::Node bone;
      uni::RTSValue value;
      b->GetTM(value);
      value.translation *= settings.sceneScale;
      memcpy(bone.translation.data(), &value.translation,
             sizeof(bone.translation));
      memcpy(bone.rotation.data(), &value.rotation, sizeof(bone.rotation));
      auto parent = b->Parent();
      bone.name = b->Name();
      auto boneLen = value.translation.Length();
      boneLens.emplace(startIndex + b->Index(), boneLen);

      if ((settings.skeleton.generation == SkeletonGeneration::MANUAL &&
           settings.generateControlBones.contains(bone.name)) ||
          settings.skeleton.generation == SkeletonGeneration::AUTO) {
        controlBones.emplace(nodes.size(), value.scale);
      } else {
        memcpy(bone.scale.data(), &value.scale, sizeof(bone.scale));
      }

      if (parent) {
        auto &parentNode = nodes[startIndex + parent->Index()];
        parentNode.children.push_back(startIndex + b->Index());

        // Generate octahedron transforms for bone visualization
        // This might be still finicky
        es::Matrix44 lookupMtx;
        lookupMtx.r1() = value.translation.Normalized();
        lookupMtx.r2() =
            lookupMtx.r1() * es::Matrix44(Vector4A16(0, 0, 0.7, 0.7));
        lookupMtx.r3() = lookupMtx.r1().Cross(lookupMtx.r2());

        lookupMtx.r1() *= boneLen;
        lookupMtx.r2() *= boneLen;
        lookupMtx.r3() *= boneLen;

        boneLookupTMs.emplace_back(startIndex + parent->Index(), lookupMtx);
      } else {
        rootIndices.emplace(startIndex + b->Index());
      }

      nodes.emplace_back(std::move(bone));
    }

    if (settings.skeleton.generation == SkeletonGeneration::AUTO) {
      std::map<size_t, Vector4A16> controlBonesFiltered;
      for (auto [index, scale] : controlBones) {
        if (nodes[index].children.empty()) {
          memcpy(nodes[index].scale.data(), &scale, sizeof(nodes[index].scale));
          continue;
        }

        controlBonesFiltered.emplace(index, scale);
      }

      controlBones = std::move(controlBonesFiltered);
    }

    if (rootIndices.size() > 1) {
      gltf::Node rootNode;
      rootNode.name = skel.Name();

      for (auto i : rootIndices) {
        rootNode.children.push_back(i);
      }
      rootNodeIndex = nodes.size();
      auto rotation = settings.corMat.ToQuat();
      memcpy(rootNode.rotation.data(), &rotation, sizeof(rotation));
      nodes.emplace_back(std::move(rootNode));
    } else if (nodes.size() != startIndex) {
      rootNodeIndex = *rootIndices.begin();
      auto &node = nodes.at(rootNodeIndex);
      glm::quat rotation{node.rotation[3], node.rotation[0], node.rotation[1],
                         node.rotation[2]};
      auto ccRotation = AsQuat(settings.corMat.ToQuat()) * rotation;
      node.rotation[0] = ccRotation.x;
      node.rotation[1] = ccRotation.y;
      node.rotation[2] = ccRotation.z;
      node.rotation[3] = ccRotation.w;
    }

    scenes.back().nodes.push_back(rootNodeIndex);

    if (!controlBones.empty()) {
      for (auto [index, scale] : controlBones) {
        auto &cBone = nodes.at(index);
        if (appInfo.internalSettings->verbosity) {
          printinfo("Bone " << cBone.name << " converted to control.");
        }
        gltf::Node bone;
        bone.name = cBone.name;
        memcpy(bone.scale.data(), &scale, sizeof(scale));
        cBone.name.append("_control");
        cBone.children.push_back(nodes.size());
        boneLens.emplace(nodes.size(), boneLens.at(index));
        nodes.emplace_back(std::move(bone));
      }

      for (auto i : scenes.back().nodes) {
        ControlBoneWalker(controlBones, i, {1, 1, 1});
      }
    }

    if (settings.skeleton.visualize) {
      size_t idStreamSlot = 0;
      {
        static const Vector octa[]{
            {0, 0, 0},   {1, 1, -1}, {1, 1, 1},
            {1, -1, -1}, {1, -1, 1}, {10, 0, 0},
        };
        static const uint16 octaIndices[]{
            0, 2, 1, 0, 4, 2, 0, 3, 4, 0, 1, 3,
            5, 1, 2, 5, 2, 4, 5, 4, 3, 5, 3, 1,
        };

        auto &vtStream = NewStream("visual-vertices", 20);
        vtStream.target = gltf::BufferView::TargetType::ArrayBuffer;
        size_t vtStreamSlot = vtStream.slot;
        size_t numVerts = 6 * boneLookupTMs.size();
        auto [vpAcc, vpId] = NewAccessor(vtStream, 4);
        vpAcc.componentType = gltf::Accessor::ComponentType::Float;
        vpAcc.type = gltf::Accessor::Type::Vec3;
        vpAcc.count = numVerts;
        auto [biAcc, biId] = NewAccessor(vtStream, 4, 12);
        biAcc.componentType = gltf::Accessor::ComponentType::UnsignedByte;
        biAcc.type = gltf::Accessor::Type::Vec4;
        biAcc.count = numVerts;
        auto [bwAcc, bwId] = NewAccessor(vtStream, 4, 16, &biAcc);
        bwAcc.normalized = true;

        auto &idStream = NewStream("visual-indices");
        idStreamSlot = idStream.slot;
        idStream.target = gltf::BufferView::TargetType::ElementArrayBuffer;
        auto [idAcc, idId] = NewAccessor(idStream, 2);
        idAcc.componentType = gltf::Accessor::ComponentType::UnsignedShort;
        idAcc.type = gltf::Accessor::Type::Scalar;
        idAcc.count = 24 * boneLookupTMs.size();

        gltf::Primitive prim;
        prim.mode = gltf::Primitive::Mode::Triangles;
        prim.indices = idId;
        prim.attributes["POSITION"] = vpId;
        prim.attributes["JOINTS_0"] = biId;
        prim.attributes["WEIGHTS_0"] = bwId;

        gltf::Mesh mesh;
        mesh.primitives.emplace_back(std::move(prim));

        gltf::Skin skin;
        std::map<size_t, size_t> remaps;

        for (auto [id, len] : boneLookupTMs) {
          if (remaps.contains(id)) {
            continue;
          }
          skin.joints.push_back(id);
          remaps.emplace(id, remaps.size());
        };

        gltf::Node meshNode;
        meshNode.mesh = meshes.size();
        meshNode.skin = skins.size();
        scenes.back().nodes.push_back(nodes.size());
        nodes.push_back(meshNode);

        meshes.emplace_back(std::move(mesh));
        skins.emplace_back(std::move(skin));

        size_t localId = 0;

        auto &vtnStream = Stream(vtStreamSlot);

        for (auto [id, len] : boneLookupTMs) {
          for (auto t : octa) {
            Vector correctedPos = (t * 0.1f) * len;
            vtnStream.wr.Write(correctedPos);
            uint8 boneId[]{uint8(remaps[id]), 0, 0, 0};
            vtnStream.wr.Write(boneId);
            uint8 boneWt[]{0xff, 0, 0, 0};
            vtnStream.wr.Write(boneWt);
          }

          for (auto t : octaIndices) {
            uint16 correctedIndex = t + (localId * 6);
            idStream.wr.Write(correctedIndex);
          }
          localId++;
        }
      }
      {

#include "icosphere.hpp"
        auto &idStream = Stream(idStreamSlot);

        auto [idAcc, idId] = NewAccessor(idStream, 2);
        idAcc.componentType = gltf::Accessor::ComponentType::UnsignedShort;
        idAcc.type = gltf::Accessor::Type::Scalar;
        idAcc.count = 960;
        idStream.wr.Write(icoSphereIndices);

        auto &vtStream = NewStream("icovisual-basevertices", 16);
        vtStream.target = gltf::BufferView::TargetType::ArrayBuffer;
        auto [vpAcc, vpId] = NewAccessor(vtStream, 4);
        vpAcc.componentType = gltf::Accessor::ComponentType::Float;
        vpAcc.type = gltf::Accessor::Type::Vec3;
        vpAcc.count = 162;
        auto [bwAcc, bwId] = NewAccessor(vtStream, 4, 12);
        bwAcc.componentType = gltf::Accessor::ComponentType::UnsignedByte;
        bwAcc.type = gltf::Accessor::Type::Vec4;
        bwAcc.count = 162;
        bwAcc.normalized = true;

        for (auto t : icoSphereX3) {
          vtStream.wr.Write(t);
          uint8 boneWt[]{0xff, 0, 0, 0};
          vtStream.wr.Write(boneWt);
        }

        gltf::Mesh mesh;
        gltf::Skin skin;
        uint8 localId = 0;

        auto vbStreamSlot = NewStream("icovisual-vertices").slot;
        auto &viStream = NewStream("icovisual-ibms");
        auto [viAcc, viId] = NewAccessor(viStream, 16);
        skin.inverseBindMatrices = viId;
        viAcc.componentType = gltf::Accessor::ComponentType::Float;
        viAcc.type = gltf::Accessor::Type::Mat4;
        viAcc.count = boneLens.size();

        auto &vbStream = Stream(vbStreamSlot);
        vbStream.target = gltf::BufferView::TargetType::ArrayBuffer;

        const float avgLen = [&] {
          float totalLen = 0;
          int32 totalItems = 0;

          for (auto [id, len] : boneLens) {
            if (len < 0.00001) {
              continue;
            }

            totalLen += len;
            totalItems++;
          }

          return totalLen / totalItems;
        }();

        for (auto [id, len] : boneLens) {
          if (len < 0.00001) {
            len = avgLen;
          }
          es::Matrix44 ibm;
          len *= 0.05f;
          ibm.r1() *= len;
          ibm.r2() *= len;
          ibm.r3() *= len;
          viStream.wr.Write(ibm);
          skin.joints.push_back(id);
          auto [biAcc, biId] = NewAccessor(vbStream, 4);
          biAcc.componentType = gltf::Accessor::ComponentType::UnsignedByte;
          biAcc.type = gltf::Accessor::Type::Vec4;
          biAcc.count = 162;

          gltf::Primitive prim;
          prim.mode = gltf::Primitive::Mode::Triangles;
          prim.indices = idId;
          prim.attributes["POSITION"] = vpId;
          prim.attributes["NORMAL"] = vpId;
          prim.attributes["JOINTS_0"] = biId;
          prim.attributes["WEIGHTS_0"] = bwId;

          mesh.primitives.emplace_back(std::move(prim));
          uint8 boneId[]{localId++, 0, 0, 0};

          for (size_t i = 0; i < biAcc.count; i++) {
            vbStream.wr.Write(boneId);
          }
        }

        gltf::Node meshNode;
        meshNode.mesh = meshes.size();
        meshNode.skin = skins.size();
        scenes.back().nodes.push_back(nodes.size());
        nodes.push_back(meshNode);

        meshes.emplace_back(std::move(mesh));
        skins.emplace_back(std::move(skin));
      }
    }
  }

  void ProcessAnimation(const hkaAnimation *anim,
                        const hkaAnimationBinding *binding) {
    gltf::Animation glanim;
    glanim.channels.reserve(anim->GetNumOfTransformTracks() * 3);
    const size_t upperLimit =
        gltfutils::FindTimeEndIndex(times, anim->Duration());
    uni::Element<const uni::MotionTrack> rootTrack;
    auto rootMotion = anim->GetExtractedMotion();

    auto Sample = [&glanim, this](gltfutils::StripResult &r,
                                  size_t componentIndex, bool isRootNode) {
      gltf::Animation::Sampler sampler;
      sampler.input = AddKeyframes(r);
      sampler.output = AddData(r, componentIndex, isRootNode);

      glanim.samplers.emplace_back(sampler);
    };

    auto Path = [](size_t componentIndex) {
      switch (componentIndex) {
      case uni::MotionTrack::Position:
        return "translation";
      case uni::MotionTrack::Rotation:
        return "rotation";
      case uni::MotionTrack::Scale:
        return "scale";
      default:
        break;
      }

      throw std::logic_error("Invalid component index");
    };

    std::map<size_t, gltfutils::StripResult> independentScalers;
    bool useScaleMotion = false;

    for (auto tracks = anim->Tracks(); auto t : *tracks) {
      size_t componentIndex = 0;
      int32 nodeId = binding
                         ? binding->GetTransformTrackToBoneIndex(t->BoneIndex())
                         : t->BoneIndex();
      if (rootMotion && nodeId == rootNodeIndex) {
        rootTrack = std::move(t);
        continue;
      }

      for (auto stripResults =
               gltfutils::StripValuesBlock(times, upperLimit, t.get());
           auto &r : stripResults) {
        if (componentIndex == uni::MotionTrack::Scale) {
          if (settings.animation.scaleType == ScaleType::NONE) {
            if (!useScaleMotion && r.timeIndices.size() > 1) {
              useScaleMotion = true;
            }
            continue;
          }
        }

        /*if (componentIndex == uni::MotionTrack::Position) {
          if (settings.animation.scaleType == ScaleType::MATRICIAL) {
            continue;
          }
        }*/

        MorphData(binding->GetBlendHint(), nodeId, r, componentIndex, *t);

        if (componentIndex == uni::MotionTrack::Scale) {
          if (settings.animation.scaleType == ScaleType::INDEPENDENT) {
            independentScalers.emplace(nodeId, std::move(r));
            continue;
          }
        }

        gltf::Animation::Channel chan;
        chan.target.node = nodeId;
        chan.target.path = Path(componentIndex);
        chan.sampler = glanim.samplers.size();
        Sample(r, componentIndex, nodeId == rootNodeIndex);
        glanim.channels.emplace_back(std::move(chan));
        componentIndex++;
      }
    }

    if (settings.animation.scaleType == ScaleType::NONE && useScaleMotion) {
      printwarning("Scale type set to NONE, but tool detected scale motions.");
    } else if (settings.animation.scaleType != ScaleType::NONE &&
               !useScaleMotion) {
      printwarning("Scale type set, but tool did not detected scale motions.");
    }

    if (rootMotion) {
      RootMixer mixer;
      mixer.rootNode = rootTrack ? rootTrack.get() : nullptr;
      mixer.reference = rootMotion;

      size_t componentIndex = 0;
      auto stripResults = gltfutils::StripValuesBlock(
          times, upperLimit,
          rootTrack ? &mixer
                    : static_cast<const uni::MotionTrack *>(rootMotion));

      for (auto &r : stripResults) {
        gltf::Animation::Channel chan;
        chan.target.node = rootNodeIndex;
        chan.target.path = Path(componentIndex);
        chan.sampler = glanim.samplers.size();
        Sample(r, componentIndex, true);
        glanim.channels.emplace_back(std::move(chan));
        componentIndex++;
      }
    }

    if (!independentScalers.empty()) {
      bool singleFrame = true;

      for (auto &[_, r] : independentScalers) {
        if (r.values.size() > 1) {
          singleFrame = false;
          break;
        }
      }

      if (singleFrame) {
        SingleFrameIndependentScaleWalker(independentScalers, rootNodeIndex,
                                          {1, 1, 1, 0});
      } else {
        ResampledScale rootValues;
        for (size_t i = 0; i < upperLimit; i++) {
          rootValues.values.emplace(times[i], Vector4A16{1, 1, 1, 0});
        }
        IndependentScaleResampleWalker(independentScalers, rootNodeIndex,
                                       rootValues);
      }

      for (auto &[nodeId_, r] : independentScalers) {
        auto &node = nodes[nodeId_];
        const bool isControl = node.name.ends_with("_control");

        if (!isControl && !node.children.empty()) {
          printwarning("Bone " << node.name
                               << " is not contol bone, scale skipped.");
          continue;
        }

        auto nodeId = isControl ? node.children.back() : nodeId_;

        gltf::Animation::Channel chan;
        chan.target.node = nodeId;
        chan.target.path = "scale";
        chan.sampler = glanim.samplers.size();
        Sample(r, uni::MotionTrack::Scale, false);
        glanim.channels.emplace_back(std::move(chan));
      }
    }

    animations.emplace_back(std::move(glanim));
  }

  void SingleFrameIndependentScaleWalker(
      std::map<size_t, gltfutils::StripResult> &data, size_t nodeId,
      Vector4A16 nodeScale) {
    auto &node = nodes[nodeId];
    const bool isControl = node.name.ends_with("_control");

    for (auto c : node.children) {
      auto &cNode = nodes[c];

      if (isControl && c == node.children.back()) {
        break;
      }

      if (auto found = data.find(c); found != data.end()) {
        auto &val = found->second.values.front();
        val *= nodeScale;
        SingleFrameIndependentScaleWalker(data, c, val);
      } else {
        Vector4A16 childScale;
        memcpy(reinterpret_cast<char *>(&childScale), cNode.scale.data(),
               sizeof(cNode.scale));
        childScale *= nodeScale;
        gltfutils::StripResult res;
        res.timeIndices.push_back(0);
        res.values.push_back(childScale);
        data.emplace(c, std::move(res));
        SingleFrameIndependentScaleWalker(data, c, childScale);
      }
    }
  }

  gltfutils::StripResult ResampleStrip(const gltfutils::StripResult &r,
                                       size_t size) {
    gltfutils::StripResult retVal;
    size_t numItems = r.timeIndices.size() - 1;

    if (!numItems) {
      for (size_t i = 0; i < size; i++) {
        retVal.timeIndices.push_back(i);
        retVal.values.push_back(r.values.front());
      }
    } else {
      for (size_t i = 0; i < numItems; i++) {
        uint16 lowKey = r.timeIndices[i];
        uint16 hiKey = r.timeIndices[i + 1];

        retVal.timeIndices.push_back(lowKey);
        retVal.values.push_back(r.values[i]);

        if (lowKey + 1 == hiKey) {
          continue;
        }

        for (size_t k = lowKey + 1; k < hiKey; k++) {
          float delta = float(k - lowKey) / float(hiKey - lowKey);
          retVal.timeIndices.push_back(k);
          retVal.values.push_back(r.values[i] +
                                  ((r.values[i + 1] - r.values[i]) * delta));
        }
      }

      for (size_t i = r.timeIndices.back(); i < size; i++) {
        retVal.timeIndices.push_back(i);
        retVal.values.push_back(r.values.back());
      }
    }

    return retVal;
  }

  void
  IndependentScaleResampleWalker(std::map<size_t, gltfutils::StripResult> &data,
                                 size_t nodeId,
                                 const ResampledScale &nodeScale) {
    auto &node = nodes[nodeId];
    ResampledScale values;

    if (auto found = data.find(nodeId); found != data.end()) {
      auto rs = ResampleStrip(found->second, nodeScale.values.size());

      for (size_t i = 0; i < nodeScale.values.size(); i++) {
        const float time = times[rs.timeIndices[i]];
        values.values.emplace(time, rs.values[i] * nodeScale.values.at(time));
      }
      auto result =
          gltfutils::StripValues(times, nodeScale.values.size(), &values);
      found->second = std::move(result);
    } else {
      Vector4A16 childScale;
      memcpy(reinterpret_cast<char *>(&childScale), node.scale.data(),
             sizeof(node.scale));
      for (auto [time, val] : nodeScale.values) {
        values.values.emplace(time, childScale * val);
      }
      auto result =
          gltfutils::StripValues(times, nodeScale.values.size(), &values);
      data.emplace(nodeId, std::move(result));
    }

    const bool isControl = node.name.ends_with("_control");

    for (auto c : node.children) {
      if (isControl && c == node.children.back()) {
        break;
      }

      IndependentScaleResampleWalker(data, c, values);
    }
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

  void MorphData(BlendHint hint, int32 nodeId, gltfutils::StripResult &r,
                 uint32 componentIndex, const uni::MotionTrack &track) {
    if (settings.animation.blendOverride != BlendHintOverride::AUTO) {
      hint = BlendHint(int32(settings.animation.blendOverride) - 1);
    }

    if (hint == BlendHint::NORMAL) {
      return;
    }

    auto &node = nodes[nodeId];

    if (componentIndex == uni::MotionTrack::Position) {
      Vector4A16 nodeVal;
      memcpy(reinterpret_cast<char *>(&nodeVal), node.translation.data(),
             sizeof(Vector));
      if (hint == BlendHint::ADDITIVE_DEPRECATED) {
        for (size_t i = 0; i < r.timeIndices.size(); i++) {
          uni::RTSValue sample;
          track.GetValue(sample, times[r.timeIndices[i]]);
          glm::quat sampleRot{sample.rotation[3], sample.rotation[0],
                              sample.rotation[1], sample.rotation[2]};

          auto pval = sampleRot * reinterpret_cast<const glm::vec3 &>(nodeVal);
          r.values[i] += Vector4A16(pval.x, pval.y, pval.z, 0);
        }
      } else {
        glm::quat nodeRot{node.rotation[3], node.rotation[0], node.rotation[1],
                          node.rotation[2]};
        for (auto &v : r.values) {
          auto pval = nodeRot * reinterpret_cast<const glm::vec3 &>(v);
          v = nodeVal + Vector4A16(pval.x, pval.y, pval.z, 0);
        }
      }
    } else if (componentIndex == uni::MotionTrack::Scale) {
      Vector4A16 nodeVal;
      memcpy(reinterpret_cast<char *>(&nodeVal), node.scale.data(),
             sizeof(Vector));

      for (auto &v : r.values) {
        v *= nodeVal;
      }
    } else if (componentIndex == uni::MotionTrack::Rotation) {
      glm::quat nodeVal{node.rotation[3], node.rotation[0], node.rotation[1],
                        node.rotation[2]};

      for (auto &v : r.values) {
        glm::quat rVal = AsQuat(v);
        if (hint == BlendHint::ADDITIVE_DEPRECATED) {
          rVal = rVal * nodeVal;
        } else {
          rVal = nodeVal * rVal;
        }

        v = Vector4A16{rVal.x, rVal.y, rVal.z, rVal.w};
      }
    }
  }

  uint32 AddData(gltfutils::StripResult &r, uint32 componentIndex,
                 bool isRootNode) {
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

        if (isRootNode) {
          for (auto v : r.values) {
            auto rot = settings.corMat * es::Matrix44(v);
            wr.Write(Vector4A16(rot.ToQuat() * 32767).Convert<int16>());
          }
        } else {
          for (auto v : r.values) {
            wr.Write(Vector4A16(v * 32767).Convert<int16>());
          }
        }
      } else {
        dataAccess.componentType = gltf::Accessor::ComponentType::Float;
        dataAccess.type = gltf::Accessor::Type::Vec3;

        if (isRootNode) {
          if (componentIndex == uni::MotionTrack::Position) {
            for (auto v : r.values) {
              Vector4A16 val = (v * settings.sceneScale) * settings.corMat;
              wr.Write<Vector>(val);
            }
          } else {
            for (auto v : r.values) {
              wr.Write<Vector>(v * settings.corMat);
            }
          }
        } else {
          if (componentIndex == uni::MotionTrack::Position) {
            for (auto v : r.values) {
              Vector4A16 val = v * settings.sceneScale;
              wr.Write<Vector>(val);
            }
          } else {
            for (auto v : r.values) {
              wr.Write<Vector>(v);
            }
          }
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
  int32 rootNodeIndex;
};

void AppProcessFile(std::istream &stream, AppContext *ctx) {
  BinReaderRef rd(stream);
  auto hkFile = IhkPackFile::Create(rd);
  auto skeletons = hkFile->GetClasses(hkaSkeleton::GetHash());

  if (skeletons.empty()) {
    if (settings.skeleton.path.empty()) {
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

  // Animations
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

      return gltfutils::MakeSamples(settings.animation.sampleRate, maxDuration);
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
