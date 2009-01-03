#ifndef ISOURCE_H
#define ISOURCE_H

#include <somanetwork/network.h>

#include <boost/shared_ptr.hpp>
#include <sigc++/sigc++.h>

#include <list>

#include "core/ielement.h" 

class ISourceVisitor; 

class ISource : public virtual core::IElement
{
  
public:
  virtual void accept(boost::shared_ptr<ISourceVisitor>) = 0; 

protected: 

};
    
typedef boost::shared_ptr<ISource> pISource_t; 

#endif // ISOURCE_H

