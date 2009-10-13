
set(propwidget_SRCS
  spinbutton.cc
#  comboboxentry.cc
  togglebutton.cc
  checkbox.cc
  hscale.cc
)

add_library(propwidgets
  ${propwidget_SRCS})

add_executable(testwidget
  test.cc)

target_link_libraries(testwidget
  propwidgets
    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}
)
