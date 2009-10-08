#ifndef SOURCEVISITORBASE_H
#define SOURCEVISITORBASE_H

#include "isourcevisitor.h"

/*
  SourceVisitorDummy

  A placeholder source visitor that can be overridden by
  children that don't want to explicitly instantiate
  all of the visit methods. 

*/
class SourceVisitorBase: public ISourceVisitor 
{
public: 
  //virtual void visit(SourceBase *  sb) {}; 
  virtual void visit(NoiseWave2 * nw) {}; 
  virtual void visit(PulseAudioMonitorWave * nw) {}; 
  //  virtual void visit(NetDataWave * ndw) {}; 
}; 



#endif // SOURCEVISITOR_H
