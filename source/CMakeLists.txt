file(GLOB_RECURSE CORE_SOURCE_FILES "*.cpp")

if((NOT NO_OBJECTS) OR HK_STATIC_LIB)
  build_target(
    NAME
    havok-objects
    TYPE
    OBJECT
    SOURCES
    ${CORE_SOURCE_FILES}
    LINKS
    precore-objects
    NO_VERINFO
    NO_PROJECT_H)

  set_target_properties(havok-objects PROPERTIES POSITION_INDEPENDENT_CODE
                                                 ${OBJECTS_PID})
  target_compile_options(havok-objects PRIVATE -fvisibility=hidden)

  target_include_directories(
    havok-objects
    PUBLIC ../include
    PRIVATE .)
endif()

if(HK_STATIC_LIB)
  build_target(
    NAME
    havok-static
    PROPERTIES
    OUTPUT_NAME
    havok
    TYPE
    STATIC
    LINKS
    havok-objects
    NO_VERINFO
    NO_PROJECT_H)
endif()

if(BUILD_SHARED_LIBS)
  build_target(
    NAME
    havok
    TYPE
    SHARED
    SOURCES
    ${CORE_SOURCE_FILES}
    LINKS
    precore
    START_YEAR
    2017
    AUTHOR
    "Lukas Cone"
    NO_PROJECT_H)

  target_include_directories(
    havok
    PUBLIC ../include
    PRIVATE .)

  target_compile_definitions(
    havok
    PRIVATE HK_EXPORT REF_EXPORT
    INTERFACE HK_IMPORT)

  install(
      TARGETS havok
      LIBRARY NAMELINK_SKIP DESTINATION $<IF:$<BOOL:${MINGW}>,bin,lib>
      RUNTIME DESTINATION bin)
endif()
