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

#include "hka_delta_decompressor.hpp"
#include <algorithm>
#include <climits>
#include <cmath>

void Decompress(float offset, float scale, char *buffer, char bitWidth,
                size_t numItems, std::vector<float> &container) {
  float refFrame = container.back();
  size_t curItem = 0;
  size_t currentBit = 0;
  const uint32 bitMask = (1 << bitWidth) - 1;
  uint32 *currentBuffer = reinterpret_cast<uint32 *>(buffer);
  const float fractal = 1.0f / bitMask;

  while (curItem < numItems) {
    uint32 curNumber = (*currentBuffer >> currentBit) & bitMask;
    currentBit += bitWidth;

    if (currentBit > 31) {
      currentBit -= 32;
      currentBuffer++;

      if (currentBit) {
        const uint32 subNumber = (*currentBuffer & ((1 << currentBit) - 1))
                                 << (bitWidth - currentBit);
        curNumber |= subNumber;
      }
    }

    float dequantized =
        refFrame + (offset + static_cast<float>(curNumber) * scale * fractal);
    refFrame = dequantized;
    container.push_back(dequantized);
    curItem++;
  }
}

void hkaDeltaDecompressor::Assign(
    hkaDeltaCompressedAnimationInternalInterface *input) {
  char *buffer = const_cast<char *>(input->GetData());
  hkaAnimation *anim = dynamic_cast<hkaAnimation *>(input);
  const size_t numTracks = anim->GetNumOfTransformTracks();
  const size_t numFloatTracks = anim->GetNumOfFloatTracks();
  const size_t numDynTracks = input->GetNumDynamicTracks();
  const size_t numFrames = input->GetNumOfPoses();

  uint8 *bitWidthStart =
      reinterpret_cast<uint8 *>(buffer + input->GetBitWidthOffset());
  StaticMask *masksStart =
      reinterpret_cast<StaticMask *>(buffer + input->GetStaticMaskOffset());
  float *offsetsStart =
      reinterpret_cast<float *>(buffer + input->GetOffsetsOffset());
  float *scalesStart =
      reinterpret_cast<float *>(buffer + input->GetScalesOffset());

  es::allocator_hybrid_base::LinkStorage(bitWidths, bitWidthStart,
                                         numDynTracks);
  es::allocator_hybrid_base::LinkStorage(masks, masksStart,
                                         numTracks + numFloatTracks);
  es::allocator_hybrid_base::LinkStorage(offsets, offsetsStart, numDynTracks);
  es::allocator_hybrid_base::LinkStorage(scales, scalesStart, numDynTracks);

  tracks.reserve(numTracks);
  floats.reserve(numFloatTracks);

  float *staticBuffer =
      reinterpret_cast<float *>(buffer + input->GetStaticDataOffset());

  buffer += input->GetQuantizedDataOffset();

  std::vector<DynamicTrack<float>> dynamicTracks;
  dynamicTracks.resize(numDynTracks);

  const size_t blockSize = input->GetBlockSize();
  const size_t numBlocks =
      static_cast<size_t>(ceilf(numFrames / static_cast<float>(blockSize)));
  const size_t numPreserved = input->GetNumPreserved();

  for (size_t b = 0; b < numBlocks; b++) {
    size_t numItems = blockSize * (b + 1);

    if (numItems > numFrames)
      numItems = numFrames % blockSize;
    else
      numItems = blockSize;

    for (size_t p = 0; p < numDynTracks; p++) {
      auto &tck = dynamicTracks[p];

      for (size_t pr = 0; pr < numPreserved; pr++) {
        tck.items.push_back(*reinterpret_cast<float *>(buffer));
        buffer += 4;
      }

      const size_t deltaSize = (bitWidths[p] * (numItems - 1) + 7) >> 3;

      Decompress(offsets[p], scales[p], buffer, bitWidths[p], numItems - 1,
                 tck.items);
      buffer += deltaSize;
    }
  }

  size_t curDynTrack = 0;

  for (size_t p = 0; p < numTracks; p++) {
    const StaticMask &mask = masks[p];
    tracks.emplace_back(std::make_unique<MasterTrack>(mask));
    auto &mRef = tracks.back();

    auto CopyDynamic = [&](StaticMask::MaskType msk, size_t id,
                           DynamicTrack<Vector4A16> *tck) {
      if (mask.UseSubTrack(msk)) {
        auto cTrack = dynamicTracks[curDynTrack++].items.begin();
        for (auto &c : tck->items) {
          c[id] = *cTrack++;
        }
      }
    };

    switch (mask.GetPosTrackType()) {
    case TT_STATIC: {
      StaticTrack<Vector4A16> *tck =
          static_cast<StaticTrack<Vector4A16> *>(mRef->pos.get());
      tck->track = reinterpret_cast<Vector &>(*staticBuffer);
      staticBuffer += 3;
      break;
    }
    case TT_DYNAMIC: {
      DynamicTrack<Vector4A16> *tck =
          static_cast<DynamicTrack<Vector4A16> *>(mRef->pos.get());

      Vector staticValues;

      if (!mask.UseSubTrack(StaticMask::posX)) {
        staticValues.X = *staticBuffer;
        staticBuffer++;
      }
      if (!mask.UseSubTrack(StaticMask::posY)) {
        staticValues.Y = *staticBuffer;
        staticBuffer++;
      }
      if (!mask.UseSubTrack(StaticMask::posZ)) {
        staticValues.Z = *staticBuffer;
        staticBuffer++;
      }

      if (mask.UseSubTrack(StaticMask::posX) ||
          mask.UseSubTrack(StaticMask::posY) ||
          mask.UseSubTrack(StaticMask::posZ)) {
        tck->items.insert(tck->items.begin(), numFrames, staticValues);
      }

      CopyDynamic(StaticMask::posX, 0, tck);
      CopyDynamic(StaticMask::posY, 1, tck);
      CopyDynamic(StaticMask::posZ, 2, tck);

      break;
    }
    default:
      break;
    }

    switch (mask.GetRotTrackType()) {
    case TT_STATIC: {
      StaticTrack<Vector4A16> *tck =
          static_cast<StaticTrack<Vector4A16> *>(mRef->rot.get());
      tck->track = reinterpret_cast<Vector4A16 &>(*staticBuffer);
      staticBuffer += 4;
      break;
    }
    case TT_DYNAMIC: {
      DynamicTrack<Vector4A16> *tck =
          static_cast<DynamicTrack<Vector4A16> *>(mRef->rot.get());

      Vector4A16 staticValues;

      if (!mask.UseSubTrack(StaticMask::rotX)) {
        staticValues.X = *staticBuffer;
        staticBuffer++;
      }
      if (!mask.UseSubTrack(StaticMask::rotY)) {
        staticValues.Y = *staticBuffer;
        staticBuffer++;
      }
      if (!mask.UseSubTrack(StaticMask::rotZ)) {
        staticValues.Z = *staticBuffer;
        staticBuffer++;
      }
      if (!mask.UseSubTrack(StaticMask::rotW)) {
        staticValues.W = *staticBuffer;
        staticBuffer++;
      }

      if (mask.UseSubTrack(StaticMask::rotX) ||
          mask.UseSubTrack(StaticMask::rotY) ||
          mask.UseSubTrack(StaticMask::rotZ) ||
          mask.UseSubTrack(StaticMask::rotW)) {
        tck->items.insert(tck->items.begin(), numFrames, staticValues);
      }

      CopyDynamic(StaticMask::rotX, 0, tck);
      CopyDynamic(StaticMask::rotY, 1, tck);
      CopyDynamic(StaticMask::rotZ, 2, tck);
      CopyDynamic(StaticMask::rotW, 3, tck);

      for (auto &t : tck->items) {
        if (t.W == 2.0f || t.W == -2.0f) {
          float basis = t.W * 0.5f;
          t.W = 0.f;
          t.QComputeElement();
          t.W *= basis;
        }
      }
      break;
    }
    default:
      break;
    }

    switch (mask.GetScaleTrackType()) {
    case TT_STATIC: {
      StaticTrack<Vector4A16> *tck =
          static_cast<StaticTrack<Vector4A16> *>(mRef->scale.get());
      tck->track = reinterpret_cast<Vector &>(*staticBuffer);
      staticBuffer += 3;
      break;
    }
    case TT_DYNAMIC: {
      DynamicTrack<Vector4A16> *tck =
          static_cast<DynamicTrack<Vector4A16> *>(mRef->scale.get());

      Vector staticValues;

      if (!mask.UseSubTrack(StaticMask::scaleX)) {
        staticValues.X = *staticBuffer;
        staticBuffer++;
      }
      if (!mask.UseSubTrack(StaticMask::scaleY)) {
        staticValues.Y = *staticBuffer;
        staticBuffer++;
      }
      if (!mask.UseSubTrack(StaticMask::scaleZ)) {
        staticValues.Z = *staticBuffer;
        staticBuffer++;
      }

      if (mask.UseSubTrack(StaticMask::scaleX) ||
          mask.UseSubTrack(StaticMask::scaleY) ||
          mask.UseSubTrack(StaticMask::scaleZ)) {
        tck->items.insert(tck->items.begin(), numFrames, staticValues);
      }

      CopyDynamic(StaticMask::scaleX, 0, tck);
      CopyDynamic(StaticMask::scaleY, 1, tck);
      CopyDynamic(StaticMask::scaleZ, 2, tck);

      break;
    }
    default:
      break;
    }

    for (size_t f = numTracks; f < numFloatTracks + numTracks; f++) {
      StaticMask &m = masks[f];

      switch (m.GetPosTrackType()) {
      case TT_IDENTITY:
        floats.emplace_back(std::make_unique<StaticTrack<float>>());
        break;
      case TT_STATIC:
        floats.emplace_back(
            std::make_unique<StaticTrack<float>>(*staticBuffer++));
        break;
      case TT_DYNAMIC: {
        auto cFloatTrack = std::make_unique<DynamicTrack<float>>();
        cFloatTrack->items.swap(dynamicTracks[curDynTrack++].items);
        floats.emplace_back(std::move(cFloatTrack));
        break;
      }
      }
    }
  }
}

MasterTrack::MasterTrack(const StaticMask &mask) {
  switch (mask.GetPosTrackType()) {
  case TT_IDENTITY:
  case TT_STATIC:
    pos = std::make_unique<StaticTrack<Vector4A16>>();
    break;
  case TT_DYNAMIC:
    pos = std::make_unique<DynamicTrack<Vector4A16>>();
    break;
  }

  switch (mask.GetRotTrackType()) {
  case TT_IDENTITY:
    rot = std::make_unique<StaticTrack<Vector4A16>>(Vector4A16{{}, 1.f});
    break;
  case TT_STATIC:
    rot = std::make_unique<StaticTrack<Vector4A16>>();
    break;
  case TT_DYNAMIC:
    rot = std::make_unique<DynamicTrack<Vector4A16>>();
    break;
  }

  switch (mask.GetScaleTrackType()) {
  case TT_IDENTITY:
    scale = std::make_unique<StaticTrack<Vector4A16>>(
        Vector4A16{1.f, 1.f, 1.f, 0.f});
    break;
  case TT_STATIC:
    scale = std::make_unique<StaticTrack<Vector4A16>>();
    break;
  case TT_DYNAMIC:
    scale = std::make_unique<DynamicTrack<Vector4A16>>();
    break;
  }
}
