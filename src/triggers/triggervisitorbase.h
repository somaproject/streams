#ifndef TRIGGERVISITORBASE_H
#define TRIGGERVISITORBASE_H

#include "itriggervisitor.h"
#include "triggerbase.h"
/*
  TriggerVisitorBase

  A placeholder trigger visitor that can be overridden by
  children that don't want to explicitly instantiate
  all of the visit methods. 

*/
class TriggerVisitorBase: public ITriggerVisitor 
{
public: 
  virtual void visit(TriggerBase * ) {}; 
      
}; 


#endif // TRIGGERVISITOR_H
