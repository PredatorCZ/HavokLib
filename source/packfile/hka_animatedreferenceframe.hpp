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

#include "base.hpp"
#include "internal/hka_animatedreferenceframe.hpp"

template <class C>
struct hkaAnimatedReferenceFrame_t
    : virtual hkaAnimatedReferenceFrameInternalInterface {
  uni::Element<C> data;
  // std::unique_ptr<hkaAnimatedReferenceFrameSaver> saver;

  void SetDataPointer(void *ptr) override {
    data = {static_cast<C *>(ptr), false};
  }
  const void *GetPointer() const override { return data.get(); }

  hkaAnimatedReferenceFrameType GetType() const override {
    return data->GetFrameType();
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaAnimatedReferenceFrame_t_shared : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parent_class;

  AccessMethodSingle(FrameType, hkaAnimatedReferenceFrameType::DEFAULT) {
    return this->FrameType;
  }
};

template <template <class C> class _ipointer, bool rp>
struct hkaAnimatedReferenceFrame500_t_data : hkReferenceObject<_ipointer> {};

template <template <class C> class _ipointer>
struct hkaAnimatedReferenceFrame500_t_data<_ipointer, true>
    : hkReferenceObject_rp<_ipointer> {};

template <template <class C> class _ipointer, bool rp>
struct hkaAnimatedReferenceFrame2012_t_data : hkReferenceObject<_ipointer> {
  hkaAnimatedReferenceFrameType FrameType;
  GNU_PADDING(3);
};

template <template <class C> class _ipointer>
struct hkaAnimatedReferenceFrame2012_t_data<_ipointer, true>
    : hkReferenceObject_rp<_ipointer> {
  hkaAnimatedReferenceFrameType FrameType;
};

template <template <class C> class _ipointer, bool rp>
using hkaAnimatedReferenceFrame500 =
    hkaAnimatedReferenceFrame_t_shared<_ipointer,
                                       hkaAnimatedReferenceFrame500_t_data, rp>;

template <template <class C> class _ipointer, bool rp>
using hkaAnimatedReferenceFrame2012 = hkaAnimatedReferenceFrame_t_shared<
    _ipointer, hkaAnimatedReferenceFrame2012_t_data, rp>;
