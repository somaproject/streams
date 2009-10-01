#include <iostream>
#include <cassert>
#include "timedb.h"

using namespace datastore; 

int main()
{


  TimeSeriesDataBase<std::string> db; 
   db.newSequence(10, "A"); 
  db.append(20, "B"); 
  db.append(30, "C"); 

  db.newSequence(70, "G"); 
  db.append(80, "H"); 
  db.append(90, "I"); 

  db.newSequence(20, "B"); 
  db.append(30, "C"); 
  db.append(40, "D"); 
  db.append(50, "E"); 
  db.append(60, "F"); 
  db.append(70, "G"); 
  db.append(80, "H"); 

}
