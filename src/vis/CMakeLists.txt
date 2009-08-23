link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(vis_SRCS
  sinevis.cc 
  wavevis.cc 
  wavevisrenderer.cc 
  visfactory.cc)


add_library(vis
  ${vis_SRCS})

