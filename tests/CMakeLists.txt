include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})
include_directories(${BDB_INCLUDE_DIR})

link_directories(
  ${Boost_LIBRARY_DIRS}
  ${GUIDEPS_LIBRARY_DIRS}
  ${SOMADEPS_LIBRARY_DIRS}
)

add_executable(queueview_test
  queueview_test.cc
  runner.cc)

add_executable(sourcepad_test
  sourcepad_test.cc
  runner.cc)


add_executable(sourcesink_test
  sourcesink_test.cc
  runner.cc)

add_executable(element_test
  element_test.cc
  runner.cc)

add_executable(networkdatacache_test
  networkdatacache_test.cc
  mocksomanetcodec.cc
  runner.cc)

add_executable(pipelinemanager_test
  pipelinemanager_test.cc
  runner.cc)

add_executable(bdbqueueview_test
  bdbqueueview_test.cc
  runner.cc)

add_executable(runtest
  runtest.cc
  mocksomanetcodec.cc

)

add_executable(pipelineconfig_test
  pipelineconfig_test.cc
  util.cc
  runner.cc)


# add_executable(pasource_test
#   pasource_test.cc
#   runner.cc)

add_executable(spectvis_test
  spectvis_test.cc
  runner.cc)

add_executable(timedb_test
  timedb_test.cc
  runner.cc)

add_executable(timedb_cursor_test
  timedb_cursor_test.cc
  runner.cc)



add_executable(valgrind
  valgrind.cc
)

# next-generation sequence stuff
add_executable(sourcepad2_test
  sourcepad2_test.cc
  runner.cc)

add_executable(sourcesink2_test
  sourcesink2_test.cc
  runner.cc)

add_executable(property_test
  property_test.cc
  runner.cc)

add_executable(noisewave2_test
  noisewave2_test.cc
  runner.cc)



  target_link_libraries(runtest
    streams_core
    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}
    ${BDB_LIBRARIES}
    json_spirit
    )
  

  target_link_libraries(valgrind
    ${Boost_LIBRARIES} 

    )
  



set(testlist 
  queueview_test
  sourcepad_test
  sourcesink_test
  element_test
  networkdatacache_test
  pipelinemanager_test
  bdbqueueview_test
  pipelineconfig_test
  #pasource_test
  spectvis_test
  timedb_test
  timedb_cursor_test


  sourcepad2_test
  sourcesink2_test
  property_test
  noisewave2_test
)


#SET(TEST_DATA_PATH ${PROJECT_SOURCE_DIR}/tests)
#CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/tests/test_config.h.in
#  ${PROJECT_BINARY_DIR}/tests/test_config.h)

# add the binary tree to the search path for include files
# so we will find this generated include file
include_directories("${PROJECT_BINARY_DIR}/tests")

foreach(testname ${testlist})
  
  target_link_libraries(${testname}
    streams_core
    elements

    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}
    ${BDB_LIBRARIES}
    json_spirit
    )
  
  ADD_TEST(${testname} ${testname})

endforeach(testname)


