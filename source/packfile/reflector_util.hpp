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

#pragma once
#define _REFLECTOR_ADDN_HASH(value) JenHash(#value),
#define _REFLECTOR_ADDN_ENUMNAME(value) #value,

#define REFLECTOR_ENUM_HASH(classname, ...)                                    \
  static const JenHash *GetHashes() {                                          \
    static const JenHash hashed[] = {                                          \
        StaticFor(_REFLECTOR_ADDN_HASH, __VA_ARGS__)};                         \
    return hashed;                                                             \
  }

#define _REFLECTOR_ADDN_REMAP(classname, _id, value)                           \
  {_REFLECTOR_ADDN_HASH(value) classname::value},

#define REFLECTOR_ENUM_BUILD_REMAP(classname, ...)                             \
  static const std::unordered_map<JenHash, classname> classname##_remap = {    \
      StaticForArgID(_REFLECTOR_ADDN_REMAP, classname, __VA_ARGS__)};
