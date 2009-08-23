#include "sourcebase.h"

const std::string SourceBase::TYPENAME("SourceBase"); 

SourceBase::SourceBase(std::string name)   : Element(name),
					     lastT_(0.0)
{
} 
  
SourceBase::~SourceBase()
{


}
