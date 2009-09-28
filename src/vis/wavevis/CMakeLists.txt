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
add_library(vis_wave
  ${wave_vis_SRCS}
  )

target_link_libraries(vis_wave
  ${Boost_LIBRARIES} 
  ${GUIDEPS_LIBRARIES}
  ${SOMADEPS_LIBRARIES}
)
