#ifndef FILTERBASE_H
#define FILTERBASE_H


#include <boost/shared_ptr.hpp>
#include <list>

#include <vector>
#include <sigc++/sigc++.h>
#include <core/element.h>
#include "ifilter.h"

class FilterBase : public core::Element, public IFilter
{
public:
  // name
  static const std::string TYPENAME; 
  FilterBase(std::string name) : Element(name) 
  {

  }

  
};


#endif // FILTER!BASE_H

