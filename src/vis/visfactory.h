#ifndef VISFACTORY_H
#define VISFACTORY_H

#include "visbase.h"
#include "ivisvisitor.h" 

/*
  Primary factory function for creating new vis elements

*/ 

class VisFactory {
public : 
  static pIVis_t create(std::string name); 
}; 


#endif //  VISFACTORY_H
