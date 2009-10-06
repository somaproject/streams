#ifndef VISVISITORBASE_H
#define VISVISITORBASE_H

#include "ivisvisitor.h"

/*
  VisVisitorBase

  A placeholder vis visitor that can be overridden by
  children that don't want to explicitly instantiate
  all of the visit methods. 

*/
class VisVisitorBase: public IVisVisitor 
{
public: 
  virtual void visit(VisBase * ){}; 
  //virtual void visit(Vis2 * ){}; 
  virtual void visit(WaveVis2 * ){}; 
  

  
}; 


#endif // VISVISITOR_H
