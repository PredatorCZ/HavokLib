project(HavokToolset)
set_target_properties(spike_cli PROPERTIES OUTPUT_NAME havok_toolset)
target_link_libraries(spike_cli havok-objects)

include(version)
toolset_version()

add_spike_subdir(gltf)
