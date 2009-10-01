#include <iostream>
#include <cassert>
#include "timedb.h"

using namespace datastore; 

int main()
{


  TimeSeriesDataBase<std::string> db; 
  
  db.newSequence(10, "E"); 
  db.append(11, "F"); 
  db.append(12, "G"); 
  
  TimeCursor<std::string> tc = db.createCursor(8); 
  
  tc.valid();
  assert(tc.requestTime() ==  8); 

  assert(tc.startTime() == 10); 
  std::string x; 

  assert(tc.getCurrentValue(&x) ==  10); 
  assert(tc.next() ==  CURSOR_RET_GOOD); 
  std::cout << "DONE" << std::endl;
}
