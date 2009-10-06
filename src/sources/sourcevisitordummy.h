#ifndef SOURCEVISITORDUMMY_H
#define SOURCEVISITORDUMMY_H

#include "isourcevisitor.h"

namespace soma {
  namespace streams {
    
    class SourceVisitorDummy: public ISourceVisitor 
    {
      virtual void visit(WaveSourceBase & sb) {} ; 
      virtual void visit(NoiseWave &  nw) {} ; 

      
      
    }; 

  }

}


#endif // SOURCEVISITOR_H
