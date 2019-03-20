/*  Havok Format Library
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

#pragma once
#include <vector>
#include <functional>
#include "datas/endian.hpp"
#include "datas/vectors.hpp"
#include "datas/jenkinshash.hpp"
#include "datas/reflector.hpp"

class BinReader;
struct IhkPackFile;
struct hkLocalFrame;
struct hkaAnimation;
struct hkaAnimationBinding;
struct hkaBoneAttachment;
struct hkaMeshBinding;
struct hkaAnimatedReferenceFrame;
struct hkaAnnotationTrack;
struct hkRootLevelContainer;
struct hkVirtualClass;

#define DECLARE_HKCLASS(classname) static const JenHash HASH = JenkinsHash(#classname, sizeof(#classname) - 1);

////////////////////////////////////////////////////////////////////////// Havok Iterators

template<
	class containerClass,
	const int (containerClass::*Counter)()const,
	class returnType,
	returnType(containerClass::*Accessor)(int)const
>
class hkInter
{
	const containerClass *tClass;
	int iterPos;
public:
	explicit hkInter(const containerClass *cls, int _num = -1) :
		iterPos(_num == -1 ? std::bind(Counter, tClass)() : _num),
		tClass(cls) {}

	hkInter &operator++() { iterPos++; return *this; }
	hkInter operator++(int) { hkInter retval = *this; ++(*this); return retval; }
	bool operator==(hkInter input) const { return iterPos == input.iterPos; }
	bool operator!=(hkInter input) const { return iterPos != input.iterPos; }

	template<class ptrTest = returnType> typename std::enable_if<
		std::is_pointer<ptrTest>::value,
		typename std::remove_pointer<returnType>::type &
	>::type
		operator*() const { return *std::bind(Accessor, tClass, iterPos)(); }

	template<class ptrTest = returnType> typename std::enable_if<!std::is_pointer<ptrTest>::value, returnType>::type
		operator*() const { return std::bind(Accessor, tClass, iterPos)(); }
};

template<
	class containerClass,
	const int (containerClass:: *Counter)()const,
	class returnType,
	returnType(containerClass:: *Accessor)(int)const
>
class hkIterProxy
{
	typedef hkInter<containerClass, Counter, returnType, Accessor> _iter;

	const containerClass *_clsPtr;
public:
	hkIterProxy(const containerClass *item) : _clsPtr(item) {}
	const _iter begin() const { return _iter(_clsPtr, 0); }
	const _iter end() const { return _iter(_clsPtr); }
};

////////////////////////////////////////////////////////////////////////// Havok Interface Classes

enum hkXMLToolsets
{
	HKUNKVER,
	HK550,
	HK660,
	HK710,
	HK2010_2,
	HK2011,
	HK2011_2,
	HK2012_2,
	HK2013,
	HK2014,
};

namespace pugi
{
	class xml_node;
};
typedef pugi::xml_node XMLnode;

struct XMLHandle
{
	XMLnode *node;
	hkXMLToolsets toolset;
};

struct IhkVirtualClass 
{
	virtual ~IhkVirtualClass() {}
};

#pragma warning(disable : 4324)
__declspec(align(16))struct hkQTransform
{
	Vector position;
	__declspec(align(16)) Vector4 rotation;
	Vector scale;

	std::string ToString() const;

	hkQTransform() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f, 1.0f), scale(1.0f, 1.0f, 1.0f) {}
	hkQTransform(Vector pos, Vector4 rot, Vector scl) : position(pos), rotation(rot), scale(scl) {}

	bool operator== (const hkQTransform &o) const
	{
		return position == o.position && rotation == o.rotation && scale == o.scale;
	}
	bool operator!= (const hkQTransform &o) const { return !(*this == o); }
};
#pragma warning(default : 4324)

struct hkLocalFrameOnBone
{
	const hkLocalFrame *localFrame;
	int boneIndex;
};

struct hkaPartition
{
	DECLARE_REFLECTOR_WNAMES_STATIC;
	std::string name;
	short startBoneIndex,
		numBones;
};

struct hkFullBone
{
	const char *name;
	short parentID;
	const hkQTransform *transform;
	operator const char *() const { return name; }
};

struct hkaSkeleton : IhkVirtualClass
{
	DECLARE_HKCLASS(hkaSkeleton)
	virtual const char *GetSkeletonName() const = 0;
	virtual const int GetNumFloatSlots() const = 0;
	virtual const char *GetFloatSlot(int id) const = 0;
	virtual const int GetNumLocalFrames() const = 0;
	virtual const hkLocalFrameOnBone GetLocalFrame(int id) const = 0;
	virtual const int GetNumPartitions() const = 0;
	virtual const hkaPartition GetPartition(int id) const = 0;
	virtual const int GetNumBones() const = 0;
	virtual const char *GetBoneName(int id) const = 0;
	virtual const hkQTransform *GetBoneTM(int id) const = 0;
	virtual const short GetBoneParentID(int id) const = 0;
	virtual const int GetNumReferenceFloats() const = 0;
	virtual const float GetReferenceFloat(int id) const = 0;

	operator const char *() const { return GetSkeletonName(); }
	hkFullBone GetFullBone(int id) const { return hkFullBone{ GetBoneName(id), GetBoneParentID(id), GetBoneTM(id) }; }

	typedef hkIterProxy<hkaSkeleton, &GetNumFloatSlots, const char *, &GetFloatSlot> iteratorFloatSlots;
	typedef hkIterProxy<hkaSkeleton, &GetNumLocalFrames, const hkLocalFrameOnBone, &GetLocalFrame> iteratorLocalFrames;
	typedef hkIterProxy<hkaSkeleton, &GetNumPartitions, const hkaPartition, &GetPartition> iteratorPartitions;
	typedef hkIterProxy<hkaSkeleton, &GetNumReferenceFloats, const float, &GetReferenceFloat> iteratorReferenceFloat;
	typedef hkIterProxy<hkaSkeleton, &GetNumBones, const char *, &GetBoneName> iteratorBoneNames;
	typedef hkIterProxy<hkaSkeleton, &GetNumBones, const hkQTransform *, &GetBoneTM> iteratorBoneTMs;
	typedef hkIterProxy<hkaSkeleton, &GetNumBones, const short, &GetBoneParentID> iteratorBoneParentIDs;
	typedef hkIterProxy<hkaSkeleton, &GetNumBones, hkFullBone, &GetFullBone> iteratorFullBones;

	const iteratorFloatSlots FloatSlots() const { return iteratorFloatSlots(this); }
	const iteratorLocalFrames LocalFrames() const { return iteratorLocalFrames(this); }
	const iteratorPartitions Partitions() const { return iteratorPartitions(this); }
	const iteratorReferenceFloat ReferenceFloats() const { return iteratorReferenceFloat(this); }
	const iteratorBoneNames BoneNames() const { return iteratorBoneNames(this); }
	const iteratorBoneTMs BoneTransforms() const { return iteratorBoneTMs(this); }
	const iteratorBoneParentIDs BoneParentIDs() const { return iteratorBoneParentIDs(this); }
	const iteratorFullBones FullBones() const { return iteratorFullBones(this); }
};

struct hkNamedVariant
{
	const char *name;
	const char *className;
	const IhkVirtualClass *pointer;

	template<class C> operator const C *() const { return dynamic_cast<const C *>(pointer); }
	operator const char *() const { return name; }
	operator JenHash() const { return JenkinsHash(className, static_cast<const int>(strlen(className))); }
	bool operator==(const JenHash iHash) const { return static_cast<JenHash>(*this) == iHash; }

	void ToXML(XMLHandle hdl) const;
};

struct hkRootLevelContainer : IhkVirtualClass
{
	DECLARE_HKCLASS(hkRootLevelContainer)

	virtual const int GetNumVariants() const = 0;
	virtual const hkNamedVariant GetVariant(int id) const = 0;

	typedef hkInter<hkRootLevelContainer, &GetNumVariants, const hkNamedVariant, &GetVariant> interatorVariant;

	const interatorVariant begin() const { return interatorVariant(this, 0); }
	const interatorVariant end() const { return interatorVariant(this); }

};

struct hkaAnimationContainer : IhkVirtualClass
{
	DECLARE_HKCLASS(hkaAnimationContainer)

	virtual const int GetNumSkeletons() const = 0;
	virtual const hkaSkeleton *GetSkeleton(int id) const = 0;
	virtual const int GetNumAnimations() const = 0;
	virtual const hkaAnimation *GetAnimation(int id) const = 0;
	virtual const int GetNumBindings() const = 0;
	virtual const hkaAnimationBinding *GetBinding(int id) const = 0;
	virtual const int GetNumAttachments() const = 0;
	virtual const hkaBoneAttachment *GetAttachment(int id) const = 0;
	virtual const int GetNumSkins() const = 0;
	virtual const hkaMeshBinding *GetSkin(int id) const = 0;

	typedef hkIterProxy<hkaAnimationContainer, &GetNumSkeletons, const hkaSkeleton *, &GetSkeleton> iteratorSkeletons;
	typedef hkIterProxy<hkaAnimationContainer, &GetNumAnimations, const hkaAnimation *, &GetAnimation> iteratorAnimations;
	typedef hkIterProxy<hkaAnimationContainer, &GetNumBindings, const hkaAnimationBinding *, &GetBinding> iteratorBindings;
	typedef hkIterProxy<hkaAnimationContainer, &GetNumAttachments, const hkaBoneAttachment *, &GetAttachment> iteratorAttachments;
	typedef hkIterProxy<hkaAnimationContainer, &GetNumSkins, const hkaMeshBinding *, &GetSkin> iteratorMeshBinds;

	const iteratorSkeletons Skeletons() const { return iteratorSkeletons(this); }
	const iteratorAnimations Animations() const { return iteratorAnimations(this); }
	const iteratorBindings Bindings() const { return iteratorBindings(this); }
	const iteratorAttachments Attachments() const { return iteratorAttachments(this); }
	const iteratorMeshBinds MeshBinds() const { return iteratorMeshBinds(this); }
};

enum hkaAnimationType
{
	UNKNOWN_ANIMATION,
	INTERLEAVED_ANIMATION,
	DELTA_COMPRESSED_ANIMATION,
	WAVELET_COMPRESSED_ANIMATION,
	MIRRORED_ANIMATION,
	SPLINE_COMPRESSED_ANIMATION,
	QUANTIZED_COMPRESSED_ANIMATION,
	PREDICTIVE_COMPRESSED_ANIMATION,
	REFERENCE_POSE_ANIMATION
};

struct hkaAnnotationTrack : IhkVirtualClass
{
	DECLARE_HKCLASS(hkaAnnotationTrack)

	struct Annotation
	{
		float time;
		const char *text;
	};
	virtual const char *GetName() const = 0;
	virtual const int GetNumAnnotations() const = 0;
	virtual Annotation GetAnnotation(int id) const = 0;

	typedef hkInter<hkaAnnotationTrack, &GetNumAnnotations, Annotation, &GetAnnotation> interatorAnnotation;

	const interatorAnnotation begin() const { return interatorAnnotation(this, 0); }
	const interatorAnnotation end() const { return interatorAnnotation(this); }

	operator const char *() const { return GetName(); }
};

typedef std::auto_ptr<hkaAnnotationTrack> hkaAnnotationTrackPtr;
struct hkaAnimation : IhkVirtualClass
{
	DECLARE_HKCLASS(hkaAnimation)

	virtual const char *GetAnimationTypeName() const = 0;
	virtual const hkaAnimationType GetAnimationType() const = 0;
	virtual const float GetDuration() const = 0;
	virtual const int GetNumOfTransformTracks() const = 0;
	virtual const int GetNumOfFloatTracks() const = 0;
	virtual const hkaAnimatedReferenceFrame *GetExtractedMotion() const = 0;
	virtual const int GetNumAnnotations() const = 0;
	virtual hkaAnnotationTrackPtr GetAnnotation(int id) const = 0;
};


struct hkaAnimatedReferenceFrame : IhkVirtualClass {};
struct hkaAnimationBinding : IhkVirtualClass {};
struct hkaBoneAttachment : IhkVirtualClass {};
struct hkaMeshBinding : IhkVirtualClass {};

struct IhkPackFile
{
	typedef std::vector<IhkVirtualClass *> VirtualClasses;
	virtual VirtualClasses &GetAllClasses() = 0;
	virtual int GetVersion() = 0;
	virtual ~IhkPackFile() = 0;

	ES_INLINE VirtualClasses GetClasses(const char *hkClassName) { return GetClasses(JenkinsHash(hkClassName, static_cast<int>(strlen(hkClassName)))); }
	ES_INLINE hkRootLevelContainer *GetRootLevelContainer() { return dynamic_cast<hkRootLevelContainer *>(GetClasses(hkRootLevelContainer::HASH)[0]); }
	VirtualClasses GetClasses(JenHash hash);
	const IhkVirtualClass *GetClass(const void *ptr);

	static IhkPackFile *Create(const wchar_t *fileName);
	int ExportXML(const wchar_t *fileName, hkXMLToolsets toolsetVersion); 

	//Internal use only
	static hkVirtualClass *ConstructClass(JenHash hash);
};
