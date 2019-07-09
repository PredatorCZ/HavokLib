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
#define _REFLECTOR_ADDN_HASH(value) JenkinsHash(#value, sizeof(#value) -1),

#define REFLECTOR_ENUM_HASH(classname,...) static const int classname##_reflectedSize = VA_NARGS(__VA_ARGS__);\
const JenHash classname##_hashed[classname##_reflectedSize] = { StaticFor(_REFLECTOR_ADDN_HASH, __VA_ARGS__) }; \
enum classname { StaticFor(_REFLECTOR_ADDN_ENUMVAL, __VA_ARGS__) };

#define _REFLECTOR_ADDN_REMAP(classname, _id, value) { _REFLECTOR_ADDN_HASH(value) classname::value },

#define REFLECTOR_ENUM_BUILD_REMAP(classname,...) static const std::map<JenHash, classname> classname##_remap = \
{ StaticForArgID(_REFLECTOR_ADDN_REMAP, classname, __VA_ARGS__) };

#define REFLECTOR_ENUM_WOENUM(classname, ...) static const int classname##_reflectedSize = VA_NARGS(__VA_ARGS__);\
static const char *classname##_reflected[classname##_reflectedSize] = { StaticFor(_REFLECTOR_ADDN_ENUM, __VA_ARGS__) };


#define REFLECTOR_WOENUM_WREMAP(classname, classHash, ...) REFLECTOR_ENUM_WOENUM(classname, classHash, __VA_ARGS__)\
REFLECTOR_ENUM_BUILD_REMAP(classname, __VA_ARGS__)
