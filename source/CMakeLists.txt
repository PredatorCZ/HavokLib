file(GLOB_RECURSE CORE_SOURCE_FILES "*.cpp")

add_library(havok-interface INTERFACE)
target_include_directories(havok-interface INTERFACE ../include)
target_link_libraries(havok-interface INTERFACE spike-interface)

if(NOT NO_OBJECTS)
  build_target(
    NAME
    havok-objects
    TYPE
    OBJECT
    SOURCES
    ${CORE_SOURCE_FILES}
    PROPERTIES
    POSITION_INDEPENDENT_CODE ${OBJECTS_PID}
    INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
    LINKS
    spike-objects
    havok-interface
    NO_VERINFO
    NO_PROJECT_H)

    target_compile_options(havok-objects PRIVATE -fvisibility=hidden)
    target_expose_defs(havok HK_EXPORT HK_IMPORT)
endif()

if(BUILD_SHARED_LIBS)
  build_target(
    NAME
    havok
    TYPE
    SHARED
    SOURCES
    ${CORE_SOURCE_FILES}
    INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
    LINKS
    spike
    pugixml
    havok-interface
    START_YEAR
    2017
    AUTHOR
    "Lukas Cone"
    NO_PROJECT_H)

  target_compile_definitions(havok PRIVATE HK_EXPORT)

  if(WIN32 OR MINGW)
    set_target_properties(havok PROPERTIES SUFFIX ${PROJECT_VERSION_MAJOR}.dll
                                           PREFIX "")
  else()
    set_target_properties(havok PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
  endif()

  target_compile_definitions(havok INTERFACE HK_IMPORT)

  install(
    TARGETS havok
    LIBRARY NAMELINK_SKIP DESTINATION $<IF:$<BOOL:${MINGW}>,bin,lib>
    RUNTIME DESTINATION bin)
endif()
