link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})


set(spect_vis_SRCS
  spectvis.cc
  spectvisrenderer.cc
  data.cc
  helper.cc
  fftengine.cc
  spectdownsample.cc
  fftwop.cc
)


add_library(vis_spect
  ${spect_vis_SRCS}
  )

target_link_libraries(vis_spect
  ${Boost_LIBRARIES} 
  ${GUIDEPS_LIBRARIES}
  ${SOMADEPS_LIBRARIES}
)
