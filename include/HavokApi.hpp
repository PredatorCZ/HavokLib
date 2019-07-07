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
	class xml_document;
};
typedef pugi::xml_node XMLnode;

struct XMLHandle
{
	XMLnode *node;
	hkXMLToolsets toolset;
};

struct IhkVirtualClass 
{
	virtual const void *GetPointer() const = 0;
	virtual ~IhkVirtualClass() {}
};

__declspec(align(16))struct hkQTransform
{
	Vector4 position;
	Vector4 rotation;
	Vector4 scale;

	std::string ToString() const;

	hkQTransform() : position(0.0f, 0.0f, 0.0f, 1.0f), rotation(0.0f, 0.0f, 0.0f, 1.0f), scale(1.0f, 1.0f, 1.0f, 0.0f) {}
	hkQTransform(Vector4 pos, Vector4 rot, Vector4 scl) : position(pos), rotation(rot), scale(scl) {}
	hkQTransform(Vector pos, Vector4 rot, Vector scl) : position(pos.X, pos.Y, pos.Z, 1.0f), rotation(rot), scale(scl.X, scl.Y, scl.Z, 0.0f) {}

	bool operator== (const hkQTransform &o) const
	{
		return reinterpret_cast<const Vector &>(position) == reinterpret_cast<const Vector &>(o.position) && rotation == o.rotation && scale == o.scale;
	}
	bool operator!= (const hkQTransform &o) const { return !(*this == o); }
};

struct hkLocalFrameOnBone
{
	const hkLocalFrame *localFrame;
	int boneIndex;
};

struct hkaPartition
{
	DECLARE_REFLECTOR;
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

//ADDITIVE_DEPRECATED since 2010
REFLECTOR_ENUM_NAKED(BlendHint, NORMAL, ADDITIVE_DEPRECATED, ADDITIVE)

struct hkaAnimationBinding : IhkVirtualClass
{
	DECLARE_HKCLASS(hkaAnimationBinding)

	virtual const char *GetSkeletonName() const = 0;
	virtual const hkaAnimation *GetAnimation() const = 0;
	virtual BlendHint GetBlendHint() const = 0;
	virtual const int GetNumTransformTrackToBoneIndices() const = 0;
	virtual const short GetTransformTrackToBoneIndex(int id) const = 0;
	virtual const int GetNumFloatTrackToFloatSlotIndices() const = 0;
	virtual const short GetFloatTrackToFloatSlotIndex(int id) const = 0;
	virtual const int GetNumPartitionIndices() const = 0;
	virtual const short GetPartitionIndex(int id) const = 0;

	typedef hkIterProxy<hkaAnimationBinding, &GetNumTransformTrackToBoneIndices, const short, &GetTransformTrackToBoneIndex> iteratorTransformTrackToBoneIndices;
	typedef hkIterProxy<hkaAnimationBinding, &GetNumFloatTrackToFloatSlotIndices, const short, &GetFloatTrackToFloatSlotIndex> iteratorFloatTrackToFloatSlotIndices;
	typedef hkIterProxy<hkaAnimationBinding, &GetNumPartitionIndices, const short, &GetPartitionIndex> iteratorNumPartitionIndices;

	const iteratorTransformTrackToBoneIndices TransformTrackToBoneIndices() const { return iteratorTransformTrackToBoneIndices(this); }
	const iteratorFloatTrackToFloatSlotIndices FloatTrackToFloatSlotIndices() const { return iteratorFloatTrackToFloatSlotIndices(this); }
	const iteratorNumPartitionIndices PartitionIndices() const { return iteratorNumPartitionIndices(this); }
};

REFLECTOR_ENUM_NAKED(hkaAnimationType, 
	HK_UNKNOWN_ANIMATION,
	HK_INTERLEAVED_ANIMATION,
	HK_DELTA_COMPRESSED_ANIMATION,
	HK_WAVELET_COMPRESSED_ANIMATION,
	HK_MIRRORED_ANIMATION,
	HK_SPLINE_COMPRESSED_ANIMATION,
	HK_QUANTIZED_COMPRESSED_ANIMATION,
	HK_PREDICTIVE_COMPRESSED_ANIMATION,
	HK_REFERENCE_POSE_ANIMATION
);

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

template<class C>
struct _hkHybridPtr
{
	C *ptr;
	mutable bool created;

	ES_FORCEINLINE C *get() { return ptr; }
	_hkHybridPtr(C *in, bool made = true) : ptr(in), created(made) {}
	_hkHybridPtr() = delete;
	void operator=(const _hkHybridPtr &ref) = delete;
	_hkHybridPtr(const _hkHybridPtr &ref)
	{
		ptr = ref.ptr;
		created = ref.created;
		ref.created = false;
	}
	~_hkHybridPtr()
	{
		if (created)
			delete ptr;
	}

};

typedef _hkHybridPtr<hkaAnnotationTrack> hkaAnnotationTrackPtr;
struct hkaAnimation : IhkVirtualClass
{
	DECLARE_HKCLASS(hkaAnimation)

	enum TrackType
	{
		Rotation,
		Position,
		Scale
	};

