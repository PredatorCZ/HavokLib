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

#pragma once
#include "hka_skeleton.hpp"
#include "hkx_mesh.hpp"
#include "spike/type/matrix44.hpp"
#include <span>

struct hkaMeshBinding : IhkaVirtualClass {
  virtual uni::Element<hkxMesh> Mesh() const = 0;
  virtual std::string_view SkeletonName() const = 0;
  virtual std::string_view Name() const = 0;
  virtual uni::Element<hkaSkeleton> Skeleton() const = 0;
  virtual size_t NumMappings() const = 0;
  virtual std::span<const int16> Mappings(size_t at) const = 0;
  virtual std::span<const es::Matrix44> BoneFromSkinMeshTransforms() const = 0;
};
