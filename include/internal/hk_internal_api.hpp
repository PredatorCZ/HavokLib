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
#include "datas/bincore_fwd.hpp"
#include "hklib/hk_base.hpp"

struct hkFixups;
struct IhkPackFile;

struct CRule {
  hkToolset version;
  bool reusePadding;
  bool x64;
  char reserved;

  CRule() : version(HKUNKVER), reusePadding(false), x64(true), reserved() {}
  CRule(hkToolset ver, bool rp, bool _x64)
      : version(ver), reusePadding(rp), x64(_x64), reserved() {}

  bool operator==(const CRule &s) const {
    return reinterpret_cast<const uint32 &>(*this) ==
           reinterpret_cast<const uint32 &>(s);
  }
};

namespace std {
template <> struct hash<CRule> {
  size_t operator()(const CRule &s) const noexcept {
    return reinterpret_cast<const uint32 &>(s);
  }
};
} // namespace std

struct hkVirtualClass {
  JenHash hash[4]{};
  CRule rule;
  std::string_view className;
  IhkPackFile *header;

  void AddHash(JenHash hsh) {
    for (size_t i = 0; i < 4; i++) {
      if (!hash[i].raw()) {
        hash[i] = hsh;
        return;
      } else if (hash[i] == hsh) {
        return;
      }
    }

    throw std::overflow_error("New hash exceeded reserverd limit.");
  }

  bool HasHash(JenHash hsh) const {
    for (size_t i = 0; i < 4; i++) {
      if (!hash[i].raw()) {
        return false;
      } else if (hash[i] == hsh) {
        return true;
      }
    }

    return false;
  }

  virtual std::string_view GetClassName(hkToolset) const {
    return className;
  }
  virtual void SwapEndian() = 0;
  virtual void Process(){};
  virtual void SetDataPointer(void *Ptr) = 0;
  virtual void ToXML(XMLHandle) const {}
  virtual void Reflect(const IhkVirtualClass *) {}
  virtual void Save(BinWritterRef wr, hkFixups &fixups) const;
  virtual ~hkVirtualClass() = default;

  static IhkVirtualClass *Create(JenHash hash, CRule rule);
};
