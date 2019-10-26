/*  Havok Format Library
    Copyright(C) 2016-2019 Lukas Cone

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

#include "hkaDeltaDecompressor.h"
#include "HavokApi.hpp"
#include <climits>
#include <cmath>

void Decompress(float offset, float scale, char *buffer, char bitWidth,
                int numItems, LineDynamicTrack *track) {
  auto &container = track->items;
  float refFrame = container.back();
  int curItem = 0;
  int currentBit = 0;
  const int bitMask = (1 << bitWidth) - 1;
  unsigned int *currentBuffer = reinterpret_cast<unsigned int *>(buffer);
  const float fractal = 1.0f / bitMask;

  while (curItem < numItems) {
    unsigned int curNumber = (*currentBuffer >> currentBit) & bitMask;
    currentBit += bitWidth;

    if (currentBit > 31) {
      currentBit -= 32;
      currentBuffer++;

      if (currentBit) {
        const unsigned int subNumber =
            (*currentBuffer & ((1 << currentBit) - 1))
            << (bitWidth - currentBit);
        curNumber |= subNumber;
      }
    }

    float dequantized =
        refFrame + (offset + static_cast<float>(curNumber) * scale * fractal);
    refFrame = dequantized;
    track->items.push_back(dequantized);
    curItem++;
  }
}

void hkaDeltaDecompressor::Assign(
    hkaDeltaCompressedAnimationInternalInterface *input) {
  char *buffer = const_cast<char *>(input->GetData());
  hkaAnimation *anim = dynamic_cast<hkaAnimation *>(input);
  const int numTracks = anim->GetNumOfTransformTracks();
  const int numFloatTracks = anim->GetNumOfFloatTracks();
  const int numDynTracks = input->GetNumDynamicTracks();
  const int numFrames = input->GetNumOfPoses();

  uchar *bitWidthStart =
      reinterpret_cast<uchar *>(buffer + input->GetBitWidthOffset());
  StaticMask *masksStart =
      reinterpret_cast<StaticMask *>(buffer + input->GetStaticMaskOffset());
  float *offsetsStart =
      reinterpret_cast<float *>(buffer + input->GetOffsetsOffset());
  float *scalesStart =
      reinterpret_cast<float *>(buffer + input->GetScalesOffset());

  bitWidths = bitWidths_type(bitWidthStart, bitWidthStart + numDynTracks,
                             bitWidths_type::allocator_type(bitWidthStart));
  masks = masks_type(masksStart, masksStart + (numTracks + numFloatTracks),
                     masks_type::allocator_type(masksStart));
  offsets = offsets_type(offsetsStart, offsetsStart + numDynTracks,
                         offsets_type::allocator_type(offsetsStart));
  scales = scales_type(scalesStart, scalesStart + numDynTracks,
                       scales_type::allocator_type(scalesStart));

  tracks.reserve(numTracks);
  floats.reserve(numFloatTracks);

  float *staticBuffer =
      reinterpret_cast<float *>(buffer + input->GetStaticDataOffset());

  std::vector<LineDynamicTrack *> dtracks;
  dtracks.reserve(numDynTracks);

  for (int p = 0; p < numTracks; p++) {
    StaticMask &m = masks[p];
    MasterTrack *mtPtr = new MasterTrack(m);
    MasterTrack &mt = *mtPtr;
    tracks.push_back(mtPtr);

    for (auto &t : mt.staticFloats) {
      *t = *staticBuffer++;

      if (fabs(*t) < FLT_EPSILON)
        *t = 0;
    }

    for (auto &dt : mt.dynamicTracks)
      dt->items.reserve(numFrames);

    dtracks.insert(dtracks.end(), mt.dynamicTracks.begin(),
                   mt.dynamicTracks.end());
    mt.dynamicTracks.clear();
    mt.staticFloats.clear();
  }

  for (int f = numTracks; f < numFloatTracks + numTracks; f++) {
    StaticMask &m = masks[f];

    switch (m.GetPosTrackType()) {
    case TT_IDENTITY:
      floats.push_back(new LineStaticTrack(0.0f));
      break;
    case TT_STATIC:
      floats.push_back(new LineStaticTrack(*staticBuffer++));
      break;
    case TT_DYNAMIC: {
      LineDynamicTrack *trck = new LineDynamicTrack();
      floats.push_back(trck);
      trck->items.reserve(numFrames);
      dtracks.push_back(trck);
      break;
    }
    }
  }

  buffer += input->GetQuantizedDataOffset();

  const int blockSize = input->GetBlockSize();
  const int numBlocks =
      static_cast<int>(ceilf(numFrames / static_cast<float>(blockSize)));
  const int numPreserved = input->GetNumPreserved();

  for (int b = 0; b < numBlocks; b++) {
    int numItems = blockSize * (b + 1);

    if (numItems > numFrames)
      numItems = numFrames % blockSize;
    else
      numItems = blockSize;

    for (int p = 0; p < numDynTracks; p++) {
      LineDynamicTrack *t = dtracks[p];

      for (int pr = 0; pr < numPreserved; pr++) {
        t->items.push_back(*reinterpret_cast<float *>(buffer));
        buffer += 4;
      }

      const int deltaSize = (bitWidths[p] * (numItems - 1) + 7) >> 3;

      Decompress(offsets[p], scales[p], buffer, bitWidths[p], numItems - 1, t);
      buffer += deltaSize;
    }
  }

  for (auto &t : tracks) {
    IVectorTrack<Vector4> *rotTrack = t->rotation;

    if (rotTrack->IsStatic()) {
      VectorStaticTrack<Vector4> *trck =
          static_cast<VectorStaticTrack<Vector4> *>(rotTrack);
      Vector4 &vec = trck->item;

      if (vec.W == 2.0f || vec.W == -2.0f) {
        vec.W = sqrtf(1 - (vec.X * vec.X) - (vec.Y * vec.Y) - (vec.Z * vec.Z)) *
                (vec.W * 0.5f);
      }
    } else {
      VectorDynamicTrack<Vector4> *trck =
          static_cast<VectorDynamicTrack<Vector4> *>(rotTrack);
      float Wframe = trck->tracks[3]->GetFrame(0);

      if (Wframe == 2.0f || Wframe == -2.0f) {
        delete trck->tracks[3];
        LineDynamicTrack *dTrack = new LineDynamicTrack();
        trck->tracks[3] = dTrack;
        dTrack->items.resize(numFrames);

        for (int t = 0; t < numFrames; t++) {
          Vector4A16 iVec = trck->GetVector(t);
          float &wItem = dTrack->items[t];
          wItem = sqrtf(1 - (iVec.X * iVec.X) - (iVec.Y * iVec.Y) -
                        (iVec.Z * iVec.Z)) *
                  (Wframe * 0.5f);
        }
      }
    }
  }
}

hkaDeltaDecompressor::~hkaDeltaDecompressor() {
  for (auto &f : floats)
    delete f;
  for (auto &t : tracks)
    delete t;
}

MasterTrack::MasterTrack(StaticMask &mask) {
  dynamicTracks.reserve(10);
  staticFloats.reserve(10);

  switch (mask.GetPosTrackType()) {
  case TT_IDENTITY:
    pos = new VectorStaticTrack<Vector>();
    break;
  case TT_STATIC: {
    auto _pos = new VectorStaticTrack<Vector>();
    pos = _pos;
    staticFloats.push_back(&_pos->item.X);
    staticFloats.push_back(&_pos->item.Y);
    staticFloats.push_back(&_pos->item.Z);
    break;
  }
  case TT_DYNAMIC:
    pos = new VectorDynamicTrack<Vector>(mask, StaticMask::posX, *this);
    break;
  }

  switch (mask.GetRotTrackType()) {
  case TT_IDENTITY:
    rotation = new VectorStaticTrack<Vector4>();
    static_cast<VectorStaticTrack<Vector4> *>(rotation)->item =
        Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    break;
  case TT_STATIC: {
    auto _rot = new VectorStaticTrack<Vector4>();
    rotation = _rot;
    staticFloats.push_back(&_rot->item.X);
    staticFloats.push_back(&_rot->item.Y);
    staticFloats.push_back(&_rot->item.Z);
    staticFloats.push_back(&_rot->item.W);
    break;
  }
  case TT_DYNAMIC:
    rotation =
        new VectorDynamicTrack<Vector4>(mask, StaticMask::rotX, *this);
    break;
  }

  switch (mask.GetScaleTrackType()) {
  case TT_IDENTITY:
    scale = new VectorStaticTrack<Vector>();
    static_cast<VectorStaticTrack<Vector> *>(scale)->item = {1.0f, 1.0f, 1.0f};
    break;
  case TT_STATIC: {
    auto _scale = new VectorStaticTrack<Vector>();
    scale = _scale;
    staticFloats.push_back(&_scale->item.X);
    staticFloats.push_back(&_scale->item.Y);
    staticFloats.push_back(&_scale->item.Z);
    break;
  }
  case TT_DYNAMIC:
    scale = new VectorDynamicTrack<Vector>(mask, StaticMask::scaleX, *this);
    break;
  }
}

MasterTrack::~MasterTrack() {
  delete pos;
  delete rotation;
  delete scale;
}
