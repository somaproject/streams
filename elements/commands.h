#ifndef __ELEMENTS_COMMANDS_H__
#define __ELEMENTS_COMMANDS_H__

#include <boost/thread/mutex.hpp>
#include <list>

#include "tsqueue.h"

namespace elements
{
  enum MESSAGES { 
    RESET,
    NEWSEQUENCE,
    CONNECT, 
    DISCONNECT}; 

  typedef NaiveQueue<MESSAGES> commandqueue_t; 
  
}


#endif 
