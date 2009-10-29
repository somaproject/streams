#ifndef __ELEMENTS_ELEMENT_H__
#define __ELEMENTS_ELEMENT_H__

#include <map>
#include <list>
#include <elements/ielement.h>
#include <elements/sourcepad.h>
#include <elements/sinkpad.h>

namespace elements { 

  class Element : public virtual IElement {
  public:
    Element(std::string name) :
      name_(name)
    {}; 
    
    template<class BufferT> 
    typename SourcePad<BufferT>::pSourcePad_t 
    createSourcePad(std::string name, 
		    typename SourcePad<BufferT>::get_data_func_t df, 
		    typename SourcePad<BufferT>::get_reset_func_t rf)  {
      
      typename SourcePad<BufferT>::pSourcePad_t pad = 
	SourcePad<BufferT>::createPad(name, df, rf); 
      pSourcePads_.insert(std::make_pair(name, pad)); 
      return pad; 
    }
      
    template<class BufferT> 
    typename SinkPad<BufferT>::pSinkPad_t
    createSinkPad(std::string name) {
      typename SinkPad<BufferT>::pSinkPad_t pad = 
	SinkPad<BufferT>::createPad(name); 

      
      pSinkPads_.insert(std::make_pair(name, pad)); 
      return pad; 
    }
    
//     // access
//     template<class BufferT>
//     SourcePad<BufferT> * getSource(std::string name) {
//       sourcePadMap_t::iterator pos = pSourcePads_.find(name);
//       if (pos == pSourcePads_.end()) {
// 	throw std::runtime_error("Unknown source pad"); 
//       }

//       return dynamic_cast<SourcePad<BufferT> *>(pos->second); 

//     }
    
//     template<class BufferT>
//     SinkPad<BufferT> * getSink(std::string name) {
//       sinkPadMap_t::iterator pos = pSinkPads_.find(name);
//       if (pos == pSinkPads_.end()) {
// 	throw std::runtime_error("Unknown sink pad"); 
//       }

//       return dynamic_cast<SinkPad<BufferT> *>(pos->second); 
      
//     }

//     std::list<std::string> getAvailableSources()
//     {
//       std::list<std::string> result; 
//       for (sourcePadMap_t::iterator ps = pSourcePads_.begin(); ps != pSourcePads_.end(); ++ps) {
// 	result.push_back((*ps).second->getName()); 
//       }
//       return result; 
//     }

//     std::list<std::string> getAvailableSinks()
//     {
//       std::list<std::string> result; 
//       for (sinkPadMap_t::iterator ps = pSinkPads_.begin(); ps != pSinkPads_.end(); ++ps) {
// 	result.push_back((*ps).second->getName()); 
//       }
//       return result; 
//     }

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
    typedef std::map<std::string, pISourcePad_t>  sourcePadMap_t; 
    sourcePadMap_t pSourcePads_; 
    
    typedef std::map<std::string, pISinkPad_t> sinkPadMap_t; 
    sinkPadMap_t pSinkPads_; 

    std::string name_; 
    
  }; 
  
  
}

#endif // ELEMENT_H
