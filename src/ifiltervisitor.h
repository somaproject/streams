#ifndef IFILTERVISITOR_H
#define IFILTERVISITOR_H

#include <data/wave.h>

// Forward Declaration
class FilterBase; 

class IFilterVisitor {
public:
  virtual void visit(FilterBase * ) = 0; 
}; 

typedef boost::shared_ptr<IFilterVisitor> pIFilterVisitor_t; 

#endif //IFILTERVISITOR_H
