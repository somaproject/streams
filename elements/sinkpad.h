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
class ISource;

template<class BufferT> 
class SinkPad : public ISinkPad {
public: 
  
  typedef boost::shared_ptr<SinkPad> pSinkPad_t; 
  
  static pSinkPad_t createPad(std::string name) {
    return pSinkPad_t(new SinkPad(name)); 
  }
  
  std::string getName() { return name_;} 
  
  void src_dirty(const timewindow_t & tw){
    // the connected source says it's dirty
    dirtyqueue_.send(tw); 
  }
  
   std::list<BufferT> get_src_data(const timewindow_t & tw) {
    return src_.get_data(tw); 
  }
  
protected:
  
  SinkPad(std::string name) :
    name_(name), 
      connected_(false), 
    src_(0)
  {
    
  }
  
  std::string name_; 
  
  bool connected_; 
  
  public:
  ISource<BufferT> * src_; 
  NaiveQueue<timewindow_t> dirtyqueue_; 
  
}; 

}

#endif // SINKPAD_H
