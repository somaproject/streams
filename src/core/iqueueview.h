#ifndef __IQUEUEVIEW_H__
#define __IQUEUEVIEW_H__


#include <list>
#include <stdexcept>
#include <iostream>

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_sequence_adapter.hpp>
#include <boost/shared_ptr.hpp>

namespace core {

  template<typename T>
  class IQueueView { 
  public:
    
    //virtual IQueueView<T> & operator=(const IQueueView<T>&) = 0; 
    virtual bool empty() = 0; 
    virtual T & front() = 0; 
    virtual void pop() = 0; 
    
    virtual void reset() = 0; 

    typedef boost::shared_ptr<IQueueView <T> > ptr; 
  }; 


}

#endif // 
