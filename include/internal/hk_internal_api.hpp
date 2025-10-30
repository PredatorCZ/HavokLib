/*  Havok Format Library
    Copyright(C) 2016-2023 Lukas Cone

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
#include "hklib/hk_base.hpp"
#include "spike/io/bincore_fwd.hpp"
#include <stdexcept>

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

  virtual std::string_view GetClassName(hkToolset) const { return className; }
  virtual void SwapEndian() = 0;
  virtual void Process() {};
  virtual void SetDataPointer(void *Ptr) = 0;
  virtual void ToXML(XMLHandle) const {}
  virtual void Reflect(const IhkVirtualClass *) {}
  virtual void Save(BinWritterRef wr, hkFixups &fixups) const;
  virtual ~hkVirtualClass() = default;

  static IhkVirtualClass *Create(JenHash hash, CRule rule);
  template <class C> static IhkVirtualClass *Create(CRule rule);
};

using ClassCreatorFunc = IhkVirtualClass *(*)(CRule);
void RegisterHkClass(JenHash, ClassCreatorFunc);
template <class C, class G> consteval ClassCreatorFunc CreateHkClass() {
  return [](CRule rule) -> IhkVirtualClass * {
    return new C{G{rule.version, rule.x64, rule.reusePadding}, nullptr};
  };
}

template <class C> struct ClassRegisterInvoker;

#define CREATE_HK_CLASS_ALIAS(name, ...)                                       \
  template <> struct ClassRegisterInvoker<name> {                              \
    static inline const bool VALUE = [] {                                      \
      RegisterHkClass(                                                         \
          JenHash(#name),                                                      \
          CreateHkClass<__VA_ARGS__##MidInterface, clgen::LayoutLookup>());    \
      return true;                                                             \
    }();                                                                       \
  };                                                                           \
  template <> IhkVirtualClass *hkVirtualClass::Create<name>(CRule rule) {      \
    return CreateHkClass<__VA_ARGS__##MidInterface, clgen::LayoutLookup>()(    \
        rule);                                                                 \
  }

#define CREATE_HK_CLASS(...) CREATE_HK_CLASS_ALIAS(__VA_ARGS__, __VA_ARGS__)
