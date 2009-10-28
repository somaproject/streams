#ifndef __ELEMENTS_SINKPAD_H__
#define __ELEMENTS_SINKPAD_H__

#include <elements/isinkpad.h>
#include <elements/isourcepad.h>
#include <elements/timecache.h>
#include <elements/cachepolicy.h>

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
  
  std::list<BufferT> get_src_data(const timewindow_t & tw) {
    return src_.get_data(tw); 
  }

  void set_src(SourcePad<BufferT> * sp) {
    src_ = sp; 
  }
  
  timecache_t::levelcache_t get_src_time_hash(size_t level, bucketid_t bid)
  {
    return src_->timecache_get_hash(level, bid); 
  }

  void timecache_insert(timeid_t tid, timecache_t::datahash_t h) {
    sink_timecache_.insert(tid, h); 
  }
  
  void timecache_reset() {
    sink_timecache_.clear(); 
  }
  
  timecache_t::levelhash_t timecache_get_hash(size_t level, bucketid_t bid)
  {
    sink_timecache_.get_hash(level, bid); 
  }
  

protected:
  
  SinkPad(std::string name) :
    name_(name), 
    connected_(false), 
    src_(0),
    sink_timecache_(100000000, 4, 8)
  {
    
  }
  
  std::string name_; 
  
  bool connected_; 
  
  public:
  SourcePad<BufferT> * src_; 
  NaiveQueue<timewindow_t> dirtyqueue_; 
  
  timecache_t sink_timecache_; 

}; 

}

#endif // SINKPAD_H
