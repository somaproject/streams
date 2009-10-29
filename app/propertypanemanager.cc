#include "propertypanemanager.h"
#include <sources/noisewave2.h>
#include <vis/wavevis2/wavevis2.h>
#include <vis/spectvis/spectvis.h>
#include <sources/netdatawave/netdatawave.h>
#include "noisewaveproperty.h"
#include "wavevisproperty.h"
// #include "netdatawaveproperty.h"
// #include "netdatarawproperty.h"
// #include "spectvisproperty.h"

PropertyPaneManager::PropertyPaneManager(Gtk::Window & parent, 
					 pSomaConfig_t sc) : 
  window_(), 
  inConflict_(false), 
  proppane_(NULL), 
  pSomaConfig_(sc)
{
  window_.set_transient_for(parent); 
}


PropertyPaneManager::~PropertyPaneManager()
{
  
}

void PropertyPaneManager::setElement(elements::IElement * elt)
{
  // remove all elements, and set this as the only one
  signalClearAllElements_.emit(); 
  appendElement(elt); 
}

void PropertyPaneManager::appendElement(elements::IElement* elt)
{
  if (elements_.find(elt) == elements_.end())
    {
      elements_.insert(elt); 
      if (!inConflict_) {
	// if we're in confict, adding more elements isn't going to
	// change that
	
	if( proppane_ == NULL) {
	  // there is no current property pane
	  // create appropriate property pane -- how the hell do we find
	  // this without horrible dynamic casts? VISITORS, with maybe
	  // a few painful dynamic casts along the way
	  proppane_ = createPropPane(elt); 
	  
	  window_.add(*proppane_); 
	  window_.show_all(); 
	  proppane_->addElement(elt); 
	  
	} else {
	  
	  // there is currently a property pane
	  if (proppane_->addElement(elt)) {
	    // successful add, do nothing
	    
	  } else {
	    inConflict_ = true; 
	    window_.remove();
	    delete proppane_; 
	    
	  }
	  
	}
	
      } else {
      }
      
    } else {
    }
  if (!elements_.empty()) {
    window_.show_all(); 
  }
}


void PropertyPaneManager::removeElement(elements::IElement * elt)
{
  if (elements_.find(elt) == elements_.end())
    {
      throw std::runtime_error("element is not currently in set"); 
    }

  elements_.erase(elt); 

  if (!inConflict_) {
    assert(proppane_ != NULL); 
    proppane_->delElement(elt); 
    if (elements_.size() == 0) {
      window_.remove(); 
      delete proppane_; 
      proppane_ = NULL; 
    }
  } else {
    // currently in conflict; this might have removed the conflict. 
    bool firstelt = true; 
    inConflict_ = false; 
    for(std::set<elements::IElement* >::iterator ei = elements_.begin(); 
	ei != elements_.end(); ++ei) {
      if (firstelt) {
	firstelt = false; 
	proppane_ = createPropPane(*ei); 
	window_.add(*proppane_); 
	window_.show_all(); 
      } else {
	if(not inConflict_) {
	  bool success = proppane_->addElement(*ei); 
	  if (!success) {
	    inConflict_ = true; 
	    window_.remove(); 
	    delete proppane_; 
	    proppane_ = NULL; 
	  }
	  
	}
      }
    }
  }

  if (elements_.empty()) {
    window_.hide();       
  }
}


PropertyPane * PropertyPaneManager::createPropPane(elements::IElement * elt)
{
  // TOTAL HORRIBLE HACK
  if (dynamic_cast<NoiseWave2*>(elt)) {
    return new NoiseWaveProperty(pSomaConfig_); 
  } else if (dynamic_cast<WaveVis2*>(elt)) {
    return new WaveVisProperty(pSomaConfig_); 
//   } else if (dynamic_cast<SpectVis*>(elt)) {
//     return new SpectVisProperty(pSomaConfig_); 
//   } else if (dynamic_cast<NetDataWave*>(elt)) {
//     return new NetDataWaveProperty(pSomaConfig_); 
//   } else if (dynamic_cast<NetDataRaw*>(elt)) {
//     return new NetDataRawProperty(pSomaConfig_); 
  } else {
    throw std::runtime_error("unkown element class for property pane creation");
  }

}
