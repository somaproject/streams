#ifndef ISOURCEVISITOR_H
#define ISOURCEVISITOR_H

#include <data/wave.h>

// Forward Declarations
class SourceBase; 
class NoiseWave2; 
// class NetDataWave; 
// class NetDataRaw; 
class PulseAudioMonitorWave; 

class ISourceVisitor {
  
public:
  virtual void visit(SourceBase * ) = 0; 
  virtual void visit(NoiseWave2 * ) = 0; 
//   virtual void visit(NetDataWave *) = 0; 
//   virtual void visit(NetDataRaw *) = 0; 
  virtual void visit(PulseAudioMonitorWave *) = 0; 

}; 

typedef boost::shared_ptr<ISourceVisitor> pISourceVisitor_t; 


#endif //ISOURCEVISITOR_H
