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

static const char 
*_hkName = "name",
*_hkObject = "hkobject",
*_hkClass = "class",
*_hkNumElements = "numelements",
*_hkParam = "hkparam"
;

ES_INLINE void PointerToString(const void *ptr, std::string &str)
{
	if (!ptr)
	{
		str.append("null");
		return;
	}

	str.append("0x");
	
	char buffer[20];
	snprintf(buffer, 20, ES_X64 ? "%llX" : "%X", reinterpret_cast<uintptr_t>(ptr));
	str.append(buffer);
}

template<class C>
ES_INLINE void ExportReflectedClass(C &input, XMLnode &parent)
{
	ReflectorWrapConst<C> refl(&input);
	
	for (int t = 0; t < refl.GetNumReflectedValues(); t++)
	{
		Reflector::KVPair pair = refl.GetReflectedPair(t);
		XMLnode nameNode = parent.append_child(_hkParam);
		nameNode.append_attribute(_hkName).set_value(pair.name);
		nameNode.append_buffer(pair.value.c_str(), pair.value.size());
	}
}