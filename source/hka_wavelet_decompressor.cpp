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

#include "hka_wavelet_decompressor.hpp"
#include <cassert>

static const Vector4A16 BOTTOM_ROW[] = {
    {1.0f, 1.0f, 1.0f, 1.0f},         {0.5f, 0.5f, 0.5f, 0.5f},
    {-0.25f, -0.25f, -0.25f, -0.25f}, {-0.25f, 0.25f, 0.75f, 0.75f},
    {0.0f, 0.0f, 0.0f, 0.0f},         {-0.25f, -0.125f, 0.0f, 0.0f},
    {-0.25f, 0.75f, -0.25f, -0.25f},  {0.0f, -0.125f, -0.25f, 0.75f}};

static const Vector4A16 TOP_ROW[] = {
    {1.0f, 1.0f, 1.0f, 1.0f},          {-0.5f, -0.25f, 0.0f, 0.25f},
    {-0.5f, 0.0625f, 0.625f, 0.1875f}, {0.0f, -0.0625f, -0.125f, -0.1875f},
    {-0.5f, 0.625f, -0.25f, -0.125f},  {0.0f, -0.125f, -0.25f, 0.75f},
    {0.0f, 0.0f, 0.0f, -0.125f},       {0.0f, 0.0f, 0.0f, 0.0f}};

static void UpdateReflect(float factor, float *outData, const float *inData,
                          int frame) {
  const int prevFrame = frame - 1;

  int v5 = 1;
  if (frame - 1 >= 4) {
    const float *v06 = inData;
    float *v07 = outData;
    uint32 v7 = ((uint32)(frame - 5) >> 2) + 1;
    v5 = 4 * v7 + 1;
    do {
      --v7;

      v07[1] = (v06[0] + v06[1]) * factor + v07[1];
      v07[2] = (v06[1] + v06[2]) * factor + v07[2];
      v07[3] = (v06[2] + v06[3]) * factor + v07[3];
      v07[4] = (v06[3] + v06[4]) * factor + v07[4];

      v06 += 4;
      v07 += 4;
    } while (v7);
  }
  if (v5 < frame) {
    float *v11 = outData + v5;
    int v12 = frame - v5;

    do {
      *v11 = (v11[prevFrame] + v11[frame]) * factor + *v11;
      v11++;
      --v12;
    } while (v12);
  }

  outData[0] = factor * 2 * inData[0] + outData[0];
}

static void PredictReflect(float factor, const float *inData, float *outData,
                           int frame) {
  int prevFrame = frame - 1;
  int v7 = 0;

  if (prevFrame >= 4) {
    const float *v06 = inData;
    float *v07 = outData;
    uint32 v10 = ((uint32)(frame - 5) >> 2) + 1;

    v7 = 4 * v10;
    do {
      --v10;

      v07[0] = (v06[0] + v06[1]) * factor + v07[0];
      v07[1] = (v06[1] + v06[2]) * factor + v07[1];
      v07[2] = (v06[2] + v06[3]) * factor + v07[2];
      v07[3] = (v06[3] + v06[4]) * factor + v07[3];

      v06 += 4;
      v07 += 4;
    } while (v10);
  }

  if (v7 < prevFrame) {
    float *v14 = outData + v7;
    int v15 = prevFrame - v7;

    do {
      *v14 = (v14[-prevFrame] + v14[-frame]) * factor + *v14;
      v14++;
      --v15;
    } while (v15);
  }

  outData[prevFrame] = inData[prevFrame] * factor * 2 + outData[prevFrame];
}

static void MergeOddEven(const float *inData, float *outData, int numFrames) {
  std::vector<float> buffer(inData, inData + numFrames);
  float *bufferPtr = buffer.data();

  const int v6 = numFrames >> 1;
  int v7 = 0;
  if (v6 >= 4) {
    uint32 v9 = ((uint32)(v6 - 4) >> 2) + 1;

    float *v01 = bufferPtr + v6;
    float *v02 = bufferPtr;
    float *v03 = outData;

    v7 = 4 * v9;
    do {
      --v9;

      v03[0] = v02[0];
      v03[1] = v01[0];
      v03[2] = v02[1];
      v03[3] = v01[1];
      v03[4] = v02[2];
      v03[5] = v01[2];
      v03[6] = v02[3];
      v03[7] = v01[3];

      v01 += 4;
      v02 += 4;
      v03 += 8;
    } while (v9);
  }
  if (v7 < v6) {
    float *v12 = &bufferPtr[v7 + v6];
    do {
      outData[2 * v7] = bufferPtr[v7];
      ++v7;
      outData[2 * v7 - 1] = *v12++;
    } while (v7 < v6);
  }
}

