link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(sources_SRCS
  sourcebase.cc
  noisewave2.cc
#  noisewaveconfig.cc

  sourcefactory.cc
#   netdataraw.cc

  )

add_library(sources
  ${sources_SRCS})


target_link_libraries(sources
  streams_core
  source_pamonitor
  source_netdatawave
  ${GUIDEPS_LIBRARIES}
  ${GUIDEPS_LIBRARIES}

)

add_subdirectory(pamonitor)
add_subdirectory(netdatawave)
  

  
