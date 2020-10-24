cmake_minimum_required(VERSION 3.12)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "X86 architecture is not supported!")
endif()

project(HavokLib)

# ~~~
# Valid targets:
#  - HavokLibStatic: if BUILD_STATIC then it's target for static library,
#                    otherwise this target will contain objects for static libraries or non-PIE executables
#  - HavokLib: objects for shared libraries or PIE executables
#  - HavokLibShared (only if BUILD_SHARED): target for shared library
#
# NOTE: HavokLibStatic and HavokLib are same for WIN
# NOTE: if BUILD_SHARED then HavokLib will have exported symbols (HK_EXPORT)
#
# Definitions:
#  - HK_EXPORT: use to export dynamic symbols from HavokLib target
#  - HK_IMPORT: use to import dynamic symbols from shared library (API only)
# ~~~

option(PYTHON_MODULE "Build a module for Python 2.7 (enables PIC)." OFF)
option(USE_PIC "HavokLib target will use PIC." OFF)
option(ODR_TEST "Enable ODR testing." OFF)
option(BUILD_STATIC "Builds HavokLibStatic as a static library." OFF)
option(BUILD_SHARED "Builds HavokLibShared shared library (enables PIC)." OFF)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(3rd_party/PreCore)
add_subdirectory(3rd_party/PugiXml)

include(${PRECORE_SOURCE_DIR}/cmake/targetex.cmake)
include(${PRECORE_SOURCE_DIR}/cmake/chartype.cmake)

if(${PYTHON_MODULE})
  if(NOT ${RELEASEVER})
    message(FATAL_ERROR "Cannot build python module in debug mode!")
  endif()

  find_package(Python2 REQUIRED COMPONENTS Development Interpreter)
elseif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}
       AND ${RELEASEVER})
  find_package(Python2 COMPONENTS Development Interpreter)
endif()

if(Python2_FOUND OR BUILD_SHARED)
  set(USE_PIC ON)
endif()

if(Python2_FOUND)
  add_subdirectory(python)
endif()

set(STATIC_TYPE OBJECT)

if(BUILD_STATIC)
  set(STATIC_TYPE STATIC)
endif()

add_subdirectory(source)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  enable_testing()
  add_subdirectory(test)
endif()

if(BUILD_SHARED)
  target_compile_definitions(HavokLib PRIVATE -DHK_EXPORT)
  build_target(NAME HavokLibShared TYPE SHARED LINKS HavokLib)
endif()
