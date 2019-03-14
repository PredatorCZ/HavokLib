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

#pragma once
#include "hkInternalInterfaces.h"

#define DECLARE_XMLCLASS(classname, parent) DECLARE_HKCLASS(classname)\
void SwapEndian() {}\
void *GetPointer() { return nullptr; };\
void Process() {}\
void SetDataPointer(void *Ptr) {}\
public:\
classname()\
{\
	hash = classname::HASH;\
	superHash = parent::HASH;\
	namePtr = #parent;\
}

class xmlHavokFile : public IhkPackFile
{
	VirtualClasses classes;
	int GetVersion() { return -1; }
public:
	VirtualClasses &GetAllClasses() { return classes; }
	template <class C> C *NewClass() 
	{ 
		C *cls = new C();
		classes.push_back(cls);
		return cls;
	}
	~xmlHavokFile();
};

class xmlRootLevelContainer : public hkRootLevelContainerInternalInterface
{
	DECLARE_XMLCLASS(xmlRootLevelContainer, hkRootLevelContainer)

	const int GetNumVariants() const { return static_cast<int>(variants.size()); }
	const hkNamedVariant GetVariant(int id) const { return variants.at(id); }
	void AddVariant(hkVirtualClass *input) { variants.push_back({ input->namePtr, input->namePtr, input }); }
private:
	std::vector<hkNamedVariant> variants;
};

class xmlAnimationContainer : public hkaAnimationContainerInternalInterface
{
	DECLARE_XMLCLASS(xmlAnimationContainer, hkaAnimationContainer)

	const int GetNumSkeletons() const { return static_cast<int>(skeletons.size()); }
	const hkaSkeleton *GetSkeleton(int id) const { return skeletons.at(id); }
	const int GetNumAnimations() const { return static_cast<int>(animations.size()); }
	const hkaAnimation *GetAnimation(int id) const { return animations.at(id); }
	const int GetNumBindings() const { return static_cast<int>(bindings.size()); }
	const hkaAnimationBinding *GetBinding(int id) const { return bindings.at(id); }
	const int GetNumAttachments() const { return static_cast<int>(attachments.size()); }
	const hkaBoneAttachment *GetAttachment(int id) const { return attachments.at(id); }
	const int GetNumSkins() const { return static_cast<int>(skins.size()); }
	const hkaMeshBinding *GetSkin(int id) const { return skins.at(id); }

//private:
	std::vector<hkaSkeleton*> skeletons;
	std::vector<hkaAnimation*> animations;
	std::vector<hkaAnimationBinding*>  bindings;
	std::vector<hkaBoneAttachment*> attachments;
	std::vector<hkaMeshBinding*> skins;
};

struct xmlBone
{
	short ID;
	std::string name;
	xmlBone *parent;
	hkQTransform transform;
};

struct xmlRefFloat
{
	std::string name;
	float value;
};

class xmlSkeleton : public hkaSkeletonInternalInterface
{
	DECLARE_XMLCLASS(xmlSkeleton, hkaSkeleton)

	const char *GetSkeletonName() const { return name.c_str(); }	
	const int GetNumLocalFrames() const {return static_cast<int>(localFrames.size()); }
	const hkLocalFrameOnBone GetLocalFrame(int id) const { return localFrames.at(id); }
	const int GetNumPartitions() const { return static_cast<int>(partitions.size()); }
	const hkaPartition GetPartition(int id) const { return partitions.at(id); }
	const int GetNumBones() const { return static_cast<int>(bones.size()); }
	const char *GetBoneName(int id) const { return bones.at(id)->name.c_str(); }
	const hkQTransform *GetBoneTM(int id) const { return &bones.at(id)->transform; }
	const short GetBoneParentID(int id) const 
	{
		xmlBone *parent = bones.at(id)->parent;
		return parent ? parent->ID : -1;
	}

	const int GetNumReferenceFloats() const { return static_cast<int>(floats.size()); };
	const float GetReferenceFloat(int id) const { return floats.at(id).value; }
	const int GetNumFloatSlots() const { return GetNumReferenceFloats(); }
	const char *GetFloatSlot(int id) const { return floats.at(id).name.c_str(); }

	std::string name;
	std::vector<xmlBone*> bones;
	std::vector<xmlRefFloat> floats;
	std::vector<hkLocalFrameOnBone> localFrames;
	std::vector<hkaPartition> partitions;

	~xmlSkeleton()
	{
		for (auto &b : bones)
			delete b;
	}
};