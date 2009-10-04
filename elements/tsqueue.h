#ifndef __ELEMENTS_TSQUEUE_H__
#define __ELEMENTS_TSQUEUE_H__


namespace elements { 
  
  template<typename T>
  class NaiveQueue
  {
  /*
    AT THE MOMENT A VERY NAIVE IMPLEMENTATION
    
  */ 
  public:
    void send(const T & x  ) {
      boost::unique_lock<boost::mutex> lock(mutex_); 
      queue_.push_back(x); 
    }

    bool empty() {
      boost::unique_lock<boost::mutex> lock(mutex_); 
      return queue_.empty() ;
    }

    T get() {
      boost::unique_lock<boost::mutex> lock(mutex_); 
      
      if (queue_.empty()){
	throw std::runtime_error("Trying to get from empty queue!"); 
      }
      
      T m = queue_.front(); 
      queue_.pop_front(); 
      return m; 
    }

  private:
    boost::mutex mutex_; 
    std::list<T> queue_; 


  }; 

}
#endif
