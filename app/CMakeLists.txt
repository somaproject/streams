link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

set(app_gui_SRCS 
  streamsapp.cc
  pipelinemanagergui.cc
  pipelinegui.cc
  filtercreator.cc
  triggercreator.cc
  sourcecreator.cc
  viscreator.cc
  propertypanemanager.cc
  elementstatus.cc
  somaconfig.cc)

add_library(app_gui
  ${app_gui_SRCS})

set(app_widgets_SRCS
  noisewavewidget.cc
  pamonitorwidget.cc
  waveviswidget.cc
  netdatawavewidget.cc
  netdatarawwidget.cc)

add_library(app_widgets
  ${app_widgets_SRCS})

set(app_props_SRCS
  noisewaveproperty.cc
  wavevisproperty.cc
  netdatawaveproperty.cc
  netdatarawproperty.cc)

add_library(app_props
  ${app_props_SRCS})

add_executable(streams
  streamsmain.cc

  )

target_link_libraries(streams
  app_gui
  app_widgets
  app_props
  propwidgets
  streams_core)

add_subdirectory(propwidgets)
