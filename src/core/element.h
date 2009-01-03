#ifndef ELEMENT_H
#define ELEMENT_H

#include <map>
#include <list>
#include "ielement.h"
#include "sourcepad.h"
#include "sinkpad.h"

namespace core { 

  class Element : public virtual IElement {
  public:
    Element(std::string name) :
      name_(name)
    {}; 
    
    template<class BufferT> SourcePad<BufferT> *
    createSourcePad(typename QueueView<BufferT>::dataContainer_t & buffer,
		    std::string name)
    {
      SourcePad<BufferT> * ppad = SourcePad<BufferT>::createPad(buffer, name); 
      pSourcePads_.insert(std::make_pair(name, ppad)); 
      return ppad; 
    }
    
    template<class BufferT> SinkPad<BufferT> * 
    createSinkPad(std::string name) {
      SinkPad<BufferT> * ppad = SinkPad<BufferT>::createPad( name); 
      pSinkPads_.insert(std::make_pair(name, ppad)); 
      return ppad; 

    }
    
    // access
    template<class BufferT>
    SourcePad<BufferT> * getSource(std::string name) {
      sourcePadMap_t::iterator pos = pSourcePads_.find(name);
      if (pos == pSourcePads_.end()) {
	throw std::runtime_error("Unknown source pad"); 
      }

      return dynamic_cast<SourcePad<BufferT> *>(pos->second); 

    }
    
    template<class BufferT>
    SinkPad<BufferT> * getSink(std::string name) {
      sinkPadMap_t::iterator pos = pSinkPads_.find(name);
      if (pos == pSinkPads_.end()) {
	throw std::runtime_error("Unknown sink pad"); 
      }

      return dynamic_cast<SinkPad<BufferT> *>(pos->second); 
      
    }

    std::list<std::string> getAvailableSources()
    {
      std::list<std::string> result; 
      for (sourcePadMap_t::iterator ps = pSourcePads_.begin(); ps != pSourcePads_.end(); ++ps) {
	result.push_back((*ps).second->getName()); 
      }
      return result; 
    }

    std::list<std::string> getAvailableSinks()
    {
      std::list<std::string> result; 
      for (sinkPadMap_t::iterator ps = pSinkPads_.begin(); ps != pSinkPads_.end(); ++ps) {
	result.push_back((*ps).second->getName()); 
      }
      return result; 
    }

    std::string getName() { return name_; } 
    void setName(std::string name) { name_ = name; }


    pISourcePad_t getSourcePad(std::string name)
    {
      sourcePadMap_t::iterator pos = pSourcePads_.find(name);
      if (pos == pSourcePads_.end()) {
	throw std::runtime_error("Unknown source pad"); 
      }

      return pos->second; 
    }

    pISinkPad_t getSinkPad(std::string name)
    {
      sinkPadMap_t::iterator pos = pSinkPads_.find(name);
      if (pos == pSinkPads_.end()) {
	throw std::runtime_error("Unknown sink pad"); 
      }
      return pos->second; 
    }
    
  private:
    typedef std::map<std::string, ISourcePad *>  sourcePadMap_t; 
    sourcePadMap_t pSourcePads_; 
    
    typedef std::map<std::string, ISinkPad *> sinkPadMap_t; 
    sinkPadMap_t pSinkPads_; 

    std::string name_; 
    
  }; 
  
  
}

#endif // ELEMENT_H
