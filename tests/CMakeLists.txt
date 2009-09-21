include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GUIDEPS_INCLUDE_DIRS})
include_directories(${SOMADEPS_INCLUDE_DIRS})

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


set(testlist 
  queueview_test
  sourcepad_test
  sourcesink_test
  element_test
  networkdatacache_test
  pipelinemanager_test)


#SET(TEST_DATA_PATH ${PROJECT_SOURCE_DIR}/tests)
#CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/tests/test_config.h.in
#  ${PROJECT_BINARY_DIR}/tests/test_config.h)

# add the binary tree to the search path for include files
# so we will find this generated include file
include_directories("${PROJECT_BINARY_DIR}/tests")

foreach(testname ${testlist})
  
  target_link_libraries(${testname}
    streams_core
    ${Boost_LIBRARIES} 
    ${GUIDEPS_LIBRARIES}
    ${SOMADEPS_LIBRARIES}

    )
  
  ADD_TEST(${testname} ${testname})

endforeach(testname)

