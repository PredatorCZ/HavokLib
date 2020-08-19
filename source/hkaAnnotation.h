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
#include "hkInternalInterfaces.h"
#include "hkObjectBase.h"

template <class C>
struct hkaAnnotation_t : hkaAnnotationTrackInternalInterface {
  uni::Element<C> data;
  // std::unique_ptr<hkxEnvironmentSaver> saver;

  void SetDataPointer(void *ptr) override {
    data = {static_cast<C *>(ptr), false};
  }
  const void *GetPointer() const override { return data.get(); }

  void SwapEndian() override { data->SwapEndian(); }
  es::string_view GetName() const override {
    return static_cast<const char *>(data->name);
  };
  size_t Size() const override { return data->GetNumAnnotations(); };
  const hkaAnnotationFrame At(size_t id) const override {
    return data->GetAnnotation(id);
  }
};

template <template <class C> class _ipointer> struct hkaAnnotation {
  float time;
  _ipointer<char> text;
};

template <template <class C> class _ipointer> struct hkaAnnotationTrack1 {
  _ipointer<char> name;
  _ipointer<hkaAnnotation<_ipointer>> Annotations;
  uint32 numAnnotations;
};

template <template <class C> class _ipointer> struct hkaAnnotationTrack2 {
  _ipointer<char> name;
  hkArray<hkaAnnotation<_ipointer>, _ipointer> Annotations;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaAnnotation_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;

  const char *GetName() const { return this->name; }

  GetNum(Annotations);

  hkaAnnotationFrame GetAnnotation(size_t id) const {
    const hkaAnnotation<_ipointer> &ano = this->Annotations[id];
    return {ano.time, static_cast<const char*>(ano.text)};
  }

  void SwapEndian() {
    FByteswapper(GetNumAnnotations());

    for (size_t a = 0; a < GetNumAnnotations(); a++) {
      FByteswapper(this->Annotations[a].time);
    }
  }
};
