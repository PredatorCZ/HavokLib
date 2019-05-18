/*	Havok Format Library
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

#include "hkaSkeleton.h"
#include "hkRootLevelContainer.h"
#include "hkaAnimationContainer.h"
#include "datas/macroLoop.hpp"
#include "datas/jenkinshash.hpp"
#include "hkaDeltaCompressedAnimation.h"
#include "hkaWaveletCompressedAnimation.h"
#include "hkaInterleavedAnimation.h"
#include "hkaAnimationBinding.h"
#include "hkxEnvironment.h"

#define hkRegisterClass(classname) { classname::HASH, &hkCreateDerivedClass<classname> },
#define hkRegisterClassID(classname)const JenHash classname::HASH = JenkinsHash(#classname, sizeof(#classname) - 1);

#define hkRegisterVersionedClassID(classname, version)\
hkRegisterClassID(classname##_t<##classname##version##_t<hkRealPointer>>);\
hkRegisterClassID(classname##_t<##classname##version##_t<hkFakePointer>>);\
hkRegisterClassID(classname##_t<##classname##version##_t<hkStripPointer>>);\
hkRegisterClassID(classname##_t<##classname##version##_rp_t<hkRealPointer>>);\
hkRegisterClassID(classname##_t<##classname##version##_rp_t<hkFakePointer>>);\
hkRegisterClassID(classname##_t<##classname##version##_rp_t<hkStripPointer>>);\

#define hkRegisterVersionedClass(classname, version)\
hkRegisterClass(classname##_t<##classname##version##_t<hkRealPointer>>)\
hkRegisterClass(classname##_t<##classname##version##_t<hkFakePointer>>)\
hkRegisterClass(classname##_t<##classname##version##_t<hkStripPointer>>)\
hkRegisterClass(classname##_t<##classname##version##_rp_t<hkRealPointer>>)\
hkRegisterClass(classname##_t<##classname##version##_rp_t<hkFakePointer>>)\
hkRegisterClass(classname##_t<##classname##version##_rp_t<hkStripPointer>>)\

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
hkRegisterVersionedClassID(classname, 2016)

#define hkRegisterFullClass(classname)\
hkRegisterVersionedClass(classname, 550)\
hkRegisterVersionedClass(classname, 660)\
hkRegisterVersionedClass(classname, 710)\
hkRegisterVersionedClass(classname, 2010)\
hkRegisterVersionedClass(classname, 2011)\
hkRegisterVersionedClass(classname, 2012)\
hkRegisterVersionedClass(classname, 2013)\
hkRegisterVersionedClass(classname, 2014)\
hkRegisterVersionedClass(classname, 2016)

hkRegisterFullClassID(hkaSkeleton)
hkRegisterFullClassID(hkRootLevelContainer)
hkRegisterFullClassID(hkaAnimationContainer)

hkRegisterVersionedClassID(hkaDeltaCompressedSkeletalAnimation, 550)
hkRegisterVersionedClassID(hkaDeltaCompressedAnimation, 660)
hkRegisterVersionedClassID(hkaDeltaCompressedAnimation, 710)
hkRegisterVersionedClassID(hkaDeltaCompressedAnimation, 2010)

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

hkRegisterVersionedClassID(hkaAnimationBinding, 550)
hkRegisterVersionedClassID(hkaAnimationBinding, 660)
hkRegisterVersionedClassID(hkaAnimationBinding, 710)
hkRegisterVersionedClassID(hkaAnimationBinding, 2010)
hkRegisterVersionedClassID(hkaAnimationBinding, 2011)
hkRegisterVersionedClassID(hkaAnimationBinding, 2012)
hkRegisterVersionedClassID(hkaAnimationBinding, 2013)
hkRegisterVersionedClassID(hkaAnimationBinding, 2014)

hkRegisterVersionedClassID(hkxEnvironment, 550)
hkRegisterVersionedClassID(hkxEnvironment, 660)
hkRegisterVersionedClassID(hkxEnvironment, 710)
hkRegisterVersionedClassID(hkxEnvironment, 2010)
hkRegisterVersionedClassID(hkxEnvironment, 2011)
hkRegisterVersionedClassID(hkxEnvironment, 2012)
hkRegisterVersionedClassID(hkxEnvironment, 2013)
hkRegisterVersionedClassID(hkxEnvironment, 2014)

template<class C> hkVirtualClass *hkCreateDerivedClass() { return new C{}; }

static const std::map<JenHash, hkVirtualClass *(*)()> hkClassStorage =
{
	StaticFor(hkRegisterFullClass, hkaSkeleton, hkRootLevelContainer, hkaAnimationContainer)

	hkRegisterVersionedClass(hkaDeltaCompressedSkeletalAnimation, 550)
	StaticForArgID(hkRegisterVersionedClassEval, hkaDeltaCompressedAnimation, 660, 710, 2010)

	hkRegisterVersionedClass(hkaWaveletCompressedSkeletalAnimation, 550)
	StaticForArgID(hkRegisterVersionedClassEval, hkaWaveletCompressedAnimation, 660, 710, 2010)

	hkRegisterVersionedClass(hkaInterleavedSkeletalAnimation, 550)
	StaticForArgID(hkRegisterVersionedClassEval, hkaInterleavedUncompressedAnimation, 660, 710, 2010, 2011, 2012, 2013, 2014)

	StaticForArgID(hkRegisterVersionedClassEval, hkaAnimationBinding, 550, 660, 710, 2010, 2011, 2012, 2013, 2014)
	StaticForArgID(hkRegisterVersionedClassEval, hkxEnvironment, 550, 660, 710, 2010, 2011, 2012, 2013, 2014)
};

hkVirtualClass *IhkPackFile::ConstructClass(JenHash hash)
{
	if (hkClassStorage.count(hash))
		return hkClassStorage.at(hash)();

	return nullptr;
}