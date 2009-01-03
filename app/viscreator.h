#ifndef VISCREATOR_H
#define VISCREATOR_H

#include "ivisvisitor.h"
#include "propertypanemanager.h"

class PipelineGUI; 
class VisCreator : public IVisVisitor
{
public:
  VisCreator(PipelineGUI &); 
  ~VisCreator(); 
  
  void visit(VisBase * ); 
  void visit(SineVis * ); 
  void visit(WaveVis * ); 


private:
  PipelineGUI & pipelineGUI_; 


}; 

typedef boost::shared_ptr<VisCreator> pVisCreator_t; 

#endif // VISCREATOR_H
