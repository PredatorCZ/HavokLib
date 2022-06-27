add_subdirectory(${TPD_PATH}/PreCore/spike ${CMAKE_CURRENT_BINARY_DIR}/spike)
add_subdirectory(${TPD_PATH}/PreCore/gltf ${CMAKE_CURRENT_BINARY_DIR}/gltf)
set_target_properties(spike PROPERTIES OUTPUT_NAME havok_toolset)

add_spike_subdir(gltf)
