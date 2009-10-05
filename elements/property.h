#ifndef __ELEMENTS_PROPERTIES_H__
#define __ELEMENTS_PROPERTIES_H__

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace elements { 

class IPropertyNotify
{
public:
  virtual void notify() = 0; 
  
}; 

typedef boost::shared_ptr<IPropertyNotify> pIPropertyNotify_t; 

class IProperty
{


}; 

template <typename ValueType>
class Property : public IProperty
{
protected:
  typedef boost::shared_lock<boost::shared_mutex> shared_lock_t; 
  typedef boost::upgrade_lock<boost::shared_mutex> upgrade_lock_t; 
  typedef boost::upgrade_to_unique_lock<boost::shared_mutex> up_unique_lock_t; 
  
  
public: 
  Property(ValueType v):
    watcherid_(0),
    value_(v), 
    reqValue_(v)
  {
  }
  
  
  
  virtual operator ValueType() 
  {
    shared_lock_t lock(value_mutex_); 
    return value_; 

  }
  
  ValueType operator =(const ValueType &  value) {
    /* 
       Value assignment occurs, potentially in another
       thread. 
       
     */ 
    
    upgrade_lock_t reqlock(reqvalue_mutex_);
    // get exclusive access
    up_unique_lock_t requniqueLock(reqlock);
    reqValue_ = value; 

    shared_lock_t vallock(value_mutex_); 
    return value_; 
  }


  bool pendingRequest() 
  {
    shared_lock_t value_lock(value_mutex_); 
    shared_lock_t reqvalue_lock(reqvalue_mutex_); 
    if (value_ != reqValue_) { 
      return true; 
    } 
    return false; 
  }

  ValueType get_req_value()
  {
    shared_lock_t reqvalue_lock(reqvalue_mutex_); 
    return reqValue_; 

  }

  void set_value(const ValueType & v) {
    upgrade_lock_t lock(value_mutex_);
    // get exclusive access
    up_unique_lock_t uniqueLock(lock);
    
    value_ = v; 

    shared_lock_t watcher_lock(watchers_mutex_);

    // now possibly notify watchers
    typedef std::map<size_t, pIPropertyNotify_t> watchermap_t; 
    for(watchermap_t::iterator i = watchers_.begin(); i != watchers_.end(); 
	i++) 
      {
	i->second->notify(); 
      }
    
  }

  size_t add_watcher(pIPropertyNotify_t n) {
    upgrade_lock_t lock(watchers_mutex_);
    // get exclusive access
    up_unique_lock_t uniqueLock(lock);
    watchers_.insert(std::make_pair(watcherid_, n)); 
    size_t wid = watcherid_; 
    watcherid_++; 
    return wid; 
    
  }

  void remove_watcher(size_t id) {
    upgrade_lock_t lock(watchers_mutex_);
    // get exclusive access
    up_unique_lock_t uniqueLock(lock);
    watchers_.erase(id); 
  }

protected:

  
  boost::shared_mutex value_mutex_; 
  ValueType value_; 

  boost::shared_mutex reqvalue_mutex_; 
  ValueType reqValue_; 
  
  // right now this is just a pipe, or potentially a list of pipes
  std::map<size_t, pIPropertyNotify_t> watchers_; 
  boost::shared_mutex watchers_mutex_; 
  size_t watcherid_; 
  

}; 

}

#endif
