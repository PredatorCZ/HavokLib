cmake_minimum_required(VERSION 3.3)

project(havokpy VERSION 1.0)

file(GLOB_RECURSE PYTHON_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

build_target(
    TYPE PYMODULE
    SOURCES 
        ${PYTHON_SOURCE_FILES}
    INCLUDES
        ../include
        ../3rd_party/PugiXml/src
    LINKS
        HavokLib
)

set_precore_sources(master_printer uni_python)
