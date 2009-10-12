#ifndef SOURCECREATOR_H
#define SOURCECREATOR_H

#include <sources/isourcevisitor.h>

class PipelineGUI; 
class SourceCreator : public ISourceVisitor
{
public: 
  SourceCreator(PipelineGUI & ); 
  ~SourceCreator(); 
  
  void visit(SourceBase * ); 
  void visit(NoiseWave2 * ); 
  void visit(NetDataWave *); 
//   void visit(NetDataRaw *); 
  void visit(PulseAudioMonitorWave *); 


  
private:
  PipelineGUI & pipelineGUI_; 


}; 

typedef boost::shared_ptr<SourceCreator> pSourceCreator_t; 

#endif // SOURCECREATOR_H
