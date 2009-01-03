#ifndef FILTERCREATOR_H 
#define FILTERCREATOR_H

#include "ifiltervisitor.h"

class PipelineGUI; 

class FilterCreator : public IFilterVisitor
{
public:
  FilterCreator(PipelineGUI & ); 
  ~FilterCreator(); 
  void visit(FilterBase * ); 

private:
  PipelineGUI & pipelineGUI_; 

}; 

typedef boost::shared_ptr<FilterCreator> pFilterCreator_t; 

#endif // FILTERCONTROL_H
