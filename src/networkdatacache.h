#ifndef NETWORKDATACACHE
#define NETWORKDATACACHE

#include <elements/types.h>
#include <somanetwork/network.h>
#include <somanetwork/wave.h>
#include <somanetwork/raw.h>
#include <data/wave.h>
#include <boost/thread/mutex.hpp>

#include "isomanetcodec.h"
#include "timer.h"

#include <boost/filesystem.hpp>


namespace bf = boost::filesystem; 

/*
  The network data cache takes in (and caches) data packets onto
  disk, allowing any subsequently-connected pipeline element
  access to the entire history of the data. 

  Right now for each data source we use a berkeley-DB-backed 
  on-disk store, where eventually the user will be able to control
  the in-memory cache size. We use a BDB btree via the TDS. 


 */ 
class NetworkDataCache
{
public:
  NetworkDataCache(pISomaNetCodec_t, pTimer_t, bf::path scratcdir);
  ~NetworkDataCache(); 

  void preload(pDataPacket_t dp); 

  elements::datawindow_t<pWaveBuffer_t> getNetWaveData(datasource_t n, elements::timewindow_t);
  elements::datawindow_t<pWaveBuffer_t> getNetRawData(datasource_t n, elements::timewindow_t);
    

  void appendNewData(pDataPacket_t dp); 

  std::vector<sigc::signal<void> > waveSignals_; 
  std::vector<sigc::signal<void> > rawSignals_; 

private: 
  static const int MAXWAVE = 64; 
  boost::mutex mutex_; 

  pISomaNetCodec_t pNetCodec_; 
  pTimer_t pTimer_; 
  bf::path scratchdir_;

  bool dataRXCallback(Glib::IOCondition io_condition); 

  typedef std::map<timeid_t, pWaveBuffer_t> datamap_t; 
  std::vector<datamap_t> rawData_; 
  std::vector<bool> rawDataEnabled_; 
  std::vector<datamap_t> waveData_; 
  std::vector<bool> waveDataEnabled_; 

}; 

typedef boost::shared_ptr<NetworkDataCache> pNetworkDataCache_t; 


#endif // NETWORKDATACACHE
