#ifndef MOCKSOMACODEC_H
#define MOCKSOMACODEC_H

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <somanetwork/fakenetwork.h>
#include <boost/enable_shared_from_this.hpp>
#include <sigc++/sigc++.h>
#include <somadspio/dspcontrol.h>

#include <gtkmm.h>
#include "streamtime.h"
#include "isomanetcodec.h"

class SomaNetCodec; 
class DataWaveCodec;

using namespace somanetwork; 
class MockSomaNetCodec :  public ISomaNetCodec, 
		      public boost::enable_shared_from_this<MockSomaNetCodec>
{
public:
  MockSomaNetCodec(); 
  
  sigc::signal<void, pDataPacket_t> & newDataSignal()
  {
    return newDataSignal_; 
  }

  sigc::signal<void, somatime_t> & signalTimeUpdate()
  {
    return signalTimeUpdate_; 

  }

  void enableDataRX(datasource_t src, datatype_t typ); 
  
  dspiolib::StateProxy & getDSPStateProxy(datasource_t src); 

  void newData(pDataPacket_t pdp); 
  void newTime(somatime_t); 


private:
  sigc::signal<void, pDataPacket_t> newDataSignal_; 
  sigc::signal<void, somatime_t> signalTimeUpdate_;

  boost::ptr_vector<dspiolib::StateProxy> dspStateProxies_; 

}; 

typedef boost::shared_ptr<MockSomaNetCodec> pMockSomaNetCodec_t; 

#endif // MOCKSOMACODEC_H
