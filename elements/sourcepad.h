#ifndef __ELEMENTS_SOURCEPAD_H__
#define __ELEMENTS_SOURCEPAD_H__

#include "isourcepad.h"
#include "sinkpad.h"


namespace elements {

  template<class BufferT>
  class SourcePad : public ISourcePad 
  {
  public:

    static pSourcePad_t<BufferT>  createPad(TimeSeriesDataBase<BufferT> & tdb, 
					    std::string name) 
    {
      return pSourcePad_t(new SourcePad(tdb, name)); 

    }
	      

    void connect(ISinkPad *); 

    void SourcePad<BufferT>::connect(ISinkPad * tgt) {
      connect(dynamic_cast<SinkPad<BufferT>* >(tgt)); 
      
    }

    void connect(pISinkPad_t * sp) {

    }

    std::string getName() { return name_;} 

    void newData() {
      BOOST_FOREACH(pSinkPad_t<BufferT> sink, pSinks_) {
	sink->send(NEWDATA); 
      }
    }

    void reset() {
      BOOST_FOREACH(pSinkPad_t<BufferT> sink, pSinks_) {
	sink->send(RESET); 
      }
    }

    void newSequence(timeid_t id) {
      BOOST_FOREACH(pSinkPad_t<BufferT> sink, pSinks_) {
	sink->send(NEWSEQUENCE); 
      }
    }
    
  protected:
    SourcePad(std::string name, TimeSeriesDataBase<BufferT> & tdb) :
      name_(name), 
      tsdb_(tdb)
    {


    }

    std::string name_; 

    TimeSeriesDataBase<BufferT> & tsdb; 
    
    std::list<pSinkPad_t<BufferT> > pSinks_; 

    
  }; 


}

#endif // SOURCEPAD_H
