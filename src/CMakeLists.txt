link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})
include_directories(logging/src)
include_directories(logging/boost)


set(streams_SRCS
  glconfig.cc 
  timer.cc
  sourcebase.cc
  streampipeline.cc
  somanetcodec.cc
  networkdatacache.cc
  pipelinemanager.cc)

add_library(streams_core
  ${streams_SRCS})

target_link_libraries(streams_core
    ${Boost_LIBRARIES} 
  ${GUIDEPS_LIBRARIES}
  ${SOMADEPS_LIBRARIES}
)

set(logging_SRCS
  ext-logging/src/logging.cc
)



add_subdirectory(core)
add_subdirectory(sources)
add_subdirectory(triggers)
add_subdirectory(vis)
