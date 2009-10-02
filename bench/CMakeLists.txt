include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})
include_directories(${BDB_INCLUDE_DIR})

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}

)

add_executable(bdb
  bdb.cc
)

add_executable(bdbqueue
  bdbqueue.cc
)

add_executable(bdblookup
  bdblookup.cc
)

add_executable(bdbqueue_cursor
  bdbqueue_cursor.cc
)


add_executable(signalbench
  signalbench.cc
  
  )

add_executable(timedb
  timedb.cc
  
  )

target_link_libraries(bdb
  ${BDB_LIBRARIES}
    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}

)


target_link_libraries(bdbqueue
  ${BDB_LIBRARIES}
    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}

)


target_link_libraries(bdbqueue_cursor
  ${BDB_LIBRARIES}
    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}

)

target_link_libraries(signalbench
    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}
)

target_link_libraries(bdblookup
  ${BDB_LIBRARIES}

    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}
)

target_link_libraries(timedb
    ${Boost_LIBRARIES} 
)

