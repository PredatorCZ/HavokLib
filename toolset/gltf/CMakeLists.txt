project(Havok2GLTF)

build_target(
  NAME
  hk_to_gltf
  TYPE
  ESMODULE
  VERSION
  1
  SOURCES
  hk_to_gltf.cpp
  LINKS
  havok-interface
  gltf-interface
  AUTHOR
  "Lukas Cone"
  DESCR
  "Convert Havok files to GLTF"
  START_YEAR
  2022)
