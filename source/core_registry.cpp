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

#include "internal/custom/hka_losslesscompressedanimation.hpp"
#include "internal/hk_rootlevelcontainer.hpp"
#include "internal/hka_animationbinding.hpp"
#include "internal/hka_animationcontainer.hpp"
#include "internal/hka_annotationtrack.hpp"
#include "internal/hka_defaultanimrefframe.hpp"
#include "internal/hka_deltaanimation.hpp"
#include "internal/hka_interleavedanimation.hpp"
#include "internal/hka_skeleton.hpp"
#include "internal/hka_splineanimation.hpp"
#include "internal/hka_waveletanimation.hpp"
#include "internal/hkx_environment.hpp"
#include <map>

#define hkRegisterCreator(cname)                                               \
  {JenHash(#cname), cname##InternalInterface::Create},
#define hkCreatorAlias(cname, aliasName)                                       \
  { JenHash(#aliasName), cname##InternalInterface::Create }

static const std::map<JenHash, hkVirtualClass *(*)(CRule)> hkConstrRegistry{
    hkCreatorAlias(hkaInterleavedAnimation, hkaInterleavedSkeletalAnimation),
    hkCreatorAlias(hkaInterleavedAnimation,
                   hkaInterleavedUncompressedAnimation),
    hkCreatorAlias(hkaSplineCompressedAnimation, hkaSplineSkeletalAnimation),
    hkCreatorAlias(hkaDeltaCompressedAnimation,
                   hkaDeltaCompressedSkeletalAnimation),
    hkCreatorAlias(hkaWaveletCompressedAnimation,
                   hkaWaveletCompressedSkeletalAnimation),
    StaticFor(hkRegisterCreator, hkRootLevelContainer, hkxEnvironment,
              hkaSkeleton, hkaSplineCompressedAnimation,
              hkaDeltaCompressedAnimation, hkaWaveletCompressedAnimation,
              hkaAnimationContainer, hkaDefaultAnimatedReferenceFrame,
              hkaAnimationBinding, hkaLosslessCompressedAnimation)};

hkVirtualClass *hkVirtualClass::Create(JenHash hash, CRule rule) {
  auto found = hkConstrRegistry.find(hash);

  if (es::IsEnd(hkConstrRegistry, found)) {
    return nullptr;
  }

  auto madeClass = found->second(rule);

  if (madeClass) {
    madeClass->rule = rule;
  }

  return madeClass;
}

REFLECT(CLASS(hkaPartition), MEMBER(name), MEMBER(startBoneIndex),
        MEMBER(numBones));
