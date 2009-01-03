#include "testviscontrol.h"

TestVisControl::TestVisControl()
{
  

}

void TestVisControl::addVis(pIVis_t iv) {
  // 
  visList_.push_back(iv); 

  connectVis(iv); 
  
}
