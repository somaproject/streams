#ifndef __ELEMENTS_TIMECACHE_H__
#define __ELEMENTS_TIMECACHE_H__

#include <boost/foreach.hpp>
#include <list>
#include <vector>
#include <map>

#include <elements/types.h>

namespace elements
{

/* 
   The timecache is a mechanism by which a source element and a sink
   element can rapidly query their respective views of what data
   has been transmitted to see if they have, in fact, seen the same data. 

   For the time being, we assume that each datum starts at a single
   point in time, and has a unique start time -- that is, no two 
   data start at the sime timeid_t. We also assume that
   each datum type is capable of computing a hash over its value. This
   way, mutation/replacement can be tracked. 
   
   We assume that we have a 64-bit hash type, hash_t, for computing
   the hash of various values. 
   
*/ 



typedef int64_t bucketid_t ; 

class TimeCacheHash
{
public:
  // example time cache hash
  typedef uint64_t datahash_t; 
  typedef uint64_t levelhash_t; 
  

  class hashcombine_data {
  public:
    hashcombine_data() :
      dh_(NONE)
    {
      
    }
    
    void combine(timeid_t tid, datahash_t d) {
      // FIXME PASS

    }

    levelhash_t value() {
      return dh_; 
    }

    static const datahash_t NONE = 0; 
  private:
    levelhash_t dh_; 
  }; 
  
  class hashcombine_level {
  public:
    hashcombine_level() :
      lh_(NONE)
    {


    }

    void combine(timeid_t tid, datahash_t d) {
      // FIXME PASS

    }

    levelhash_t value() {
      return lh_; 
    }


    static const levelhash_t NONE = 0; 
  private:
    levelhash_t lh_;
  }; 
  
  

}; 

template<typename HashPolicy> 
class TimeCache
{
public:
  
  typedef typename HashPolicy::datahash_t datahash_t; 
  typedef typename HashPolicy::levelhash_t levelhash_t; 

  TimeCache(size_t granularity, size_t bucketcnt, size_t maxlevel) :
    granularity_(granularity),
    bucketcnt_(bucketcnt),
    maxlevel_(maxlevel)
  {
    granularities_.reserve(maxlevel_+1);
    size_t g = granularity; 
    for(int i = 0; i < (maxlevel_+1); i++) {
      granularities_.push_back(g); 
      g = g * bucketcnt; 
    }
    clear(); 
  }
  
  void clear()
  {
    data_.clear(); 
    hashtree_.clear(); 
    
    for (int i = 0; i < maxlevel_ +1; i++) {
      hashtree_.push_back(levelcache_t()); 
    }
    
  }
  
  timewindow_t get_bucket_window(size_t level, bucketid_t bucketid)
  {
    if (level > maxlevel_) {
    throw std::runtime_error("Level too high"); 
    }
  
    size_t g = granularities_[level]; 

    timewindow_t tw; 
    tw.start = bucketid * g; 
    tw.end = (bucketid+1) * g -1; 
    return tw; 
  }
  
  bucketid_t get_bucket(size_t level, timeid_t t)
  {
    if (level > maxlevel_) {
      throw std::runtime_error("Level too high"); 
    }
    bucketid_t g; 

    if (level == 0) {
      g = granularity_; 
    } else {
      g = granularity_ * level * bucketcnt_; 
    }

    if (t < 0) { 
      
      bucketid_t bi = -t / g; 
      if (-t % g ==0) {
	return -bi; 
      } else { 
	return -bi -1;
      } 

    } else {
      bucketid_t bi = t / g; 
      return bi; 
    }
    
  }
  
  std::list<bucketid_t> get_sub_buckets(size_t level, bucketid_t bid)
  {
    if (level > maxlevel_) {
      throw std::runtime_error("Level too high"); 
    }
    
    if (level == 0) {
      throw std::runtime_error("No sub bins of level zero"); 
    }
    
    timewindow_t win = get_bucket_window(level, bid); 
    
    bucketid_t ibid = get_bucket(level-1, win.start); 

    std::list<bucketid_t> results; 
    for(int i = 0; i < bucketcnt_; i++) {
      results.push_back(ibid + i); 
    }
    
    return results; 
    
  }
  
  levelhash_t get_hash(size_t level, bucketid_t bid) 
  {
    if (level > maxlevel_) {
      throw std::runtime_error("Level too high"); 
    }
    
    typename levelcache_t::iterator ci = hashtree_[level].find(bid); 
    
    if (ci == hashtree_[level].end()) {
      return 0; 
    } else {
      return ci->second; 
    }
    
  }
  
  std::list<timeid_t> get_data_times_in_window(timewindow_t ti)
  {
    std::list<timeid_t> times; 
    
    typename datacache_t::iterator di = data_.lower_bound(ti.start); 
    if(data_.empty()) {
      return times; 
    }
    
    if (di != data_.begin()) {
      di--; 
    }
    
    
    while( (di != data_.end()) and (di->first <= ti.end)) {
      if(di->first >= ti.start) {
	times.push_back(di->first); 
      }
      di++; 
    }
    return times; 
    
  }
  
  
  void insert(timeid_t t, datahash_t datumhash) 
  {
  
    data_.insert(std::make_pair(t, datumhash)); 
    
    // get all the data points in this window, and hash them
    timewindow_t tw_0 = get_bucket_window(0, get_bucket(0, t)); 
    
    typename datacache_t::iterator di = data_.lower_bound(tw_0.start); 
    if (di != data_.begin()) {
      di--; 
    }
    
    typename HashPolicy::hashcombine_data hc; 
    while((di != data_.end()) && (di->first <= tw_0.end)) {
      if(di->first >= tw_0.start) {
	hc.combine(di->first, di->second); 
    }
      di++; 
    }
    
    hashtree_[0].insert(std::make_pair(get_bucket(0, t), hc.value()));
    
    for(int level = 1; level < (maxlevel_+1); level++) {
      bucketid_t bid = get_bucket(level, t); 
      std::list<bucketid_t> subbuckets = get_sub_buckets(level, bid);
      
      typename HashPolicy::hashcombine_level level_hc; 
      BOOST_FOREACH(bucketid_t sub_bi, subbuckets) {
	typename levelcache_t::iterator hi = hashtree_[level-1].find(sub_bi); 
	if (hi == hashtree_[level-1].end()) { 
	  level_hc.combine(sub_bi, HashPolicy::hashcombine_level::NONE); 
	} else {
	  level_hc.combine(sub_bi, hi->second); 
	}
      }
      hashtree_[level].insert(std::make_pair(bid, level_hc.value())); 
      
    }
    
    
  }
  
private:
  size_t granularity_;
  size_t bucketcnt_; 
  size_t maxlevel_; 

  typedef std::map<timeid_t, datahash_t> datacache_t; 
  datacache_t data_; 
  typedef std::map<timeid_t, levelhash_t> levelcache_t; 

  typedef std::vector<levelcache_t> hashtree_t; 
  hashtree_t hashtree_; 

  std::vector<size_t> granularities_; 

}; 





}

#endif
