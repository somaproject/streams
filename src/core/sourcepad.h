#ifndef SOURCEPAD_H
#define SOURCEPAD_H

#include <sigc++/sigc++.h>
#include "queueview.h"
#include "isourcepad.h"
#include "sinkpad.h"


namespace core {

  template<class BufferT>
  class SourcePad : public ISourcePad 
  {
  public:

    static SourcePad<BufferT> * 
    createPad(typename QueueView<BufferT>::dataContainer_t & buffer,
	      std::string name); 
	      
    
    QueueView<BufferT> * getQueueView(); 

    void connect(ISinkPad *); 
    void connect(SinkPad<BufferT> * ); 
    
    std::string getName() { return name_;} 

    // interfacing
    void newData(); 
    void invalidateData(); 
    
  protected:
    SourcePad( typename QueueView<BufferT>::dataContainer_t & buffer,
	       std::string name); 

    typename QueueView<BufferT>::dataContainer_t & buffer_; 
    std::list<SinkPad<BufferT> *> pSinks_; 
    std::string name_; 
    
  }; 

  template<class BufferT>
  SourcePad<BufferT>::SourcePad(typename QueueView<BufferT>::dataContainer_t & buffer, 
				std::string name) :
    buffer_(buffer), 
    name_(name)
  {


  }

  template<class BufferT>
  SourcePad<BufferT>* SourcePad<BufferT>::createPad(typename QueueView<BufferT>::dataContainer_t & buffer, 
						    std::string name) {

    return new SourcePad(buffer, name); 

  }

  template<class BufferT>
  void SourcePad<BufferT>::connect(SinkPad<BufferT> * tgt) {

    tgt->setSource(this); 
    pSinks_.push_back(tgt); 
    
    // connect to the disconnect signal FIXME
    
  }
  template<class BufferT>
  void SourcePad<BufferT>::connect(ISinkPad * tgt) {
    connect(dynamic_cast<SinkPad<BufferT>* >(tgt)); 

  }

  template<class BufferT>
  void SourcePad<BufferT>::newData() {
    typename std::list<core::SinkPad<BufferT>*,
      std::allocator<core::SinkPad<BufferT>*> >::iterator i; 
    for(i = pSinks_.begin(); i != pSinks_.end(); ++i) {
      (*i)->newData(); 
    }

  }

  template<class BufferT>
  void SourcePad<BufferT>::invalidateData() {
    typename std::list<core::SinkPad<BufferT>*,
      std::allocator<core::SinkPad<BufferT>*> >::iterator i; 

    for(i = pSinks_.begin(); i != pSinks_.end(); ++i) {
      (*i)->invalidateData(); 
    }


  }

  template<class BufferT>
  QueueView<BufferT> * SourcePad<BufferT>::getQueueView() {
    return new QueueView<BufferT>(buffer_);     
  }

}

#endif // SOURCEPAD_H
