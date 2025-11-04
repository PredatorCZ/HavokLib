#pragma once
#include "spike/type/detail/sc_type.hpp"

enum class hkxVertexDescriptionDataHint : uint16 {
  NONE = 0,
  LIGHTMAP = 1,
};

struct hkxVertexFormat {
  uint8 stride;
  uint8 positionOffset;
  uint8 normalOffset;
  uint8 tangentOffset;
  uint8 binormalOffset;
  uint8 numBonesPerVertex;
  uint8 boneIndexOffset;
  uint8 boneWeightOffset;
  uint8 numTextureChannels;
  uint8 tFloatCoordOffset;
  uint8 tQuantizedCoordOffset;
  uint8 colorOffset;
};
