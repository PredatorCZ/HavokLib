project(havokpy)

include(version)

file(GLOB_RECURSE PYTHON_SOURCE_FILES "*.cpp")

python3_add_library(havokpy MODULE ${PYTHON_SOURCE_FILES}
                    ${SPIKE_SOURCE_DIR}/src/uni_python.cpp)
target_link_libraries(havokpy PRIVATE spike-objects havok-objects pugixml-objects)

target_compile_definitions(havokpy PRIVATE havokpy_VERSION="${PROJECT_VERSION}")

target_version(havokpy 1 TO_FILE)

install(
    TARGETS havokpy
    LIBRARY NAMELINK_SKIP DESTINATION python_module/lib)
