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
  noisewave.cc
  noisewaveconfig.cc
  pamonitor.cc
  sourcefactory.cc
  netdatawave.cc
  netdataraw.cc
  pasource.cc
  )

add_library(sources
  ${sources_SRCS})

set(sources2_SRCS
  noisewave2.cc
  )

add_library(sources2
  ${sources2_SRCS}
)

target_link_libraries(sources
  streams_core
  ${GUIDEPS_LIBRARIES}
  ${GUIDEPS_LIBRARIES}

)
  
target_link_libraries(sources2
  ${GUIDEPS_LIBRARIES}
  ${GUIDEPS_LIBRARIES}
)
  
