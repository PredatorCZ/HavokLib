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

#include "datas/jenkinshash.hpp"
#include "datas/macroLoop.hpp"
#include "hkRootLevelContainer.h"
#include "hkaAnimationBinding.h"
#include "hkaAnimationContainer.h"
#include "hkaDeltaCompressedAnimation.h"
#include "hkaInterleavedAnimation.h"
#include "hkaSkeleton.h"
#include "hkaSplineCompressedAnimation.h"
#include "hkaWaveletCompressedAnimation.h"
#include "hkxEnvironment.h"
#include "hkaDefaultAnimatedReferenceFrame.h"

#include <unordered_map>

#define hkRegisterClass(classname)                                             \
  {classname::HASH, []() -> hkVirtualClass * { return new classname{}; }},
#define hkRegisterClassID(classname)                                           \
  template <> const JenHash classname::HASH = JenkinsHashC(#classname);

#define hkRegisterVersionedClassID(classname, version)                         \
  hkRegisterClassID(classname##_t<classname##version##_t<esPointerX64>>);      \
  hkRegisterClassID(classname##_t<classname##version##_t<esPointerX86>>);      \
  hkRegisterClassID(classname##_t<classname##version##_rp_t<esPointerX64>>);   \
  hkRegisterClassID(classname##_t<classname##version##_rp_t<esPointerX86>>);

// clang-format off

#define hkRegisterVersionedClass(classname, version)\
hkRegisterClass(classname##_t<classname##version##_t<esPointerX64>>)\
hkRegisterClass(classname##_t<classname##version##_t<esPointerX86>>)\
hkRegisterClass(classname##_t<classname##version##_rp_t<esPointerX64>>)\
hkRegisterClass(classname##_t<classname##version##_rp_t<esPointerX86>>)

#define hkRegisterVersionedClassEval(classname, id, version) hkRegisterVersionedClass(classname, version)

#define hkRegisterFullClassID(classname)\
hkRegisterVersionedClassID(classname, 550)\
hkRegisterVersionedClassID(classname, 660)\
hkRegisterVersionedClassID(classname, 710)\
hkRegisterVersionedClassID(classname, 2010)\
hkRegisterVersionedClassID(classname, 2011)\
hkRegisterVersionedClassID(classname, 2012)\
hkRegisterVersionedClassID(classname, 2013)\
hkRegisterVersionedClassID(classname, 2014)\
hkRegisterVersionedClassID(classname, 2015)\
hkRegisterVersionedClassID(classname, 2016)\
hkRegisterVersionedClassID(classname, 2017)

#define hkRegisterFullClass(classname)\
hkRegisterVersionedClass(classname, 550)\
hkRegisterVersionedClass(classname, 660)\
hkRegisterVersionedClass(classname, 710)\
hkRegisterVersionedClass(classname, 2010)\
hkRegisterVersionedClass(classname, 2011)\
hkRegisterVersionedClass(classname, 2012)\
hkRegisterVersionedClass(classname, 2013)\
hkRegisterVersionedClass(classname, 2014)\
hkRegisterVersionedClass(classname, 2015)\
hkRegisterVersionedClass(classname, 2016)\
hkRegisterVersionedClass(classname, 2017)

hkRegisterFullClassID(hkaSkeleton)
hkRegisterFullClassID(hkRootLevelContainer)
hkRegisterFullClassID(hkaAnimationContainer)
hkRegisterFullClassID(hkaAnimationBinding)


hkRegisterVersionedClassID(hkaDeltaCompressedSkeletalAnimation, 550)
hkRegisterVersionedClassID(hkaDeltaCompressedAnimation, 660)
hkRegisterVersionedClassID(hkaDeltaCompressedAnimation, 710)
hkRegisterVersionedClassID(hkaDeltaCompressedAnimation, 2010)

hkRegisterVersionedClassID(hkaSplineSkeletalAnimation, 550)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 660)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 710)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 2010)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 2011)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 2012)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 2013)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 2014)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 2015)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 2016)
hkRegisterVersionedClassID(hkaSplineCompressedAnimation, 2017)

