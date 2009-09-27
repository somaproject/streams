#ifndef __TEST_UTIL_H__
#define __TEST_UTIL_H__

#include <boost/filesystem.hpp>

namespace bf = boost::filesystem; 

bf::path create_tempdir(); 

#endif
