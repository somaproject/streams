#ifndef __ELEMENTS_SINKPAD_H__
#define __ELEMENTS_SINKPAD_H__

#include "isinkpad.h"
#include "isourcepad.h"
#include "commands.h"
#include "timedb.h"
#include "elementlist.h"

namespace elements {
  using namespace datastore; 

  template<class BufferT> 
  class SinkPad : public ISinkPad {
  public: 

    typedef boost::shared_ptr<SinkPad> pSinkPad_t; 

    static pSinkPad_t createPad(std::string name) {
      return pSinkPad_t(new SinkPad(name)); 
    }
    
    std::string getName() { return name_;} 

    void sendmsg(MESSAGES m)  {
      commandqueue_.send(m); 
    }
    
    void senddata(typename LinkElement<BufferT>::ptr_t data)  {
      dataqueue_.send(data); 
    }
    
  protected:

    SinkPad(std::string name) :
      name_(name)
    {
      
    }
    
    std::string name_; 
    
    bool connected_; 

  public:
    commandqueue_t commandqueue_; 

    typedef LinkElement<BufferT> elt_t; 
    typedef boost::shared_ptr<elt_t> pelt_t; 
    NaiveQueue<pelt_t> dataqueue_; 
    
  }; 
  
  





}

#endif // SINKPAD_H