hkRegisterVersionedClassID(hkaWaveletCompressedSkeletalAnimation, 550)
hkRegisterVersionedClassID(hkaWaveletCompressedAnimation, 660)
hkRegisterVersionedClassID(hkaWaveletCompressedAnimation, 710)
hkRegisterVersionedClassID(hkaWaveletCompressedAnimation, 2010)

hkRegisterVersionedClassID(hkaInterleavedSkeletalAnimation, 550)
hkRegisterVersionedClassID(hkaInterleavedUncompressedAnimation, 660)
hkRegisterVersionedClassID(hkaInterleavedUncompressedAnimation, 710)
hkRegisterVersionedClassID(hkaInterleavedUncompressedAnimation, 2010)
hkRegisterVersionedClassID(hkaInterleavedUncompressedAnimation, 2011)
hkRegisterVersionedClassID(hkaInterleavedUncompressedAnimation, 2012)
hkRegisterVersionedClassID(hkaInterleavedUncompressedAnimation, 2013)
hkRegisterVersionedClassID(hkaInterleavedUncompressedAnimation, 2014)
hkRegisterVersionedClassID(hkaInterleavedUncompressedAnimation, 2015)

hkRegisterVersionedClassID(hkxEnvironment, 550)
hkRegisterVersionedClassID(hkxEnvironment, 660)
hkRegisterVersionedClassID(hkxEnvironment, 710)
hkRegisterVersionedClassID(hkxEnvironment, 2010)
hkRegisterVersionedClassID(hkxEnvironment, 2011)
hkRegisterVersionedClassID(hkxEnvironment, 2012)
hkRegisterVersionedClassID(hkxEnvironment, 2013)
hkRegisterVersionedClassID(hkxEnvironment, 2014)

hkRegisterVersionedClassID(hkaDefaultAnimatedReferenceFrame, 550)
hkRegisterVersionedClassID(hkaDefaultAnimatedReferenceFrame, 660)
hkRegisterVersionedClassID(hkaDefaultAnimatedReferenceFrame, 710)
hkRegisterVersionedClassID(hkaDefaultAnimatedReferenceFrame, 2010)
hkRegisterVersionedClassID(hkaDefaultAnimatedReferenceFrame, 2011)
hkRegisterVersionedClassID(hkaDefaultAnimatedReferenceFrame, 2012)
hkRegisterVersionedClassID(hkaDefaultAnimatedReferenceFrame, 2013)
hkRegisterVersionedClassID(hkaDefaultAnimatedReferenceFrame, 2014)

static const std::unordered_map<JenHash, hkVirtualClass *(*)()> hkClassStorage =
{
  StaticFor(hkRegisterFullClass, hkaSkeleton, hkRootLevelContainer, hkaAnimationContainer, hkaAnimationBinding)

  hkRegisterVersionedClass(hkaDeltaCompressedSkeletalAnimation, 550)
  StaticForArgID(hkRegisterVersionedClassEval, hkaDeltaCompressedAnimation, 660, 710, 2010)

  hkRegisterVersionedClass(hkaWaveletCompressedSkeletalAnimation, 550)
  StaticForArgID(hkRegisterVersionedClassEval, hkaWaveletCompressedAnimation, 660, 710, 2010)

  hkRegisterVersionedClass(hkaInterleavedSkeletalAnimation, 550)
  StaticForArgID(hkRegisterVersionedClassEval, hkaInterleavedUncompressedAnimation, 660, 710, 2010, 2011, 2012, 2013, 2014, 2015)

  StaticForArgID(hkRegisterVersionedClassEval, hkxEnvironment, 550, 660, 710, 2010, 2011, 2012, 2013, 2014)

  hkRegisterVersionedClass(hkaSplineSkeletalAnimation, 550)
  StaticForArgID(hkRegisterVersionedClassEval, hkaSplineCompressedAnimation, 660, 710, 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017)

  StaticForArgID(hkRegisterVersionedClassEval, hkaDefaultAnimatedReferenceFrame, 660, 710, 2010, 2011, 2012, 2013, 2014)
};

// clang-format on

hkVirtualClass *IhkPackFile::ConstructClass(JenHash hash) {
  auto found = hkClassStorage.find(hash);

  if (found == hkClassStorage.end())
    return nullptr;

  return found->second();
}