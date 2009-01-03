#ifndef SINKPAD_H
#define SINKPAD_H

#include <sigc++/sigc++.h>
#include "isinkpad.h"
#include "isourcepad.h"
#include "queueview.h"


namespace core {
  
  template<class BufferT> 
  class SinkPad : public ISinkPad {
  public: 
    static SinkPad<BufferT> * createPad(std::string name); 
    
    sigc::signal<void> & newDataSignal() { return newDataSignal_; }
    sigc::signal<void> & invalidateDataSignal() {return invalidateDataSignal_;}
    sigc::signal<void> & disconnectSignal() {return disconnectSignal_;}
    
    std::string getName() { return name_;} 
    
    inline QueueView<BufferT> * getpQueueView() {
      if (connected_) {
	return connectedQueueView_; 
      } else {
	return NULL;
      }
    }
    
  protected:

    QueueView<BufferT> *  connectedQueueView_; 
    
    SinkPad(std::string name) :
      name_(name), 
      connected_(false), 
      newDataSignal_(), 
      invalidateDataSignal_(), 
      disconnectSignal_()
    {
      
    }

    sigc::signal<void> newDataSignal_; 
    sigc::signal<void> invalidateDataSignal_; 
    sigc::signal<void> disconnectSignal_; 

    void setSource(SourcePad<BufferT> * src); 
    std::string name_; 

    bool connected_; 
    void newData() { 
      newDataSignal_.emit(); 
    } 
    void invalidateData() { invalidateDataSignal_.emit(); }

    friend class SourcePad<BufferT>; 

  }; 
  

  template<class BufferT>
  SinkPad<BufferT> * SinkPad<BufferT>::createPad(std::string name) {
    return new SinkPad<BufferT>(name); 
  }

  template<class BufferT>
  void SinkPad<BufferT>::setSource(SourcePad<BufferT>* src)
  {
    if (connected_) {
      disconnectSignal_.emit(); 
    }
    
    connectedQueueView_ = src->getQueueView(); 
    
    connected_ = true; 
  }


}

#endif // SINKPAD_H
