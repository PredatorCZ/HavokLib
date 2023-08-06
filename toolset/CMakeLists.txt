project(HavokToolset)
set_target_properties(spike_cli PROPERTIES OUTPUT_NAME havok_toolset)

include(version)
toolset_version()

add_spike_subdir(gltf)
