cmake_minimum_required(VERSION 3.12)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "X86 architecture is not supported!")
endif()

project(HavokLib VERSION 1)

# ~~~
# Valid targets:
#  - havok-static: static library target (BUILD_STATIC_LIBS == ON OR BUILD_STATIC_OBJECTS == ON)
#  - havok-objects: static objects target (BUILD_STATIC_OBJECTS == ON)
#  - havok-shared: PIC objects target (BUILD_SHARED_OBJECTS == ON OR BUILD_SHARED_LIBS == ON)
#  - havok: shared library target (BUILD_SHARED_LIBS == ON), havok-shared target will have exported symbols
#
# Definitions:
#  - HK_EXPORT: use to export dynamic symbols from HavokLib target (BUILD_SHARED_LIBS == ON)
#  - HK_IMPORT: use to import dynamic symbols from shared library (API only)
# ~~~

option(PYTHON_MODULE "Build a module for Python 2.7 (enables BUILD_SHARED_OBJECTS)." OFF)
option(ODR_TEST "Enable ODR testing." OFF)
option(BUILD_STATIC_OBJECTS "Builds havok-objects target." ON)
option(BUILD_SHARED_OBJECTS "Builds havok-shared target." OFF)
option(BUILD_STATIC_LIBS "Builds havok-static target." OFF)
option(BUILD_SHARED_LIBS "Builds havok target." OFF)
option(PUGI_AS_SHARED "pugixml target will be shared library instead of being embedded." OFF)

set(CMAKE_CXX_STANDARD 20)

set(PUGI_STATIC_OBJECTS ON CACHE BOOL "")

add_subdirectory(3rd_party/PreCore)

include(targetex)

if(${PYTHON_MODULE})
  if(NOT ${RELEASEVER})
    message(FATAL_ERROR "Cannot build python module in debug mode!")
  endif()

  find_package(Python2 REQUIRED COMPONENTS Development Interpreter)
elseif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}
       AND ${RELEASEVER})
  find_package(Python2 COMPONENTS Development Interpreter)
endif()

if(Python2_FOUND)
  set(BUILD_SHARED_OBJECTS ON)
  add_subdirectory(python)
endif()

add_subdirectory(source)

# add_subdirectory(classgen)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  enable_testing()
  add_subdirectory(test)
endif()
