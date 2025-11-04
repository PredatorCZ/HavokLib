/*  Havok Format Library
    Copyright(C) 2016-2025 Lukas Cone

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

#include "spike/master_printer.hpp"
#include "spike/util/pugiex.hpp"
#include "spike/util/vectors_string.hpp"
#include "toolset.hpp"
#include <charconv>
#include <cinttypes>

[[maybe_unused]] static const char *_hkName = "name", *_hkObject = "hkobject",
                                   *_hkClass = "class",
                                   *_hkNumElements = "numelements",
                                   *_hkParam = "hkparam";

static inline void PointerToString(const void *ptr, std::string &str) {
  if (!ptr) {
    str.append("null");
    return;
  }

  str.append("0x");
  char buffer[20];
  snprintf(buffer, 20, "%" PRIXPTR, reinterpret_cast<uintptr_t>(ptr));
  str.append(buffer);
}

template <class C> static void ExportReflectedClass(C &input, XMLnode &parent) {
  ReflectorWrap<const C> refl(input);

  for (ReflectorMember v : refl) {
    ReflectorMember::KVPair pair = v.ReflectedPair();
    XMLnode nameNode = parent.append_child(_hkParam);
    nameNode.append_attribute(_hkName).set_value(pair.name.data());
    nameNode.append_buffer(pair.value.c_str(), pair.value.size());
  }
}

static inline std::string to_string(const hkQTransform &tm) {
  std::string retVal;
  retVal.push_back('(');
  retVal
      .append(std::to_string(reinterpret_cast<const Vector &>(tm.translation)))
      .append(")(")
      .append(std::to_string(tm.rotation))
      .append(")(")
      .append(std::to_string(reinterpret_cast<const Vector &>(tm.scale)))
      .push_back(')');
  return retVal;
}

static inline void ToXML(const char *name, const IhkVirtualClass *item,
                         pugi::xml_node &parent, hkToolset toolset) {
  pugi::xml_node variantNode = parent.append_child(_hkParam);
  variantNode.append_attribute(_hkName).set_value(name);
  std::string buffer;

  if (toolset < HK700)
    buffer.push_back('(');

  PointerToString(item->GetPointer(), buffer);

  if (toolset < HK700) {
    buffer.append(" null)");
  }

  variantNode.append_buffer(buffer.c_str(), buffer.size());
}

static inline void ToXML(const char *name, std::string_view item,
                         pugi::xml_node &parent) {
  pugi::xml_node node = parent.append_child(_hkParam);
  node.append_attribute(_hkName).set_value(name);
  node.append_buffer(item.data(), item.size());
}

template <class T>
  requires std::is_arithmetic_v<T>
static void ToXML(const char *name, T item, pugi::xml_node &parent) {
  char buffer[32]{};
  std::to_chars(buffer, buffer + 32, item);
  ToXML(name, buffer, parent);
}

template <class T>
  requires std::is_arithmetic_v<T>
static pugi::xml_node ToXMLArray(const char *name, T numItems,
                                 pugi::xml_node &parent) {
  pugi::xml_node node = parent.append_child(_hkParam);
  node.append_attribute(_hkName).set_value(name);
  node.append_attribute(_hkNumElements).set_value(numItems);
  return node;
}

template <class T>
  requires std::is_base_of_v<IhkVirtualClass, T>
static void ToXML(const char *name, T *item, pugi::xml_node &parent) {
  std::string buffer;
  PointerToString(item->GetPointer(), buffer);
  ToXML(name, buffer, parent);
}

template <class T>
static void ToXML(const char *name, uni::Element<const T> &&item,
                  pugi::xml_node &parent) {
  std::string buffer;
  PointerToString(item->GetPointer(), buffer);
  ToXML(name, buffer, parent);
}

template <class T>
static void ToXML(T &&iterable, pugi::xml_node &node,
                  uint32 numLineItems = 0x10) {
  std::string buffer;
  uint32 cc = 0;
  static const char *ident = "\n\t\t\t\t";

  for (auto &&s : iterable) {
    if constexpr (std::is_arithmetic_v<std::decay_t<decltype(s)>>) {
      buffer.append(std::to_string(s));
    } else {
      std::string _buffer;
      PointerToString(s->GetPointer(), _buffer);
      buffer += _buffer;
    }

    cc++;
    if (cc % numLineItems) {
      buffer.push_back(' ');
    } else {
      buffer.append(ident);
    }
  }

  if (buffer.size()) {
    if (cc % numLineItems) {
      buffer.pop_back();
      buffer += ident;
    }
    buffer.pop_back();
    node.append_buffer(buffer.c_str(), buffer.size());
  }
}
