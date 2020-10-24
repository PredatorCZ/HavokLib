file(GLOB_RECURSE CORE_SOURCE_FILES "*.cpp")

build_target(
  NAME
  HavokLibStatic
  TYPE
  ${STATIC_TYPE}
  SOURCES
  ${CORE_SOURCE_FILES}
  LINKS
  pugixml
  NO_VERINFO
  NO_PROJECT_H)

target_include_directories(
  HavokLibStatic
  PUBLIC ../include
  PRIVATE .)

set_precore_sources(HavokLibStatic master_printer matrix44 reflector
                    reflector_xml)

if(USE_PIC AND _is_cool_compiler)
  build_target(
    NAME
    HavokLib
    TYPE
    OBJECT
    SOURCES
    ${CORE_SOURCE_FILES}
    ../3rd_party/PugiXml/src/pugixml.cpp
    PROPERTIES
    POSITION_INDEPENDENT_CODE
    ON
    NO_VERINFO
    NO_PROJECT_H)

  target_include_directories(
    HavokLib
    PUBLIC ../include ../3rd_party/PugiXml/src
    PRIVATE .)
  set_precore_sources(HavokLib master_printer matrix44 reflector reflector_xml)
else()
  add_library(HavokLib ALIAS HavokLibStatic)
endif()
