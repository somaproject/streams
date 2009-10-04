#include "commands.h"

namespace elements
{

  void  commandqueue::send(MESSAGES m) {
    boost::scoped_lock<boost::mutex> lock(mutex_); 
    queue_.push_back(m); 
    
  }

  bool commandqueue::empty()
  {
    boost::scoped_lock<boost::mutex> lock(mutex_); 
    return queue_.empty(); 

  }

  MESSAGE commandqueue::get()
  {
    boost::scoped_lock<boost::mutex> lock(mutex_); 
    if (queue_.empty()){
      throw std::runtime_error("Trying to get from empty queue!"); 
    }

    MESSAGE m = queue_.front(); 
    queue_.pop(); 
    return m; 
  }



}
