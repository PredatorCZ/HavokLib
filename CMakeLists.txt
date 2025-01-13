cmake_minimum_required(VERSION 3.13)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "X86 architecture is not supported!")
endif()

project(HavokLib VERSION 1)

find_package(Python3 COMPONENTS Development Interpreter)

# ~~~
# Valid targets:
#  - havok-objects: static objects target (enabled by default, set NO_OBJECTS to disable all)
#  - havok: shared library target (BUILD_SHARED_LIBS == ON)
# ~~~

option(PYTHON_MODULE "Build a module for Python 3 (enables OBJECTS_PID)." ON)
option(TOOLSET "Build toolset." ON)
option(ODR_TEST "Enable ODR testing." OFF)

if(NOT Python3_FOUND AND PYTHON_MODULE)
  message(FATAL_ERROR "Python3 not found.")
endif()

option(OBJECTS_PID "Imply PID for all objects." ${PYTHON_MODULE})

option(NO_OBJECTS "" OFF)
option(CLI "" ${TOOLSET})
option(GLTF "" ${TOOLSET})

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra)

if(TOOLSET)
  set(EXPOSE_SYMBOLS spike;gltf;havok;pugixml)
endif()

set(TPD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party)
add_subdirectory(${TPD_PATH}/spike)
include(targetex)
add_subdirectory(classgen)
add_subdirectory(source)

if(TOOLSET)
  add_subdirectory(toolset)
endif()

if(PYTHON_MODULE)
  add_subdirectory(python)
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  enable_testing()
  add_subdirectory(test)
endif()
