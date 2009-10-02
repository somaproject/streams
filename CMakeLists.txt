
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(streams)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

#IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
#   SET(CMAKE_BUILD_TYPE RelWithDebInfo)
#ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

find_package(Boost 1.37.0 COMPONENTS thread unit_test_framework filesystem program_options system REQUIRED)
find_package(PkgConfig)
find_package(BerkeleyDB REQUIRED)

# we should handle this with build/release distinctions, but I'm not that l33t. 

SET(CMAKE_CXX_FLAGS "-g -O2 ${CMAKE_CXX_FLAGS}")

pkg_check_modules(GUIDEPS REQUIRED 
  sigc++-2.0>=2.0 
  libpulse>=0.9.14
  libpulse-mainloop-glib>=0.9.2
  glibmm-2.4>=2.13.0
  gtkmm-2.4>=2.10
  gtkglextmm-1.2>=1.2
  libglademm-2.4>=2.6
  vsipl++>=2.0
  fftw3>=3.1
)

pkg_check_modules(SOMADEPS REQUIRED
  somanetwork-1.0>=1.0
  somadspio-1.0>=1.0
)

ENABLE_TESTING()

include_directories(src)
include_directories(app)
include_directories(src/ext-common/)
include_directories(json_spirit/json_spirit)

include_directories(src/logging/src)
include_directories(src/logging/boost)
include_directories(timedb)

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(tests)
add_subdirectory(bench)
add_subdirectory(json_spirit)
add_subdirectory(timedb)
