#ifndef NETWORKDATACACHE
#define NETWORKDATACACHE

#include <bdbqueueview.h>
#include <somanetwork/network.h>
#include <somanetwork/wave.h>
#include <somanetwork/raw.h>
#include <data/wave.h>
#include "isomanetcodec.h"
#include "timer.h"

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem; 

/*
  The network data cache takes in (and caches) data packets onto
  disk, allowing any subsequently-connected pipeline element
  access to the entire history of the data. 

  Because our WaveBuffer abstraction uses a heap-allocated vector for
  data, the resulting struct cannot be simply read-out of BDB. There
  were two options, creating a fixed-length data structure for all
  wave-buffers (with a max data size), or doing some extra copying and
  _storing_ a different class of data structure.
  

 */ 
class NetworkDataCache
{
public:
  NetworkDataCache(pISomaNetCodec_t, pTimer_t, bf::path scratcdir);
  ~NetworkDataCache(); 

  void preload(pDataPacket_t dp); 

  core::IQueueView<WaveBuffer_t>::ptr getNetWaveSource(datasource_t n); 
  core::IQueueView<WaveBuffer_t>::ptr getNetRawSource(datasource_t n); 

  void appendNewData(pDataPacket_t dp); 

  std::vector<sigc::signal<void> > waveSignals_; 
  std::vector<sigc::signal<void> > rawSignals_; 

private: 
  static const int MAXWAVE = 64; 

  pISomaNetCodec_t pNetCodec_; 
  pTimer_t pTimer_; 
  bf::path scratchdir_;

  bool dataRXCallback(Glib::IOCondition io_condition); 

  typedef boost::ptr_map<datasource_t, Db> dbmap_t; 
  dbmap_t waveCacheDBs_; 
  dbmap_t rawCacheDBs_; 

}; 

typedef boost::shared_ptr<NetworkDataCache> pNetworkDataCache_t; 


#endif // NETWORKDATACACHE
