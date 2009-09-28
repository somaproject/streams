link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})


set(wave_vis_SRCS
  wavevis.cc 
  wavevisrenderer.cc 
  renderdownsample.cc
)

set(sine_vis_SRCS
  sinevis.cc 
)

set(spect_vis_SRCS
  spectvis.cc
  spectvisrenderer.cc
)

set(vis_SRCS
  visfactory.cc
)


add_library(vis
  ${vis_SRCS}
  ${wave_vis_SRCS}
  ${sine_vis_SRCS}
  ${spect_vis_SRCS}
  )

target_link_libraries(vis
  ${Boost_LIBRARIES} 

  ${GUIDEPS_LIBRARIES}
  ${SOMADEPS_LIBRARIES}
)
