#ifndef __ELEMENTS_SOURCEPAD_H__
#define __ELEMENTS_SOURCEPAD_H__

#include <boost/shared_ptr.hpp>
#include <elements/isourcepad.h>
#include <elements/sinkpad.h>
#include <elements/types.h>
#include <elements/cachepolicy.h>

namespace elements {


  template<class BufferT>
  class SourcePad : public ISourcePad 
  {
  private:
    typedef typename SinkPad<BufferT>::pSinkPad_t pSinkPad_t; 

  public:
    typedef boost::shared_ptr<SourcePad> pSourcePad_t; 
    typedef boost::function<datawindow_t<BufferT> (const timewindow_t &)> get_data_func_t; 
    typedef boost::function<size_t ()> get_reset_func_t; 

    static pSourcePad_t  createPad(std::string name, 
				   get_data_func_t gf, get_reset_func_t rf ) 
    {
      return pSourcePad_t(new SourcePad(name, gf, rf)); 
    }
    

    void connect(pSinkPad_t sp) {
      sp->set_src(this);       
    }

    void connect(pISinkPad_t tgt) {
      typedef  SinkPad<BufferT> sp_t; 
      connect(boost::dynamic_pointer_cast<sp_t>(tgt)); 
      
    }

    void disconnect(pSinkPad_t sp) { 
      sp.set_src(NULL); 
    }
    
    std::string getName() { return name_;} 

    /* Ugh, this returns the result in data so we can type-specialize
     */ 
    
    datawindow_t<BufferT> get_data(const timewindow_t & tw) {
      return getdatafunc_(tw); 
    }
    size_t get_series() {

      return getresetfunc_();
    }
    
  protected:
    SourcePad(std::string name, get_data_func_t gf, get_reset_func_t rf) :
      name_(name),
      getdatafunc_(gf),
      getresetfunc_(rf)
    {
      

    }

    padid_t id_; 
    std::string name_; 
    get_data_func_t getdatafunc_; 
    get_reset_func_t getresetfunc_; 
  }; 


}

#endif // SOURCEPAD_H
