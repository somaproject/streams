link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(netdatawave_SRCS
  netdatawave.cc
  )

add_library(source_netdatawave
  ${netdatawave_SRCS})


target_link_libraries(source_netdatawave
  streams_core
  ${GUIDEPS_LIBRARIES}
  ${GUIDEPS_LIBRARIES}

)
  

  
