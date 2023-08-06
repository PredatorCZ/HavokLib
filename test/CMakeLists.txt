add_subdirectory(compiled_resources)

build_target(
  NAME
  test_main
  TYPE
  APP
  SOURCES
  test.cpp
  LINKS
  havok-objects
  spike-objects
  pugixml-objects
  NO_PROJECT_H
  NO_VERINFO)

add_test(
  NAME test_main
  COMMAND test_main
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)

if(PYTHON_MODULE)
  add_test(
    NAME havokpy_test
    COMMAND ${Python3_EXECUTABLE} havok.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(ODR_TEST)
  include(odr_test)
  test_odr(PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_link_libraries(test_odr spike-interface)
endif()
