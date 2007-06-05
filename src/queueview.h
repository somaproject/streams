#ifndef QUEUEVIEW_H
#define QUEUEVIEW_H

#include <list>
#include <stdexcept>

template <typename T>
class QueueView {
public:
  QueueView(std::list<T> & lref); 
  ~QueueView(); 
  bool empty(); 
  T & front(); 
  void pop(); 
  
  void reset(); 
  
protected: 
  std::list<T> &  lref_; 
  bool listWasEmpty_; 
  typename std::list<T>::iterator curItem_; 
  bool pastEnd_; 
  
}; 

template <typename T>
QueueView<T>::QueueView(std::list<T> & lref) :
  lref_(lref), 
  listWasEmpty_(false), 
  pastEnd_(false)
  {
    if (lref_.empty()) {
      listWasEmpty_ = true; 
    } else {
      curItem_ = lref_.begin(); 
      listWasEmpty_ = false; 
    }
    
  }

template <typename T>
QueueView<T>::~QueueView()
{


}

template <typename T>
bool QueueView<T>::empty()
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
      pastEnd_ = false; 
      return false; 

    } else {
      // this is the normal case; check to make sure that we're 
      // not past the end
      if (pastEnd_) {
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
    if (curItem_ == --lref_.end() )
      {
	pastEnd_ = true; 
      } else {
	pastEnd_ = false; 
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
  curItem_ = lref_.begin(); 
  
}


#endif // QUEUEVIEW_H


