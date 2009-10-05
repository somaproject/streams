#ifndef __ELEMENTS_PROPERTIES_H__
#define __ELEMENTS_PROPERTIES_H__


namespace elements { 

  class IPropertyNotify
  {
  public:
    virtual void notify() = 0; 
    
  }; 
  
  typedef boost::shared_ptr<IPropertyNotify> pIPropertyNotify_t; 
  


template <typename ValueType>
class Property
{
protected:
  typedef boost::shared_lock<boost::shared_mutex> shared_lock_t; 
  typedef boost::upgrade_lock<boost::shared_mutex> uprade_lock_t; 
  typedef boost::upgrade_to_unique_lock<boost::shared_mutex> up_unique_lock_t; 
  
  
public: 
  Property(ValueType v) :
  {
    shared_lock_t lock(value_mutex_); 
    value_ = v; 
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
    
    upgrade_lock_t lock(reqvalue_mutex_);
    // get exclusive access
    up_unique_lock_t uniqueLock(lock);
    reqValue_ = value; 

    shared_lock_t lock(mutex_); 
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

    // now possibly notify watchers
    BOOST_FOREACH(pIPropertyNotify_t n, watchers_)
      {
	n->notify(); 
      }
    
  }

  void addWatcher(pIPropertyNotify_t n) {
    upgrade_lock_t lock(watchers_mutex_);
    // get exclusive access
    up_unique_lock_t uniqueLock(lock);
    watchers_.insert(n); 
  }

  void removeWatcher(pIPropertyNotify_t n) {
    upgrade_lock_t lock(watchers_mutex_);
    // get exclusive access
    up_unique_lock_t uniqueLock(lock);
    watchers_.erase(n); 
  }

protected:

  
  boost::shared_mutex value_mutex_; 
  ValueType value_; 

  boost::shared_mutex reqvalue_mutex_; 
  ValueType reqValue_; 
  
  // right now this is just a pipe, or potentially a list of pipes
  std::set<pIPropertyNotify_t> watchers_; 
  boost::shared_mutex watchers_mutex_; 
  

}; 

}

#endif
