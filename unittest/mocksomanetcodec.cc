#include "mocksomanetcodec.h"

MockSomaNetCodec::MockSomaNetCodec()
{


}

void MockSomaNetCodec::enableDataRX(datasource_t src, datatype_t typ)
{
  

}
  
dspiolib::StateProxy & MockSomaNetCodec::getDSPStateProxy(datasource_t src)
{



}



void MockSomaNetCodec::newData(pDataPacket_t pdp)
{
  newDataSignal_.emit(pdp); 

}

void MockSomaNetCodec::newTime(somatime_t st)
{
  signalTimeUpdate_.emit(st); 

}
