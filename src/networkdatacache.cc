#include "networkdatacache.h"


NetworkDataCache::NetworkDataCache(pSomaNetCodec_t pnc, 
				   pTimer_t ptime): 
  pNetCodec_(pnc), 
  pTimer_(ptime),
  waveCache_(MAXWAVE), 
  waveSignals_(MAXWAVE)
{
  pNetCodec_->newDataSignal().connect(sigc::mem_fun(*this, 
						 &NetworkDataCache::appendNewData)); 
  

}


NetworkDataCache::~NetworkDataCache()
{



}

core::QueueView<WaveBuffer_t> NetworkDataCache::getNetWaveSource(datasource_t n)
{
  
  if (waveCache_[n] == 0) {
    // need to enable, create list! 
    waveCache_[n] = new core::QueueView<WaveBuffer_t>::dataContainer_t(); 

    pNetCodec_->enableDataRX(n, WAVE); 
    
  } 

  return core::QueueView<WaveBuffer_t>(*waveCache_[n]); 

}

void NetworkDataCache::appendNewData(pDataPacket_t newData)
{
  if (newData->typ == WAVE) {
    if (waveCache_[newData->src] != 0) {
      int src = newData->src; 
      Wave_t  wp = rawToWave(newData); 
      WaveBuffer_t * wb = new WaveBuffer_t; 
      wb->samprate = wp.sampratenum / float(wp.samprateden); 
      wb->time = pTimer_->somaTimeToStreamTime(wp.time); 
      wb->data.reserve(WAVEBUF_LEN); 
      for(int i = 0; i < WAVEBUF_LEN; i++) {
	wb->data.push_back(wp.wave[i]); 
      }
      
      (*(waveCache_[src])).push_back(wb); 
      waveSignals_[src].emit(); 
    }

  } else {
    // FIXME: Log unknown data packet 
    
  }

}

