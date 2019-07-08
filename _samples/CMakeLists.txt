cmake_minimum_required(VERSION 3.3)

set(UNICODE TRUE)
set(RELEASEVER FALSE)

add_subdirectory(../ HavokLib/)

project(HavokLibSamples
VERSION 1.0.0)

add_executable(${PROJECT_NAME}
${PROJECT_NAME}.cpp
../3rd_party/PreCore/datas/reflectorXML.cpp
)

target_link_libraries(${PROJECT_NAME} HavokLib)

include_directories(../include/)
include_directories(../3rd_party/PreCore/)
include_directories(../3rd_party/PugiXml/src/)

if (WIN32 AND UNICODE)
	message(STATUS "Compiling with wchar_t")
	add_definitions(-D_UNICODE -DUNICODE)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(STATUS "Architecture: X86")
else()
	message(STATUS "Architecture: X64")
endif()

if (RELEASEVER)
	set(CMAKE_BUILD_TYPE Release)
else()
	set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")