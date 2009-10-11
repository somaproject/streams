link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(vis_SRCS
#  sinevis.cc
  visfactory.cc
)


add_library(vis
  ${vis_SRCS}
#  ${sine_vis_SRCS}
  )

target_link_libraries(vis
#  vis_wave
  vis_wave2
  vis_spect
  ${Boost_LIBRARIES} 
  ${GUIDEPS_LIBRARIES}
  ${SOMADEPS_LIBRARIES}
)

#add_subdirectory(wavevis)
add_subdirectory(wavevis2)
add_subdirectory(spectvis)
