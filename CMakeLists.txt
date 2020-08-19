cmake_minimum_required(VERSION 3.12)

project(HavokLib)

option(PYTHON_MODULE "Build a module for Python 2.7 (enables PIC).")
option(USE_PIC "Build static library as PIC (when used for shared library).")

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(3rd_party/PreCore)

include(${PRECORE_SOURCE_DIR}/cmake/targetex.cmake)
include(${PRECORE_SOURCE_DIR}/cmake/chartype.cmake)

if (${PYTHON_MODULE})
	if (NOT ${RELEASEVER})
		message(FATAL_ERROR "Cannot build python module in debug mode!")
	endif()

	find_package(Python2 REQUIRED COMPONENTS Development Interpreter)
elseif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR} AND ${RELEASEVER})
	find_package(Python2 COMPONENTS Development Interpreter)
endif()

if (${Python2_FOUND})
	set(USE_PIC ON)
endif()

file(GLOB_RECURSE CORE_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

build_target(
	TYPE STATIC
    SOURCES
		${CORE_SOURCE_FILES}
		3rd_party/PugiXml/src/pugixml.cpp
    INCLUDES
		src
		include
		3rd_party/PugiXml/src
	NO_PROJECT_H
	NO_VERINFO
	PIC ${USE_PIC}
)

set_precore_sources(master_printer matrix44 reflector reflector_xml)

if (${Python2_FOUND})
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/python)
endif()

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
	enable_testing()
	add_test(NAME HavokLibTesting COMMAND HavokLibTesting 
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/resources)

	if (${Python2_FOUND})
		add_test(NAME HavokPyTesting COMMAND ${Python2_EXECUTABLE} havok.py
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
	endif()
endif()