static void TransformWavelet(float *values, size_t blockSize) {
  Vector4A16 top(values[0]);
  Vector4A16 bottom(top);

  for (uint32 i = 1; i < 8; i++) {
    top = TOP_ROW[i] * values[i] + top;
    bottom = BOTTOM_ROW[i] * values[i] + bottom;
  }

  memcpy(values, &top, 16);
  memcpy(values + 4, &bottom, 16);

  for (size_t v7 = 8; v7 < blockSize;) {
    UpdateReflect(-0.25, values, values + v7, v7);
    PredictReflect(0.5, values, values + v7, v7);
    v7 *= 2;
    MergeOddEven(values, values, v7);
  }
}

static size_t Decompress(float offset, float scale, char *buffer, char bitWidth,
                         size_t numIndicies, std::vector<float> &coeficients) {
  const size_t actualNumItems = coeficients.size();
  size_t currentItem = 0;
  size_t currentBit = 0;
  int dataCurrentBit = 0;
  int *cBuffer = reinterpret_cast<int *>(buffer);
  uint32 *dataBuffer = reinterpret_cast<uint32 *>(buffer + numIndicies);
  const int bitMask = (1 << bitWidth) - 1;
  const float fractal = 1.0f / bitMask;
  int numCoeficients = 0;

  while (currentItem < actualNumItems) {
    if (!(*cBuffer & (1 << currentBit))) {
      uint32 curNumber = (*dataBuffer >> dataCurrentBit) & bitMask;
      dataCurrentBit += bitWidth;

      if (dataCurrentBit > 31) {
        dataCurrentBit -= 32;
        dataBuffer++;

        if (dataCurrentBit) {
          const uint32 subNumber = (*dataBuffer & ((1 << dataCurrentBit) - 1))
                                   << (bitWidth - dataCurrentBit);
          curNumber |= subNumber;
        }
      }

      float dequantized =
          (offset + static_cast<float>(curNumber) * scale * fractal);
      coeficients[currentItem] = dequantized;
      numCoeficients++;
    }

    currentBit++;

    if (currentBit > 31) {
      cBuffer++;
      currentBit = 0;
    }

    currentItem++;
  }

  numCoeficients = (numCoeficients * bitWidth + 7) >> 3;
  return numIndicies + numCoeficients;
}

void hkaWaveletDecompressor::Assign(
    hkaWaveletCompressedAnimationInternalInterface *input) {
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

  bitWidths = {bitWidthStart, numDynTracks};
  masks = {masksStart, numTracks + numFloatTracks};
  offsets = {offsetsStart, numDynTracks};
  scales = {scalesStart, numDynTracks};

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

      const size_t numIndicies = (blockSize + 7) >> 3;
      std::vector<float> coeficients(numIndicies * 8);

      buffer += Decompress(offsets[p], scales[p], buffer, bitWidths[p],
                           numIndicies, coeficients);
      TransformWavelet(coeficients.data(), blockSize);

      tck.items.insert(tck.items.end(), coeficients.begin(), coeficients.end());
    }
  }

  size_t curDynTrack = 0;
  for (size_t p = 0; p < numTracks; p++) {
    StaticMask &mask = masks[p];
    MasterTrack *mtPtr = new MasterTrack(mask);
    tracks.emplace_back(mtPtr);

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
          static_cast<StaticTrack<Vector4A16> *>(mtPtr->pos.get());
      tck->track = reinterpret_cast<Vector &>(*staticBuffer);
      staticBuffer += 3;
      break;
    }
    case TT_DYNAMIC: {
      DynamicTrack<Vector4A16> *tck =
          static_cast<DynamicTrack<Vector4A16> *>(mtPtr->pos.get());

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
          static_cast<StaticTrack<Vector4A16> *>(mtPtr->rot.get());
      memcpy(tck->track._arr, staticBuffer, sizeof(Vector4A16));
      staticBuffer += 4;
      break;
    }
    case TT_DYNAMIC: {
      DynamicTrack<Vector4A16> *tck =
          static_cast<DynamicTrack<Vector4A16> *>(mtPtr->rot.get());

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
          static_cast<StaticTrack<Vector4A16> *>(mtPtr->scale.get());
      tck->track = reinterpret_cast<Vector &>(*staticBuffer);
      staticBuffer += 3;
      break;
    }
    case TT_DYNAMIC: {
      DynamicTrack<Vector4A16> *tck =
          static_cast<DynamicTrack<Vector4A16> *>(mtPtr->scale.get());

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
        floats.emplace_back(new StaticTrack<float>);
        break;
      case TT_STATIC:
        floats.emplace_back(new StaticTrack<float>(*staticBuffer++));
        break;
      case TT_DYNAMIC: {
        auto cFloatTrack = new DynamicTrack<float>;
        floats.emplace_back(cFloatTrack);
        cFloatTrack->items.swap(dynamicTracks[curDynTrack++].items);
        break;
      }
      }
    }
  }
}
