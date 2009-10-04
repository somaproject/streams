#ifndef __ELEMENTS_SINKPAD_H__
#define __ELEMENTS_SINKPAD_H__

#include "isinkpad.h"
#include "isourcepad.h"
#include "commands.h"


namespace elements {
  
  template<class BufferT> 
  class SinkPad : public ISinkPad {
  public: 
    static pSinkPad_t<BufferT> createPad(std::string name) {
      return new pSinkPad_t<BufferT>(name); 
    }
    
    std::string getName() { return name_;} 
    
  protected:
    
    SinkPad(std::string name) :
      name_(name), 
      connected_(false)
    {
      
    }
    
    void SinkPad<BufferT>::setSource(pSourcePad_t<BufferT> src)
    {
      if(connected_) { 
	connected_ = false; 
	commandqueue_.send(DISCONNECT); 
      } 

      connected_ = true; 
      commandqueue_.send(CONNECT); 

    }
    
    std::string name_; 
    
    bool connected_; 
    
    friend class SourcePad<BufferT>; 

    commandqueue commandqueue_; 
    
    pTimeSeriesDatabase_t<bufferT> pendingTimeSeriesDatabase_; 
    pTimeSeriesDatabase_t<bufferT> activeTimeSeriesDatabase_; 
    
  }; 
  
  





}

#endif // SINKPAD_H
