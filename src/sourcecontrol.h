#ifndef SOURCECONTROL_H
#define SOURCECONTROL_H

#include "timer.h"

class SourceControl
{
public:
  SourceControl(pTimer_t); 
  pStreamSourceBase_t createSource(std::string); 

private:
  pTimer_t pTimer_; 
  std::list<pStreamSourceBase_t> sourceList_; 

}; 

typedef boost::shared_ptr<SourceControl> pSourceControl_t; 


#endif // SOURCECONTROL_H
