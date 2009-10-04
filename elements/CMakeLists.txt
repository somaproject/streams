link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})
include_directories(${BDB_INCLUDE_DIR})
include_directories(logging/src)
include_directories(logging/boost)

set(elements_SRCS
    commands.cc
)

add_library(elements
  ${elements_SRCS}
)


target_link_libraries(elements
    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}
)


