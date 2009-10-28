link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})
include_directories(${BDB_INCLUDE_DIR})
include_directories(logging/src)
include_directories(logging/boost)

set(streams_SRCS
  glconfig.cc 
  timer.cc
  viscontrol.cc
  streamrenderwin.cc
  streampipeline.cc
  somanetcodec.cc
  networkdatacache.cc
  pipelinemanager.cc
  pipelineconfig.cc
  elementrunner.cc
  jsonutil)

set(ext_SRCS
  ext-common/cairogltext/glstring.cc
  ext-common/shaderutil/shaders.cc
)

set(logging_SRCS
  logging/src/logging.cc
)

add_library(streams_core
  ${streams_SRCS}
  ${ext_SRCS}
  ${logging_SRCS}
)


target_link_libraries(streams_core
    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}
    ${BDB_LIBRARIES}
    sources
#    vis
    json_spirit
#    triggers
#    filters
)



#add_subdirectory(core)
#add_subdirectory(sources)
#add_subdirectory(triggers)
#add_subdirectory(vis)
#add_subdirectory(filters)
