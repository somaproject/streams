#ifndef QUEUEVIEW_H
#define QUEUEVIEW_H

#include <list>
#include <stdexcept>
#include <iostream>

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_sequence_adapter.hpp>
#include "iqueueview.h"

namespace core {
    template <typename T>
    class QueueView : public IQueueView<T> {
    public:
      typedef typename boost::ptr_list<T> dataContainer_t; 

      QueueView(dataContainer_t & lref); 
      QueueView(); 
  
      ~QueueView(); 
      QueueView<T> & operator=(const QueueView<T>&); 
      bool empty(); 
      T & front(); 
      void pop(); 
  
      void reset(); 
      dataContainer_t * lref_; 
  
    protected: 

      bool listWasEmpty_; 
      typename dataContainer_t::iterator curItem_; 
      bool passedEnd_; 
  
    }; 

    template <typename T>
    QueueView<T>::QueueView(dataContainer_t & lref) :
      lref_(&lref), 
      listWasEmpty_(false), 
      passedEnd_(false)
    {
      if (lref_->empty()) {
	listWasEmpty_ = true; 
      } else {
	curItem_ = lref_->begin(); 
	listWasEmpty_ = false; 
      }
    
    }

    template <typename T>
    QueueView<T>::QueueView() :
      lref_(NULL), 
      listWasEmpty_(false), 
      passedEnd_(false)
    {
      
    }

    template <typename T>
    QueueView<T>::~QueueView()
    {


    }

    template <typename T>
    bool QueueView<T>::empty()
    {


      if (lref_->empty() )
	{
	  listWasEmpty_ = true; 
	  return true; 
	}
      else {
	if (listWasEmpty_) {
	  // the source used to be empty and now it's not; inc the pointer!
	  curItem_ = lref_->begin(); 
	  listWasEmpty_ = false; 
	  passedEnd_ = false; 
	  return false; 

	} else {
	  // this is the normal case; check to make sure that we're 
	  // not past the end
	  if (passedEnd_) {
	    if (curItem_ != --lref_->end()) {
	      // if we're no longer at the end:
	      passedEnd_ = false; 
	      curItem_++; 
	      return false; 
	    }
	    return true; 
	  }
	  return false; 
	}
      }


    }

    template <typename T>
    void QueueView<T>::pop()
    {
      if (empty() ) {
	// throw an error
	throw std::out_of_range("View is empty"); 

      } else {
	if (curItem_ == --(lref_->end()) )
	  {
	    passedEnd_ = true; 
	  } else {
	    passedEnd_ = false; 
	    curItem_++; 
	  }

      }

  
    }

    template <typename T>
    T & QueueView<T>::front(){
      return *curItem_; 

    }

    template <typename T>
    void QueueView<T>::reset(){
      listWasEmpty_ = true; 
      passedEnd_ = false; 
      empty(); 

    }

    template <typename T>
    QueueView<T> &  QueueView<T>::operator=(const QueueView<T>& qv)
    {
      lref_ = qv.lref_; 
      listWasEmpty_ = qv.listWasEmpty_; 
      curItem_ = qv.curItem_; 
      passedEnd_ = qv.passedEnd_; 

    }
}

#endif // QUEUEVIEW_H


