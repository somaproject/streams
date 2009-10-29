#ifndef __ELEMENTS_SINKPAD_H__
#define __ELEMENTS_SINKPAD_H__

#include <elements/isinkpad.h>
#include <elements/isourcepad.h>

namespace elements {

template<class BufferT>
class SourcePad;

template<class BufferT> 
class SinkPad : public ISinkPad {
public: 
  
  typedef boost::shared_ptr<SinkPad> pSinkPad_t; 
  
  static pSinkPad_t createPad(std::string name) {
    return pSinkPad_t(new SinkPad(name)); 
  }
  
  std::string getName() { return name_;} 
  
  datawindow_t<BufferT> get_src_data(const timewindow_t & tw) {
    return src_->get_data(tw); 
  }

  void set_src(SourcePad<BufferT> * sp) {
    src_ = sp; 
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
  SourcePad<BufferT> * src_; 

}; 

}

#endif // SINKPAD_H
