add_subdirectory(compiled_resources)

build_target(
  NAME
  test_main
  TYPE
  APP
  SOURCES
  test.cpp
  LINKS
  HavokLibStatic
  NO_PROJECT_H
  NO_VERINFO)

set_precore_sources(test_main)

add_test(
  NAME test_main
  COMMAND test_main
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)

if(${Python2_FOUND})
  add_test(
    NAME havokpy_test
    COMMAND ${Python2_EXECUTABLE} havok.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(ODR_TEST)
  include(${PRECORE_SOURCE_DIR}/cmake/odr_test.cmake)
  test_odr(PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  set_precore_sources(test_odr)
endif()
