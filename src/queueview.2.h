#ifndef QUEUEVIEW_H
#define QUEUEVIEW_H

#include <list>

template <class T>
class QueueView {
public:
  QueueView(const std::list<T> & lref) :
  lref_(lref), 
  listWasEmpty_(false), 
  pastEnd_(false)
  {
    if (ref.empty()) {
      listWasEmpty_ = true; 
    } else {
      curItem_ = lref_.begin(); 
      listWasEmpty_ = false; 
    }
    
  }

  ~QueueView(); 
  bool empty(); 
  T & front(); 
  void pop(); 
  
  void reset(); 
  
protected: 
  const std::list<T> &  lref_; 
  bool listWasEmpty_; 
  std::list<T>::iterator curItem_; 
  bool pastEnd_; 
  
}; 

template <class T>
bool QueueView::empty()
{
  if (lref_.empty() )
    {
      listWasEmpty_ = true; 
      return true; 
    }
  else {
    if (listWasEmpty_) {
      // the source used to be empty and now it's not; inc the pointer!
      curItem_ = lref_.begin(); 
      listWasEmpty_ = false; 
    } else {
      // this is the normal case; check to make sure that we're 
      // not past the end
      if (pastEnd_) {
	return false; 
      }
    }
  }


}

template <class T>
void QueueView::pop()
{
  if (empty() ) {
    // throw an error
    throw std::logic_error("Attempt to pop from empty queue"); 
  } else {
    if (curItem_ + 1 == lref_.end() )
      {
	pastEnd_ = true; 
      } else {
	pastEnd_ = false; 
	curItem_++; 
      }

  }

  
}
#endif // QUEUEVIEW_H


