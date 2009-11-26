#include "networkdatacache.h"
#include <boost/format.hpp>

NetworkDataCache::NetworkDataCache(pISomaNetCodec_t pnc, 
				   pTimer_t ptime, bf::path scratchdir): 
  pNetCodec_(pnc), 
  pTimer_(ptime),
  scratchdir_(scratchdir / "netdatawave"),
  rawData_(MAXWAVE),
  waveData_(MAXWAVE),
  rawDataEnabled_(MAXWAVE),
  waveDataEnabled_(MAXWAVE)
{

  boost::mutex::scoped_lock  lock(mutex_);

  pNetCodec_->newDataSignal().connect(sigc::mem_fun(*this, 
						 &NetworkDataCache::appendNewData)); 
  
  // create the db for each data type. Note that we don't put data
  // in there until it has been expressly enabled. 
  // 
  
  boost::filesystem::remove_all(scratchdir_ );
  boost::filesystem::create_directories(scratchdir_); 

  for(int i = 0; i < MAXWAVE; i++)
    {
      rawDataEnabled_[i] = false; 
      waveDataEnabled_[i] = false; 
    }
  
}


NetworkDataCache::~NetworkDataCache()
{
  //boost::filesystem::remove_all(scratchdir_ );

}

elements::datawindow_t<pWaveBuffer_t> NetworkDataCache::getNetWaveData(datasource_t n, elements::timewindow_t tw)
{
  //  std::cout << "NetworkDataCache::getNetWaveSource(datasource_t n =" << n << std::endl;
  boost::mutex::scoped_lock  lock(mutex_);
  
  elements::datawindow_t<pWaveBuffer_t> wb; 
  
  if(waveDataEnabled_[n]) { 
    if (!waveData_[n].empty()) {
      
      timeid_t start = tw.start; 
      timeid_t end = tw.end; 

      datamap_t::iterator i = waveData_[n].lower_bound(start);       
      if (i != waveData_[n].begin()) {
	i--; 
      }
      for (i; ((i != waveData_[n].end() ) and (i->first <= end)); ++i) {
	if (i->first >= start) {
	  wb.data.push_back(i->second); 
	}
      }
      if (!wb.data.empty()) {
	wb.interval = elements::timeinterval_t(wb.data.front()->time, 
					       wb.data.back()->time); 
      }
      
    // do the query, return the data
    }
  } else { 
    
    pNetCodec_->enableDataRX(n, WAVE); 
    // return null data 
    waveDataEnabled_[n] = true; 
  } 
//   std::cout << "NetworkDataCache returning "
// 	    << wb.interval.lower() << " " << wb.interval.upper() << std::endl; 
  return wb;
    
}

elements::datawindow_t<pWaveBuffer_t> 
NetworkDataCache::getNetRawData(datasource_t n, elements::timewindow_t tw)
{

  // std::cout << "NetworkDataCache::getNetRawSource(datasource_t n =" << (int)n << std::endl;
  boost::mutex::scoped_lock  lock(mutex_);

  elements::datawindow_t<pWaveBuffer_t> wb; 
  
  if(rawDataEnabled_[n]) { 
    timeid_t start = tw.start; 
    timeid_t end = tw.end; 

    datamap_t::iterator i = rawData_[n].lower_bound(start); 
    if (i != rawData_[n].begin()) {
      i--; 
    }
    for (i; ((i != rawData_[n].end() ) and (i->first <= end)); ++i) {
      if (i->first >= start) {
	wb.data.push_back(i->second); 
	}
    }
    
    if (!wb.data.empty()) {
      wb.interval = elements::timeinterval_t(wb.data.front()->time, 
					     wb.data.back()->time); 
    }
    
      
    
  } else { 
    pNetCodec_->enableDataRX(n, RAW); 
    rawDataEnabled_[n] = true; 
    // return null data 
  } 
  
}

void NetworkDataCache::appendNewData(pDataPacket_t newData)
{
  boost::mutex::scoped_lock  lock(mutex_);

  if (newData->typ == WAVE) {
    int src = newData->src; 
    Wave_t  wp = rawToWave(newData); 
    pWaveBuffer_t wb(new WaveBuffer_t); 
    wb->samprate = wp.sampratenum / float(wp.samprateden); 
    wb->time = pTimer_->somaTimeToTimeID(wp.time); 
    
    
    for(int i = 0; i < WAVEBUF_LEN; i++) {
      float val = float(wp.wave[i]) / 1e9; 
      float tsdelta =  1.0 / wb->samprate; 
      wb->data.push_back(val); 
    }
    waveData_[src].insert(std::make_pair(wb->time, wb)); 

  } else if (newData->typ == RAW) {
    int src = newData->src; 
    Raw_t  wp = rawToRaw(newData); 
    pWaveBuffer_t wb(new WaveBuffer_t); 
    wb->samprate = 32000.0; 
    wb->time = pTimer_->somaTimeToStreamTime(wp.time); 
    
    for(int i = 0; i < RAWBUF_LEN; i++) {
      wb->data.push_back(float(wp.data[i])/1e9); 
    }
    waveData_[src].insert(std::make_pair(wb->time, wb)); 
  } 
  else {
    // FIXME: Log unknown data packet 
    
  }

}

