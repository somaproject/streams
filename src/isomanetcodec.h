#ifndef ISOMANETCODEC_H
#define ISOMANETCODEC_H

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <somanetwork/networkinterface.h>
#include <boost/enable_shared_from_this.hpp>
#include <sigc++/sigc++.h>
#include <somadspio/dspcontrol.h>

#include <gtkmm.h>
#include "streamtime.h"
class SomaNetCodec; 
class DataWaveCodec;

using namespace somanetwork; 
class ISomaNetCodec 
{
public:
  
  virtual sigc::signal<void, pDataPacket_t> & newDataSignal() = 0; 
  virtual sigc::signal<void, somatime_t> & signalTimeUpdate() = 0; 

  virtual void enableDataRX(datasource_t src, datatype_t typ) = 0; 
  
  virtual somadspio::StateProxy & getDSPStateProxy(datasource_t src) = 0; 
  
private:
}; 

typedef boost::shared_ptr<ISomaNetCodec> pISomaNetCodec_t; 

#endif // SOMANETCODEC_H
