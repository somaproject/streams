#ifndef NETWORKDATACACHE
#define NETWORKDATACACHE

#include <core/queueview.h>
#include <somanetwork/network.h>
#include <somanetwork/wave.h>
#include <somanetwork/raw.h>
#include <data/wave.h>
#include "isomanetcodec.h"
#include "timer.h"

#include <boost/filesystem.hpp>
namespace bf = boost::filesystem; 

class NetworkDataCache
{
public:
  NetworkDataCache(pISomaNetCodec_t, pTimer_t, bf::path scratcdir);
  ~NetworkDataCache(); 

  void preload(pDataPacket_t dp); 

  core::QueueView<WaveBuffer_t> getNetWaveSource(datasource_t n); 
  core::QueueView<WaveBuffer_t> getNetRawSource(datasource_t n); 

  void appendNewData(pDataPacket_t dp); 

  std::vector<sigc::signal<void> > waveSignals_; 
  std::vector<sigc::signal<void> > rawSignals_; 

private: 
  static const int MAXWAVE = 64; 

  pISomaNetCodec_t pNetCodec_; 
  pTimer_t pTimer_; 
  bf::path scratchdir_;

  bool dataRXCallback(Glib::IOCondition io_condition); 
  
  std::vector<core::QueueView<WaveBuffer_t>::dataContainer_t * > waveCache_; 
  std::vector<core::QueueView<WaveBuffer_t>::dataContainer_t * > rawCache_; 

}; 

typedef boost::shared_ptr<NetworkDataCache> pNetworkDataCache_t; 


#endif // NETWORKDATACACHE
