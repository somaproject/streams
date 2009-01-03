#ifndef TESTVISCONTROL_H
#define TESTVISCONTROL_H

#include <boost/shared_ptr.hpp>
#include "istreamrenderer.h" 
#include "itriggerrenderer.h" 

#include "ivis.h"
#include "viscontrol.h" 

/*
   A simple test vis control for simple wire-ups of elements
   and 



*/

class TestVisControl : public VisControl
{
public:
  TestVisControl(); 
  
  std::list<pIVis_t> getVisList() {
    return visList_; 
  }
  
  void addVis(pIVis_t iv); 

private:
  std::list<pIVis_t> visList_; 
 
}; 

#endif // TESTVISCONTROL_H
