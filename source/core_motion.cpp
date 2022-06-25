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

#include "internal/hka_animation.hpp"
#include "internal/hka_defaultanimrefframe.hpp"

void hkaAniTrackHandle::GetValue(uni::RTSValue &output, float time) const {
  hdl->GetValue(output, time, index);
}

// https://en.wikipedia.org/wiki/Slerp
static Vector4A16 SLerp(const Vector4A16 &v0, const Vector4A16 &_v1, float t) {
  Vector4A16 v1 = _v1;
  float dot = v0.Dot(v1);

  // If the dot product is negative, slerp won't take
  // the shorter path. Fix by reversing one quaternion.
  if (dot < 0.0f) {
    v1 *= -1;
    dot *= -1;
  }

  static const float DOT_THRESHOLD = 0.9995f;
  if (dot > DOT_THRESHOLD) {
    // If the inputs are too close for comfort, linearly interpolate
    // and normalize the result.

    Vector4A16 result = v0 + (v1 - v0) * t;
    return result.Normalize();
  }

  const float theta00 = acos(dot);   // theta00 = angle between input vectors
  const float theta01 = theta00 * t; // theta01 = angle between v0 and result
  const float theta02 = sin(theta01);
  const float theta03 = 1.0f / sin(theta00);
  const float s0 = cos(theta01) - dot * theta02 * theta03;
  const float s1 = theta02 * theta03;

  return (v0 * s0) + (v1 * s1);
}

static Vector4A16 Lerp(const Vector4A16 &v1, const Vector4A16 &v2,
                       float delta) {
  return v1 + (v2 - v1) * delta;
}

void hkaAnimationLerpSampler::GetValue(uni::RTSValue &output, float time,
                                       size_t trackID) const {
  const float frameFull = time * frameRate;
  int32 frame = static_cast<int32>(frameFull);
  float delta = frameFull - frame;
  const int32 endFrame = static_cast<int32>(numFrames - 1);

  if (frameFull >= endFrame) {
    frame = endFrame;
    delta = 0.f;
  }

  if (!delta) {
    GetFrame(trackID, frame, output);
  } else {
    hkQTransform start;
    GetFrame(trackID, frame++, start);
    hkQTransform end;
    GetFrame(trackID, frame, end);

    output.translation = ::Lerp(start.translation, end.translation, delta);
    output.rotation = ::SLerp(start.rotation, end.rotation, delta);
    output.scale = ::Lerp(start.scale, end.scale, delta);
  }
}

void hkaAnimatedReferenceFrameInternalInterface::GetValue(uni::RTSValue &output,
                                                          float time) const {
  const float frameFull = time * frameRate;
  int32 frame = static_cast<int32>(frameFull);
  float delta = frameFull - frame;
  const auto numFrames = GetNumFrames();
  const int32 endFrame = static_cast<int32>(numFrames - 1);

  if (frameFull >= endFrame) {
    frame = endFrame;
    delta = 0.f;
  }

  auto ConvertRefFrame = [&](const Vector4A16 &input) {
    uni::RTSValue retVal;
    retVal.translation = Vector4A16(input, 1.f);
    retVal.rotation.X = sinf(input.W * 0.5f);
    retVal.rotation.Y = retVal.rotation.X;
    retVal.rotation.Z = retVal.rotation.X;
    retVal.rotation *= GetUp();
    retVal.rotation.W = cosf(input.W * 0.5f);
    return retVal;
  };

  if (!delta) {
    auto cFrame = GetRefFrame(frame);
    output = ConvertRefFrame(cFrame);
  } else {
    auto frame0 = GetRefFrame(frame++);
    auto frame1 = GetRefFrame(frame);

    hkQTransform start = ConvertRefFrame(frame0);
    hkQTransform end = ConvertRefFrame(frame1);

    output.translation = ::Lerp(start.translation, end.translation, delta);
    output.rotation = ::SLerp(start.rotation, end.rotation, delta);
  }
}