	virtual const char *GetAnimationTypeName() const = 0;
	virtual const hkaAnimationType GetAnimationType() const = 0;
	virtual const float GetDuration() const = 0;
	virtual const int GetNumOfTransformTracks() const = 0;
	virtual const int GetNumOfFloatTracks() const = 0;
	virtual const hkaAnimatedReferenceFrame *GetExtractedMotion() const = 0;
	virtual const int GetNumAnnotations() const = 0;
	virtual hkaAnnotationTrackPtr GetAnnotation(int id) const = 0;
	virtual bool IsDecoderSupported() const { return false; }
	virtual bool IsTrackStatic(int trackID, TrackType type) const = 0;
	
private:
	virtual void GetTrack(int trackID, int frame, float delta, TrackType type, Vector4 &out) const = 0;
	virtual void GetTransform(int trackID, int frame, float delta, hkQTransform &out) const = 0;
	static Vector4 Lerp(Vector4 v1, Vector4 v2, float delta)
	{
		return v1 + (v2 - v1) * delta;
	}
	virtual int GetNumInternalFrames() const = 0;
	mutable int numFrames;
	mutable float frameRate;
public:
	void GetTransform(int trackID, float time, hkQTransform &out) const
	{
		int frame;
		float delta;
		GetFrameDelta(time, frame, delta);

		if (delta > 0.0f && GetAnimationType() != HK_SPLINE_COMPRESSED_ANIMATION)
		{
			hkQTransform start;
			GetTransform(trackID, frame++, 0.0f, start);
			hkQTransform end;
			GetTransform(trackID, frame, 0.0f, end);
			
			out.position = Lerp(start.position, end.position, delta);
			out.rotation = Lerp(start.rotation, end.rotation, delta);
			out.scale = Lerp(start.scale, end.scale, delta);
		}
		else
		{
			GetTransform(trackID, frame, delta, out);
		}
	}

	void ComputeFrameRate() const
	{
		numFrames = GetNumInternalFrames() - 1;
		frameRate = numFrames / GetDuration();
	}

	ES_INLINE void GetFrameDelta(float time, int &frame, float &delta) const
	{
		const float frameFull = time * frameRate;
		frame = static_cast<int>(frameFull);

		if (frame > numFrames)
			frame--;

		delta = frameFull >= GetDuration() * frameRate ? 0.0f : frameFull - frame;
	}

	typedef hkIterProxy<hkaAnimation, &GetNumAnnotations, hkaAnnotationTrackPtr, &GetAnnotation> interatorAnnotation;

	const interatorAnnotation Annotations() const { return interatorAnnotation(this); }
};

struct hkxEnvironmentVariable
{
	const char *name;
	const char *value;
};

struct hkxEnvironment : IhkVirtualClass
{
	DECLARE_HKCLASS(hkxEnvironment)

	virtual const int GetNumVars() const = 0;
	virtual hkxEnvironmentVariable GetVar(int id) const = 0;

	typedef hkInter<hkxEnvironment, &GetNumVars, hkxEnvironmentVariable, &GetVar> interator;

	const interator begin() const { return interator(this, 0); }
	const interator end() const { return interator(this); }
};

struct hkaAnimatedReferenceFrame : IhkVirtualClass {};
struct hkaBoneAttachment : IhkVirtualClass {};
struct hkaMeshBinding : IhkVirtualClass {};

struct IhkPackFile
{
	typedef std::vector<IhkVirtualClass *> VirtualClasses;
private:
	template<class _Ty>
	static IhkPackFile *_Create(const _Ty *fileName, bool suppressErrors);

	template<class _Ty>
	int _ExportXML(const _Ty *fileName, hkXMLToolsets toolsetVersion);

	void _GenerateXML(pugi::xml_document &doc, hkXMLToolsets toolsetVersion);
public:
	virtual VirtualClasses &GetAllClasses() = 0;
	virtual int GetVersion() = 0;
	virtual ~IhkPackFile() = 0;

	ES_INLINE VirtualClasses GetClasses(const char *hkClassName) { return GetClasses(JenkinsHash(hkClassName, static_cast<int>(strlen(hkClassName)))); }
	ES_INLINE hkRootLevelContainer *GetRootLevelContainer() { return dynamic_cast<hkRootLevelContainer *>(GetClasses(hkRootLevelContainer::HASH)[0]); }
	VirtualClasses GetClasses(JenHash hash);
	const IhkVirtualClass *GetClass(const void *ptr);

	ES_FORCEINLINE static IhkPackFile *Create(const wchar_t *fileName, bool suppressErrors = false) { return _Create(fileName, suppressErrors); }
	ES_FORCEINLINE static IhkPackFile *Create(const char *fileName, bool suppressErrors = false) { return _Create(fileName, suppressErrors); }

	ES_FORCEINLINE int ExportXML(const wchar_t *fileName, hkXMLToolsets toolsetVersion) { return _ExportXML(fileName, toolsetVersion); }
	ES_FORCEINLINE int ExportXML(const char *fileName, hkXMLToolsets toolsetVersion) { return _ExportXML(fileName, toolsetVersion); }

	//Internal use only
	static hkVirtualClass *ConstructClass(JenHash hash);
};
