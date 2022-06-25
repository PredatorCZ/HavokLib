project(havokpy VERSION 1)

file(GLOB_RECURSE PYTHON_SOURCE_FILES "*.cpp")

python3_add_library(havokpy MODULE ${PYTHON_SOURCE_FILES}
                    ${PRECORE_SOURCE_DIR}/uni/python/uni_python.cpp)
target_link_libraries(havokpy PRIVATE precore-objects havok-objects pugixml-objects)

target_compile_definitions(havokpy PRIVATE havokpy_VERSION="${PROJECT_VERSION}")
