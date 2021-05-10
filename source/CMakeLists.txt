file(GLOB_RECURSE CORE_SOURCE_FILES "*.cpp")

if(BUILD_STATIC_LIBS)
  set(BUILD_STATIC_OBJECTS ON)
endif()

if(BUILD_SHARED_LIBS)
  set(BUILD_SHARED_OBJECTS ON)
endif()

if(BUILD_STATIC_OBJECTS OR (MSVC AND BUILD_SHARED_OBJECTS))
  build_target(
    NAME
    havok-objects
    TYPE
    OBJECT
    SOURCES
    ${CORE_SOURCE_FILES}
    LINKS
    pugixml
    NO_VERINFO
    NO_PROJECT_H)

  target_include_directories(
    havok-objects
    PUBLIC ../include
    PRIVATE .)

  set_precore_sources(havok-objects master_printer matrix44 reflector
                      reflector_xml)
endif()

if(BUILD_SHARED_OBJECTS)
  build_target(
    NAME
    havok-shared
    TYPE
    OBJECT
    SOURCES
    ${CORE_SOURCE_FILES}
    PROPERTIES
    POSITION_INDEPENDENT_CODE
    ON
    NO_VERINFO
    NO_PROJECT_H)

  target_include_directories(
    havok-shared
    PUBLIC ../include
    PRIVATE .)

  set_precore_sources(havok-shared master_printer matrix44 reflector
                      reflector_xml)

  if(PUGI_AS_SHARED)
    target_link_libraries(havok-shared pugixml)
  else()
    target_include_directories(havok-shared PUBLIC ../3rd_party/PugiXml/src)
    target_sources(havok-shared PRIVATE ../3rd_party/PugiXml/src/pugixml.cpp)
  endif()
endif()

if(BUILD_STATIC_LIBS)
  build_target(
    NAME
    havok-static
    PROPERTIES
    OUTPUT_NAME
    havok
    TYPE
    STATIC
    LINKS
    pugixml
    havok-objects
    NO_VERINFO
    NO_PROJECT_H)
endif()

if(BUILD_SHARED_LIBS)
  target_compile_definitions(havok-shared PRIVATE HK_EXPORT)

  build_target(
    NAME
    havok
    TYPE
    SHARED
    LINKS
    havok-shared
    START_YEAR
    2017
    AUTHOR
    "Lukas Cone"
    NO_PROJECT_H)
endif()
