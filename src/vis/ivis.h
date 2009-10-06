#ifndef IVIS_H
#define IVIS_H
#include <boost/shared_ptr.hpp>
#include <map>
#include <iostream>

#include <elements/ielement.h>
#include "istreamrenderer.h"
#include "itriggerrenderer.h" 

// this is where we typedef the relevent typedefs
//typedef sigc::signal<void, bool> updateTriggersSignal_t; 
class IVisVisitor; 

class IVis : public virtual elements::IElement,  
	     public IStreamRenderer, public ITriggerRenderer
{
  
public:
  
  virtual int getPixelHeight() = 0; // hight of the vis element in pixels
  virtual void setPixelHeight(int ) = 0; 
  // getTriggerSink(); // I think we want this, but not RIGHT NOW
  virtual void accept(boost::shared_ptr<IVisVisitor>) = 0; 
}; 

typedef boost::shared_ptr<IVis> pIVis_t; 

#endif // IVIS_H
