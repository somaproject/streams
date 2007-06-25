#ifndef STREAMSOURCE_H
#define STREAMSOURCE_H

#include <boost/shared_ptr.hpp>
#include <list>

#include "streams.h"
#include "streamvis.h"


class StreamSource
{

 public:
  virtual streamVisPtr_t newVisFactory(std::string name) = 0; 
  
    // delete signal
  sigc::signal<void> & disconnectSignal() { return disconnectSignal_; };

  void disconnect() {
    disconnectSignal_.emit(); 
  }

 private:
  sigc::signal<void> disconnectSignal_; 

};

#endif // STREAMSOURCE_H

