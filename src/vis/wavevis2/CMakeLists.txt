link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})


set(wave_vis2_SRCS
  wavevis2.cc 
  renderall.cc

)
add_library(vis_wave2
  ${wave_vis2_SRCS}
  )

target_link_libraries(vis_wave2
  ${Boost_LIBRARIES} 
  ${GUIDEPS_LIBRARIES}
  ${SOMADEPS_LIBRARIES}
)
