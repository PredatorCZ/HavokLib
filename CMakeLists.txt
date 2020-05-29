cmake_minimum_required(VERSION 3.3)

project(HavokLib)

if(NOT TARGETEX_LOCATION)
	set(TARGETEX_LOCATION 3rd_party/PreCore/cmake)
	include(${TARGETEX_LOCATION}/targetex.cmake)
	include(${TARGETEX_LOCATION}/chartype.cmake)
endif()

add_subdirectory(3rd_party/PreCore)

set(CMAKE_CXX_STANDARD 11)
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
)

set_precore_sources(master_printer matrix44 reflector reflector_xml)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
	enable_testing()
	add_test(NAME HavokLibTesting COMMAND HavokLibTesting 
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/resources)
endif()
