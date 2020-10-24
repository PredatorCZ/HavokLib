project(havokpy VERSION 1.0)

file(GLOB_RECURSE PYTHON_SOURCE_FILES "*.cpp")

build_target(
    NAME havokpy
    TYPE PYMODULE
    SOURCES 
        ${PYTHON_SOURCE_FILES}
    LINKS
        HavokLib
)

set_precore_sources(havokpy uni_python)
