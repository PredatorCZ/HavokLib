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
#include "HavokApi.hpp"
#include "hkObjectBase.h"

template <class C>
struct hkaAnnotation_t : hkaAnnotationTrackInternalInterface {
  C *Data;
  hkClassConstructor_nohash(hkaAnnotation_t<C>);
  void SwapEndian() override { Data->SwapEndian(); }
  es::string_view GetName() const override {
    return static_cast<const char *>(Data->name);
  };
  size_t Size() const override { return Data->GetNumAnnotations(); };
  const hkaAnnotationFrame At(size_t id) const override {
    return Data->GetAnnotation(id);
  }
};

template <template <class C> class _ipointer> struct hkaAnnotation {
  float time;
  _ipointer<char> text;
};

template <template <class C> class _ipointer> struct hkaAnnotationTrack1 {
  _ipointer<char> name;
  _ipointer<hkaAnnotation<_ipointer>> annotations;
  uint32 numAnnotations;
};

template <template <class C> class _ipointer> struct hkaAnnotationTrack2 {
  _ipointer<char> name;
  hkArray<hkaAnnotation<_ipointer>, _ipointer> annotations;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaAnnotation_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;

  const char *GetName() const { return this->name; }

  enablePtrPair(annotations) GetNumAnnotations() const {
    return this->numAnnotations;
  }
  enablehkArray(annotations) GetNumAnnotations() const {
    return this->annotations.count;
  }

  enablePtrPairRef(annotations) GetNumAnnotations() {
    return this->numAnnotations;
  }
  enablehkArrayRef(annotations) GetNumAnnotations() {
    return this->annotations.count;
  }

  hkaAnnotationFrame GetAnnotation(size_t id) const {
    const hkaAnnotation<_ipointer> &ano = this->annotations[id];
    return {ano.time, static_cast<const char*>(ano.text)};
  }

  void SwapEndian() {
    FByteswapper(GetNumAnnotations());

    for (size_t a = 0; a < GetNumAnnotations(); a++) {
      FByteswapper(this->annotations[a].time);
    }
  }
};
