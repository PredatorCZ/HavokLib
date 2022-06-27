project(Havok2GLTF VERSION 1)

build_target(
  NAME
  hk_to_gltf
  TYPE
  ESMODULE
  SOURCES
  hk_to_gltf.cpp
  LINKS
  havok
  gltf
  AUTHOR
  "Lukas Cone"
  DESCR
  "Convert Havok files to GLTF"
  START_YEAR
  2022)
