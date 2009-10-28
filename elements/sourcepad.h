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

    static pSourcePad_t  createPad(padid_t id, IElementSource<BufferT> * owner, 
				   std::string name) 
    {
      return pSourcePad_t(new SourcePad(id, owner, name)); 
    }
    

    void connect(pSinkPad_t sp) {
      pSinks_.push_back(sp); // these are the ones that get the dirty message
      sp->set_src(this);       
    }

    void connect(pISinkPad_t tgt) {
      typedef  SinkPad<BufferT> sp_t; 
      connect(boost::dynamic_pointer_cast<sp_t>(tgt)); 
      
    }

    void disconnect(pSinkPad_t sp) { 
      pSinks_.erase(sp); 
      sp.set_src(NULL); 
    }
    
    std::string getName() { return name_;} 

    /* Ugh, this returns the result in data so we can type-specialize
     */ 
    
    void get_data(std::list<BufferT> & data,  const timewindow_t & tw) {
      return owner_->get_src_data(data, id_, tw); 
    }
    
    void timecache_insert(timeid_t tid, timecache_t::datahash_t h) {
      // FIXME: mutex-protect
      src_timecache_.insert(tid, h); 
    }

    void timecache_reset() {
      src_timecache_.clear(); 
    }

    timecache_t::levelhash_t timecache_get_hash(size_t level, bucketid_t bid)
    {
      src_timecache_.get_hash(level, bid); 
    }
    
  protected:
    SourcePad(padid_t id, IElementSource<BufferT> * owner, std::string name) :
      id_(id), 
      owner_(owner),
      name_(name), 
      src_timecache_(100000000, 4, 8)
    {
      

    }

    padid_t id_; 
    IElementSource<BufferT> * owner_;
    std::string name_; 
    
    std::list<pSinkPad_t > pSinks_; 

    timecache_t src_timecache_; 
    
  }; 


}

#endif // SOURCEPAD_H
