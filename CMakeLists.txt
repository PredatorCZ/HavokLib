cmake_minimum_required(VERSION 3.3)

project(HavokLib)

if (NOT DEFINED UNICODE)
	set (UNICODE TRUE)
endif()

if (NOT DEFINED RELEASEVER)
	set(RELEASEVER FALSE)
endif()

if (RELEASEVER)
	set(CMAKE_BUILD_TYPE Release)
else()
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 11)

add_library(HavokLib STATIC 
3rd_party/PreCore/datas/MasterPrinter.cpp
3rd_party/PreCore/datas/reflector.cpp
3rd_party/PugiXml/src/pugixml.cpp
source/hkaDeltaDecompressor.cpp
source/hkaSplineDecompressor.cpp
source/hkBin2XML.cpp
source/hkCore.cpp
source/hkHeader.cpp
source/hkNewHeader.cpp
source/hkRegistry.cpp
)

include_directories(include/)
include_directories(3rd_party/PreCore/)
include_directories(3rd_party/PugiXml/src/)

if (WIN32 AND UNICODE)
	message(STATUS "Compiling with wchar_t")
	add_definitions(-D_UNICODE -DUNICODE)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof")
endif()