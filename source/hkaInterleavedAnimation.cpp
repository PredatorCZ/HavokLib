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

#include "hkaAnimation.h"

namespace {

template <class C>
struct hkaInterleavedAnimation_t
    : hkaInterleavedAnimationInternalInterface,
      hkaAnimation_t<typename C::parentClass, hkaAnimationLerpSampler> {
  typedef C value_type;
  typedef hkaAnimation_t<typename C::parentClass, hkaAnimationLerpSampler>
      parent;

  value_type *Data() { return static_cast<value_type *>(this->data.get()); }
  const value_type *Data() const {
    return static_cast<const value_type *>(this->data.get());
  }

  void SwapEndian() override {
    parent::SwapEndian();
    Data()->SwapEndian();
  }

  void Process() override {
    this->numFrames = static_cast<uint32>(this->GetNumTransforms() /
                                          this->GetNumOfTransformTracks());
    this->frameRate = static_cast<uint32>(this->numFrames / this->Duration());
  }

  void GetFrame(size_t trackID, int32 frame, hkQTransform &out) const override {
    out = *GetTransform(frame * GetNumOfTransformTracks() + trackID);
  }

  size_t GetNumTransforms() const override { return Data()->GetNumTransforms(0); }
  size_t GetNumFloats() const override { return Data()->GetNumFloats(0); }

  const hkQTransform *GetTransform(size_t id) const override {
    return Data()->GetTransform(id);
  }
  float GetFloat(size_t id) const override { return Data()->GetFloat(id); }
};

template <template <class C> class _ipointer, class _parent>
struct hkaInterleavedAnimation_t_shared : _parent {
  typedef _parent parent_class;

  GetNum(Transforms);
  GetNum(Floats);

  const hkQTransform *GetTransform(size_t id) const {
    return &this->Transforms[id];
  }

  AccessMethod(Float, 0.f) { return this->Floats[id]; }

  SwapMethod(Floats) {
    FByteswapper(GetNumFloats(0));
    const size_t numFloats = GetNumFloats(0);

    for (size_t i = 0; i < numFloats; i++)
      FByteswapper(this->Floats[i]);
  }

  void SwapEndian() {
    FByteswapper(GetNumTransforms(0));
    const size_t numTransforms = GetNumTransforms(0) * 12;

    float *data = reinterpret_cast<float *>(
        static_cast<hkQTransform *>(this->Transforms));
    float *dataEnd = data + numTransforms;

    for (; data != dataEnd; data++)
      FByteswapper(*data);

    SwapFloats(0);
  }
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaInterleavedAnimation500_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
  _ipointer<hkQTransform> Transforms;
  uint32 numTransforms;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaInterleavedAnimation550_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
  _ipointer<hkQTransform> Transforms;
  uint32 numTransforms;
  _ipointer<float> Floats;
  uint32 numFloats;
};

template <template <class C> class _ipointer,
          template <template <class C> class __ipointer, bool _rp>
          class _parent,
          bool rp>
struct hkaInterleavedAnimation700_t_data : _parent<_ipointer, rp> {
  typedef _parent<_ipointer, rp> parentClass;
  hkArray<hkQTransform, _ipointer> Transforms;
  hkArray<float, _ipointer> Floats;
};

template <template <class C> class _ipointer, bool rp>
using hkaInterleavedAnimation500 = hkaInterleavedAnimation_t_shared<
    _ipointer,
    hkaInterleavedAnimation500_t_data<_ipointer, hkaAnimation500, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaInterleavedAnimation550 = hkaInterleavedAnimation_t_shared<
    _ipointer,
    hkaInterleavedAnimation550_t_data<_ipointer, hkaAnimation550, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaInterleavedAnimation700 = hkaInterleavedAnimation_t_shared<
    _ipointer,
    hkaInterleavedAnimation700_t_data<_ipointer, hkaAnimation700, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaInterleavedAnimation2011 = hkaInterleavedAnimation_t_shared<
    _ipointer,
    hkaInterleavedAnimation700_t_data<_ipointer, hkaAnimation2011, rp>>;

template <template <class C> class _ipointer, bool rp>
using hkaInterleavedAnimation2016 = hkaInterleavedAnimation_t_shared<
    _ipointer,
    hkaInterleavedAnimation700_t_data<_ipointer, hkaAnimation2016, rp>>;

template <hkToolset ts, template <class C> class _ipointer, bool rp>
struct hkaInterleavedAnimationTier
    : hkaInterleavedAnimation2011<_ipointer, rp> {};

hkClassTier1Alias(hkaInterleavedAnimation, 500, 500);
hkClassTier1Alias(hkaInterleavedAnimation, 500, 510);
hkClassTier1Alias(hkaInterleavedAnimation, 550, 550);
hkClassTier1Alias(hkaInterleavedAnimation, 550, 600);
hkClassTier1Alias(hkaInterleavedAnimation, 550, 610);
hkClassTier1Alias(hkaInterleavedAnimation, 550, 650);
hkClassTier1Alias(hkaInterleavedAnimation, 550, 660);
hkClassTier1Alias(hkaInterleavedAnimation, 700, 700);
hkClassTier1Alias(hkaInterleavedAnimation, 700, 710);
hkClassTier1Alias(hkaInterleavedAnimation, 700, 2010_1);
hkClassTier1Alias(hkaInterleavedAnimation, 700, 2010_2);
hkClassTier1Alias(hkaInterleavedAnimation, 2016, 2016);
hkClassTier1Alias(hkaInterleavedAnimation, 2016, 2017);

static const std::unordered_map<CRule, hkVirtualClass *(*)()> registry{
    hkRegisterFullClass(hkaInterleavedAnimation)};
} // namespace

hkVirtualClass *hkaInterleavedAnimationInternalInterface::Create(CRule rule) {
  auto found = registry.find(rule);

  if (es::IsEnd(registry, found)) {
    return nullptr;
  }

  return found->second();
}
