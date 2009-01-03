#ifndef TRIGGERCREATOR_H
#define TRIGGERCREATOR_H

#include "itriggervisitor.h"

class PipelineGUI;
class TriggerCreator : public ITriggerVisitor
{
public:
  TriggerCreator(PipelineGUI &); 
  ~TriggerCreator(); 

  void visit(TriggerBase  * ); 

private:
  PipelineGUI & pipelineGUI_; 

};

typedef boost::shared_ptr<TriggerCreator> pTriggerCreator_t; 


#endif // TRIGGERCREATOR_H
