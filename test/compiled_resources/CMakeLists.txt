get_directory_property(PARENT_DIRECTORY PARENT_DIRECTORY)

function(create_arc arcPath tocData)
  message("Creating resource archive")
  set(arcToc ${arcPath}.toc)
  file(WRITE ${arcToc} ${tocData})

  if(${CMAKE_MINOR_VERSION} LESS 13)
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E tar c ${arcPath}.d --files-from=${arcToc}
      WORKING_DIRECTORY ${PARENT_DIRECTORY})
    execute_process(COMMAND zstd -f10 ${arcPath}.d -o ${arcPath}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar c ${arcPath} --zstd
                            --files-from=${arcToc} WORKING_DIRECTORY ${PARENT_DIRECTORY})
  endif()

  file(REMOVE ${arcToc})
endfunction()

function(extract_arc arcPath)
  message("Extracting resource archive")
  if(${CMAKE_MINOR_VERSION} LESS 13)
    execute_process(COMMAND zstd -fd ${arcPath} -o ${arcPath}.d
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(arcPath ${arcPath}.d)
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar x ${arcPath}
                  WORKING_DIRECTORY ${PARENT_DIRECTORY})
endfunction()

# Determine whenever extract or compress data based on file creation times
function(make_arc dirPath)
  file(GLOB_RECURSE RESOURCE_XML_FILES "${cDir}/*.xml")
  file(GLOB_RECURSE RESOURCE_HKX_FILES "${cDir}/*.hkx")
  file(GLOB_RECURSE RESOURCE_GLB_FILES "${cDir}/*.hk.glb")

  set(MAX_TMSTAMP 0)
  set(RESOURCESTOC "")

  foreach(E ${RESOURCE_XML_FILES} ${RESOURCE_HKX_FILES} ${RESOURCE_GLB_FILES})
    file(TIMESTAMP ${E} SS "%s" UTC)
    string(APPEND RESOURCESTOC "${E}\n")

    if(${SS} GREATER ${MAX_TMSTAMP})
      set(MAX_TMSTAMP ${SS})
    endif()
  endforeach()

  if(RESOURCESTOC)
    get_filename_component(dirName ${cDir} NAME)
    set(RES_ARCHIVE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/${dirName}.tar.zst)
    file(TIMESTAMP ${RES_ARCHIVE_NAME} RES_ARCHIVE_TIMESTAMP "%s" UTC)

    if(NOT EXISTS ${RES_ARCHIVE_NAME})
      set(RES_ARCHIVE_TIMESTAMP 0)
    endif()

    if(MAX_TMSTAMP GREATER RES_ARCHIVE_TIMESTAMP)
      create_arc(${RES_ARCHIVE_NAME} ${RESOURCESTOC})
    else()
      extract_arc(${RES_ARCHIVE_NAME})
    endif()
  endif()
endfunction()

# Get all compressed resources
file(GLOB RES_ARCS "resources*.tar.zst")
list(APPEND extractedArcs)

# Check if they're extracted
foreach(cArc ${RES_ARCS})
  get_filename_component(resName ${cArc} NAME_WE)

  if(NOT EXISTS ${PARENT_DIRECTORY}/${resName})
    extract_arc(${CMAKE_CURRENT_SOURCE_DIR}/${resName}.tar.zst)
    list(APPEND extractedArcs ${resName})
  endif()
endforeach()

file(GLOB RES_DIRS "${PARENT_DIRECTORY}/resources*")

# Filter out already extracted resources
foreach(cArc ${extractedArcs})
  list(FILTER RES_DIRS EXCLUDE REGEX ${cArc})
endforeach()

foreach(cDir ${RES_DIRS})
  make_arc(cDir)
endforeach()
