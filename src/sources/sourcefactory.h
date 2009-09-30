#ifndef SOURCEFACTORY_H
#define SOURCEFACTORY_H

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem; 

#include "jsonutil.h"
#include "sourcebase.h"
#include "sourcestate.h"
#include "isourcevisitor.h"
/*
  Primary factory function for creating new source elements

*/ 

class SourceFactory {
public : 
  static pISource_t create(std::string name, pSourceState_t ss, 
			   bf::path scratchdir, 
			   const json_util::mObject & config); 
  
}; 


#endif //  SOURCEFACTORY_H
