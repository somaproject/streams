#ifndef NETDATAWAVE_H
#define NETDATAWAVE_H

#include <vector>
#include <sigc++/sigc++.h>
#include <somanetwork/wave.h>
#include <boost/shared_ptr.hpp>

#include "queueview.h"
#include "streamsource.h"
#include "waves.h" 

class NetDataWave : public StreamSource<WaveBuffer_t>
{
 public:
  // this is just a prototype source
  NetDataWave(pSomaNetCodec_t);

  ~NetDataWave(); 
  
  // emit updatedData
  
  
 private:
  datasource_t datasource_; 
  datatype_t datatype_; 
  pSomaNetCodec_t pSomaNetCodec_; 
  void newDataPacket(pDataPacket_t dp); 
};

#endif // NETDATAWAVE_H
