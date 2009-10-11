#ifndef __LRU_CACHE_H__
#define __LRU_CACHE_H__

/* 

LRU cache, inspired by Patrick Audley's LRU cache. 

Store primary values in a std::list of key, value pairs and 
use a map for constant-time lookup

The general API here is that we support a size() method
to query how large the cache is, and we can pop_old() to remove
old elements. 

*/ 

#include <map>
#include <list>

template <typename Key, typename Value>
class LRUCache { 

public:
  typedef std::pair<Key, Value> list_datum_t; 
  typedef std::list<list_datum_t> pair_list_t; 
  typedef typename pair_list_t::iterator pair_list_iter_t; 
  
  typedef std::map<Key, pair_list_iter_t> map_t; 
  typedef typename map_t::iterator map_iter_t; 
  

private:
  pair_list_t seqlist_; 
  map_t keymap_; 

  size_t size_; // constant-time access size. 
  
public:
  LRUCache() : 
    size_(0)
  {


  }
  
  const size_t size() { 

    return size_ ;
  }

  void clear() { 
    seqlist_.clear(); 
    keymap_.clear(); 
    size_ = 0; 
  }
  
  void insert(const Key & key, const Value & data) { 
    
    map_iter_t mi = keymap_.find(key); 
    if (mi != keymap_.end()) { 
      // value is alread in cache, remove
      seqlist_.erase(mi->second); 
    }
    
    seqlist_.push_front(std::make_pair(key, data)); 
    pair_list_iter_t li = seqlist_.begin(); 
    keymap_.insert(std::make_pair(key, li)); 
    
    size_++; 
  }

  void touch( const Key &key ) {
    map_iter_t mi = keymap_.find(key); 
    seqlist_.splice(seqlist_.begin(), seqlist_, 
		    mi->second); 
  }

  bool exists(const Key & key) {

    return keymap_.find(key) != keymap_.end(); 
  }
  
  Value get(const Key & key, bool do_touch=true) {
    map_iter_t mi = keymap_.find(key); 
    if (mi == keymap_.end()) {
      return Value(); 
    }

    Value d = mi->second->second; 
    if (do_touch ) { 
      touch(key); 
    }
    return d; 

  }

  Value pop_old() { 
    if (size_ > 0) { 
      
      pair_list_iter_t i = seqlist_.end(); 
      i--; 
      // should be a valid element
      Value d = i->second; 
      
      keymap_.erase(i->first); // remove from map
      seqlist_.erase(i); 
      size_--; 
      return d;

    } else { 
      return Value(); 

    }
  }

  

}; 

#endif 
