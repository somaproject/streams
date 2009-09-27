#include <stdio.h>
#include "util.h"

bf::path create_tempdir()
{
  char foo[] = "/tmp/testXXXXXX"; 
  std::string sdir(mkdtemp(foo)); 
  bf::path tmpfile(sdir);
  create_directories(tmpfile); 
  return tmpfile; 
  
}


