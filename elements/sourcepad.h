#ifndef __ELEMENTS_SOURCEPAD_H__
#define __ELEMENTS_SOURCEPAD_H__

#include <boost/shared_ptr.hpp>
#include "isourcepad.h"
#include "sinkpad.h"


namespace elements {

  template<class BufferT>
  class SourcePad : public ISourcePad 
  {
  private:
    typedef typename SinkPad<BufferT>::pSinkPad_t pSinkPad_t; 
    typedef LinkElement<BufferT> elt_t; 
    typedef boost::shared_ptr<elt_t> pelt_t; 

  public:
    typedef boost::shared_ptr<SourcePad> pSourcePad_t; 

    static pSourcePad_t  createPad(std::string name) 
    {
      return pSourcePad_t(new SourcePad(name)); 

    }
	      

    void connect(pSinkPad_t sp) {
      pSinks_.push_back(sp); 
    }

    void connect(pISinkPad_t tgt) {
      typedef  SinkPad<BufferT> sp_t; 
      boost::dynamic_pointer_cast<sp_t>(tgt); 
      
    }


    std::string getName() { return name_;} 

    void newData(timeid_t t, BufferT B) {
      pelt_t elt(new elt_t); 
      elt->datum = B; 
      elt->time = t; 
      elt->state = elt_t::DATA; 

      BOOST_FOREACH(pSinkPad_t sink, pSinks_) {
	
	sink->senddata(elt); 
      }
    }

    void reset() {
      pelt_t elt(new elt_t); 
      elt->time = 0; 
      elt->state = elt_t::RESET; 
      BOOST_FOREACH(pSinkPad_t sink, pSinks_) {
	sink->sendmsg(RESET); 
	sink->send(elt); 
      }
    }

    void newSequence(timeid_t t, BufferT B) {
      pelt_t elt(new elt_t); 
      elt->datum = B; 
      elt->time = t; 
      elt->state = elt_t::SEQUENCEHEAD; 

      BOOST_FOREACH(pSinkPad_t sink, pSinks_) {
	sink->senddata(elt); 
      }
    }
    
  protected:
    SourcePad(std::string name) :
      name_(name) 
    {


    }

    std::string name_; 

    
    std::list<pSinkPad_t > pSinks_; 

    
  }; 


}

#endif // SOURCEPAD_H
