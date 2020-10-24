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

#include "internal/hka_defaultanimrefframe.hpp"
#include "hka_animatedreferenceframe.hpp"

namespace {

template <class C>
struct hkaDefaultAnimatedReferenceFrame_t
    : hkaAnimatedReferenceFrame_t<typename C::parentClass>,
      hkaDefaultAnimatedReferenceFrameInternalInterface {
  typedef C value_type;
  typedef hkaAnimatedReferenceFrame_t<typename C::parentClass> parent;

  value_type *Data() { return static_cast<value_type *>(this->data.get()); }
  const value_type *Data() const {
    return static_cast<const value_type *>(this->data.get());
  }

  void Process() override {
    this->frameRate =
        static_cast<uint32>(this->GetNumFrames() / this->GetDuration());
  }

  const Vector4A16 &GetUp() const override { return Data()->GetUp(); }
  const Vector4A16 &GetForward() const override { return Data()->GetForward(); }
  float GetDuration() const override { return Data()->GetDuration(); }
  size_t GetNumFrames() const override {
    return Data()->GetNumReferenceFrameSamples(0);
  }

  const Vector4A16 &GetRefFrame(size_t id) const override {
    return Data()->GetRefFrame(id);
  }

  void SwapEndian() override { Data()->SwapEndian(); }
};

template <template <class C> class _ipointer, class _parent>
struct hkaDefaultAnimatedReferenceFrame_t_shared : _parent {
  typedef _parent parent_class;

  GetNum(ReferenceFrameSamples);

  const Vector4A16 &GetRefFrame(size_t id) const {
    return this->ReferenceFrameSamples[id];
  }

  float GetDuration() const { return this->duration; }
  Vector4A16 &GetRefFrameRef(size_t id) {
    return this->ReferenceFrameSamples[id];
  }
  const Vector4A16 &GetUp() const { return this->up; }
  const Vector4A16 &GetForward() const { return this->forward; }

  void SwapEndian() {
    FByteswapper(this->up);
    FByteswapper(this->forward);
    FByteswapper(this->duration);
    FByteswapper(GetNumReferenceFrameSamples(0));
    const size_t numRefFrames = GetNumReferenceFrameSamples(0);

    for (size_t r = 0; r < numRefFrames; r++) {
      FByteswapper(GetRefFrameRef(r));
    }
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaDefaultAnimatedReferenceFrame500_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
  Vector4A16 up;
  Vector4A16 forward;
  float duration;
  _ipointer<Vector4A16> ReferenceFrameSamples;
  uint32 numReferenceFrameSamples;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaDefaultAnimatedReferenceFrame700_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
  Vector4A16 up;
  Vector4A16 forward;
  float duration;
  hkArray<Vector4A16, _ipointer> ReferenceFrameSamples;
};

template <template <class C> class _ipointer, bool rp>
using hkaDefaultAnimatedReferenceFrame500 =
    hkaDefaultAnimatedReferenceFrame_t_shared<
        _ipointer, hkaDefaultAnimatedReferenceFrame500_t_data<
                       _ipointer, hkaAnimatedReferenceFrame500, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaDefaultAnimatedReferenceFrame700 =
    hkaDefaultAnimatedReferenceFrame_t_shared<
        _ipointer, hkaDefaultAnimatedReferenceFrame700_t_data<
                       _ipointer, hkaAnimatedReferenceFrame500, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaDefaultAnimatedReferenceFrame2012 =
    hkaDefaultAnimatedReferenceFrame_t_shared<
        _ipointer, hkaDefaultAnimatedReferenceFrame700_t_data<
                       _ipointer, hkaAnimatedReferenceFrame2012, rp>>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkaDefaultAnimatedReferenceFrameTier
    : hkaDefaultAnimatedReferenceFrame700<_ipointer, rp> {};

hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 500, 500);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 500, 510);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 500, 550);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 500, 600);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 500, 610);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 500, 650);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 500, 660);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 2012, 2012_1);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 2012, 2012_2);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 2012, 2013);
hkClassTier1Alias(hkaDefaultAnimatedReferenceFrame, 2012, 2014);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterVersionedClasses(hkaDefaultAnimatedReferenceFrame, 500, 510, 550,
                               600, 610, 650, 660, 700, 710, 2010_1, 2010_2,
                               2011_1, 2011_2, 2011_3, 2012_1, 2012_2, 2013,
                               2014)};
} // namespace

hkVirtualClass *
hkaDefaultAnimatedReferenceFrameInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
