cmake_minimum_required(VERSION 3.3)

project(HavokLibTesting)

file(GLOB_RECURSE RESOURCE_XML_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.xml")
file(GLOB_RECURSE RESOURCE_HKX_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.hkx")

set(MAX_TMSTAMP 0)

foreach(E ${RESOURCE_XML_FILES} ${RESOURCE_HKX_FILES})
	file(TIMESTAMP ${E} SS "%s" UTC)

	if(${SS} GREATER ${MAX_TMSTAMP})
		set(MAX_TMSTAMP ${SS})
	endif()
endforeach()


set(RES_ARCHIVE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/resources.tar.zst)
file(TIMESTAMP ${RES_ARCHIVE_NAME} RES_ARCHIVE_TIMESTAMP "%s" UTC)

if(NOT EXISTS ${RES_ARCHIVE_NAME})
	set(RES_ARCHIVE_TIMESTAMP 0)
endif()

if(MAX_TMSTAMP GREATER RES_ARCHIVE_TIMESTAMP)
	message("Creating resource archive")
	execute_process(COMMAND
		${CMAKE_COMMAND} -E tar c ${RES_ARCHIVE_NAME} --zstd --
		${RESOURCE_XML_FILES} ${RESOURCE_HKX_FILES}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
	message("Extracting resource archive")
	if(${CMAKE_MINOR_VERSION} LESS 13)
		execute_process(COMMAND zstd -d ${RES_ARCHIVE_NAME} -o ${RES_ARCHIVE_NAME}.d)
		set(RES_ARCHIVE_NAME ${RES_ARCHIVE_NAME}.d)
	endif()
	execute_process(COMMAND
		${CMAKE_COMMAND} -E tar x ${RES_ARCHIVE_NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(STATUS "Architecture: X86")
else()
	message(STATUS "Architecture: X64")
endif()

include(${PRECORE_SOURCE_DIR}/cmake/targetex.cmake)
include(${PRECORE_SOURCE_DIR}/cmake/chartype.cmake)
set(CMAKE_CXX_STANDARD 11)

build_target(
	TYPE APP
	SOURCES 
		test.cpp
	INCLUDES
		../include
		../3rd_party/PugiXml/src
	LINKS
		HavokLib
	NO_PROJECT_H
	NO_VERINFO
)

set_precore_sources(master_printer)
