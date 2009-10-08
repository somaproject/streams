link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(pasources_SRCS
  pamonitor.cc
  pasource.cc
  )

add_library(source_pamonitor
  ${pasources_SRCS})


target_link_libraries(source_pamonitor
  streams_core
  ${GUIDEPS_LIBRARIES}
  ${GUIDEPS_LIBRARIES}

)
  

  
