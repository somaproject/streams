#ifndef FILTERVISITORDUMMY_H
#define FILTERVISITORDUMMY_H

#include "ifiltervisitor.h"
#include "filterbase.h"
/*
  FilterVisitorBase

  A placeholder filter visitor that can be overridden by
  children that don't want to explicitly instantiate
  all of the visit methods. 

*/
class FilterVisitorBase: public IFilterVisitor 
{
public: 
  virtual void visit(FilterBase * ) {} 
  
}; 

#endif // FILTERVISITORBASE_H
