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

#include "HavokApi.hpp"
#include "hkObjectBase.h"

template <class C>
struct hkaAnimatedReferenceFrame_t
    : hkaAnimatedReferenceFrameInternalInterface {
  C *Data;
  typedef C value_type;
  hkClassConstructor_nohash(hkaAnimatedReferenceFrame_t);
  hkaAnimatedReferenceFrameType GetType() const override {
    return Data->GetType();
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer> class _parent>
struct hkaAnimatedReferenceFrame_t_shared : _parent<_ipointer> {
  typedef _parent<_ipointer> parent_class;
  ADD_DISABLERS(parent_class, noType);

  disabledFunction(noType, hkaAnimatedReferenceFrameType) GetType() const {
    return hkaAnimatedReferenceFrameType::DEFAULT;
  };
  enabledFunction(noType, hkaAnimatedReferenceFrameType) GetType() const {
    return this->frameType;
  };
};

template <template <class C> class _ipointer>
struct hkaAnimatedReferenceFrame550_t_data : hkReferenceObject<_ipointer> {
  void noType();
};

template <template <class C> class _ipointer>
struct hkaAnimatedReferenceFrame550_rp_t_data
    : hkReferenceObject_rp<_ipointer> {
  void noType();
};

template <template <class C> class _ipointer>
struct hkaAnimatedReferenceFrame2012_t_data : hkReferenceObject<_ipointer> {
  hkaAnimatedReferenceFrameType frameType;
  GNU_PADDING(3);
};

template <template <class C> class _ipointer>
struct hkaAnimatedReferenceFrame2012_rp_t_data
    : hkReferenceObject_rp<_ipointer> {
  hkaAnimatedReferenceFrameType frameType;
};

template <template <class C> class _ipointer>
using hkaAnimatedReferenceFrame550_t =
    hkaAnimatedReferenceFrame_t_shared<_ipointer,
                                       hkaAnimatedReferenceFrame550_t_data>;
template <template <class C> class _ipointer>
using hkaAnimatedReferenceFrame550_rp_t =
    hkaAnimatedReferenceFrame_t_shared<_ipointer,
                                       hkaAnimatedReferenceFrame550_rp_t_data>;
template <template <class C> class _ipointer>
using hkaAnimatedReferenceFrame2012_t =
    hkaAnimatedReferenceFrame_t_shared<_ipointer,
                                       hkaAnimatedReferenceFrame2012_t_data>;
template <template <class C> class _ipointer>
using hkaAnimatedReferenceFrame2012_rp_t =
    hkaAnimatedReferenceFrame_t_shared<_ipointer,
                                       hkaAnimatedReferenceFrame2012_rp_t_data>;