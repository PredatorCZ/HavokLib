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

#include "hkaAnimatedReferenceFrame.h"

template <class C>
struct hkaDefaultAnimatedReferenceFrame_t
    : hkaAnimatedReferenceFrame_t<typename C::parentClass> {
  typedef C value_type;
  typedef hkaAnimatedReferenceFrame_t<typename C::parentClass> parent;
  hkClassConstructor(hkaDefaultAnimatedReferenceFrame_t);

  void Process() override {
    this->frameRate =
        static_cast<uint32>(this->GetNumFrames() / this->GetDuration());
  }

  Vector4 GetUp() const override {
    return static_cast<value_type *>(this->Data)->GetUp();
  }
  Vector4 GetForward() const override {
    return static_cast<value_type *>(this->Data)->GetForward();
  }

  float GetDuration() const override {
    return static_cast<value_type *>(this->Data)->GetDuration();
  }

  size_t GetNumFrames() const override {
    return static_cast<value_type *>(this->Data)->NumRefFrames();
  }

  const Vector4 *GetRefFrame(size_t id) const override {
    return static_cast<value_type *>(this->Data)->GetRefFrame(id);
  }

  void SwapEndian() override {
    static_cast<value_type *>(this->Data)->SwapEndian();
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaDefaultAnimatedReferenceFrame_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;

  enablePtrPair(referenceFrameSamples) NumRefFrames() const {
    return this->numReferenceFrameSamples;
  }
  enablehkArray(referenceFrameSamples) NumRefFrames() const {
    return this->referenceFrameSamples.count;
  }
  enablePtrPairRef(referenceFrameSamples) NumRefFrames() {
    return this->numReferenceFrameSamples;
  }
  enablehkArrayRef(referenceFrameSamples) NumRefFrames() {
    return this->referenceFrameSamples.count;
  }

  const Vector4 *GetRefFrame(size_t id) const {
    return &this->referenceFrameSamples[id];
  }

  float GetDuration() const { return this->duration; }

  Vector4 &GetRefFrameRef(size_t id) { return this->referenceFrameSamples[id]; }

  Vector4 GetUp() const { return this->up; }
  Vector4 GetForward() const { return this->forward; }

  void SwapEndian() {
    FByteswapper(this->up);
    FByteswapper(this->forward);
    FByteswapper(this->duration);
    FByteswapper(NumRefFrames());
    const size_t numRefFrames = NumRefFrames();

    for (size_t r = 0; r < numRefFrames; r++) {
      FByteswapper(GetRefFrameRef(r));
    }
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaDefaultAnimatedReferenceFrame_ptrPair : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  Vector4 up;
  Vector4 forward;
  float duration;
  _ipointer<Vector4> referenceFrameSamples;
  uint32 numReferenceFrameSamples;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaDefaultAnimatedReferenceFrame_array : _parent<_ipointer> {
  typedef _parent<_ipointer> parentClass;
  Vector4 up;
  Vector4 forward;
  float duration;
  hkArray<Vector4, _ipointer> referenceFrameSamples;
};

template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame550_t_sharedData =
    hkaDefaultAnimatedReferenceFrame_ptrPair<_ipointer,
                                             hkaAnimatedReferenceFrame550_t>;
template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame550_rp_t_sharedData =
    hkaDefaultAnimatedReferenceFrame_ptrPair<_ipointer,
                                             hkaAnimatedReferenceFrame550_rp_t>;

template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame550_t =
    hkaDefaultAnimatedReferenceFrame_t_shared<
        _ipointer, hkaDefaultAnimatedReferenceFrame550_t_sharedData>;
template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame550_rp_t =
    hkaDefaultAnimatedReferenceFrame_t_shared<
        _ipointer, hkaDefaultAnimatedReferenceFrame550_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame660_t
    : hkaDefaultAnimatedReferenceFrame550_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame660_rp_t
    : hkaDefaultAnimatedReferenceFrame550_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame710_t_sharedData =
    hkaDefaultAnimatedReferenceFrame_array<_ipointer,
                                           hkaAnimatedReferenceFrame550_t>;
template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame710_rp_t_sharedData =
    hkaDefaultAnimatedReferenceFrame_array<_ipointer,
                                           hkaAnimatedReferenceFrame550_rp_t>;

template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame710_t =
    hkaDefaultAnimatedReferenceFrame_t_shared<
        _ipointer, hkaDefaultAnimatedReferenceFrame710_t_sharedData>;
template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame710_rp_t =
    hkaDefaultAnimatedReferenceFrame_t_shared<
        _ipointer, hkaDefaultAnimatedReferenceFrame710_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame2010_t
    : hkaDefaultAnimatedReferenceFrame710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame2010_rp_t
    : hkaDefaultAnimatedReferenceFrame710_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame2011_t
    : hkaDefaultAnimatedReferenceFrame710_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame2011_rp_t
    : hkaDefaultAnimatedReferenceFrame710_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame2012_t_sharedData =
    hkaDefaultAnimatedReferenceFrame_array<_ipointer,
                                           hkaAnimatedReferenceFrame2012_t>;
template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame2012_rp_t_sharedData =
    hkaDefaultAnimatedReferenceFrame_array<_ipointer,
                                           hkaAnimatedReferenceFrame2012_rp_t>;

template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame2012_t =
    hkaDefaultAnimatedReferenceFrame_t_shared<
        _ipointer, hkaDefaultAnimatedReferenceFrame2012_t_sharedData>;
template <template <class C> class _ipointer>
using hkaDefaultAnimatedReferenceFrame2012_rp_t =
    hkaDefaultAnimatedReferenceFrame_t_shared<
        _ipointer, hkaDefaultAnimatedReferenceFrame2012_rp_t_sharedData>;

template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame2013_t
    : hkaDefaultAnimatedReferenceFrame2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame2013_rp_t
    : hkaDefaultAnimatedReferenceFrame2012_rp_t<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame2014_t
    : hkaDefaultAnimatedReferenceFrame2012_t<_ipointer> {};
template <template <class C> class _ipointer>
struct hkaDefaultAnimatedReferenceFrame2014_rp_t
    : hkaDefaultAnimatedReferenceFrame2012_rp_t<_ipointer> {};
