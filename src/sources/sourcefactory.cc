#include "sourcefactory.h"

#include "noisewaveconfig.h"
#include "netdatawave.h"
#include "netdataraw.h"
#include "pamonitor.h"


pISource_t SourceFactory::create(std::string typname, pSourceState_t ss,
				 bf::path scratchdir, 
				 const json_util::mObject & config) {
  
  std::string defaultname = "sourcename"; 
  
  if (typname == NoiseWave::TYPENAME) {
    return NoiseWave_config(defaultname, ss->timer, scratchdir, config); 

  } else if (typname == NetDataWave::TYPENAME) {
    pNetDataWave_t ndw(new NetDataWave(defaultname, ss->timer, ss->netdatacache, 
				       ss->somanetcodec, scratchdir)); 
    return ndw; 
  } else if (typname == NetDataRaw::TYPENAME) {
    pNetDataRaw_t ndw(new NetDataRaw(defaultname, ss->timer, ss->netdatacache, 
				     ss->somanetcodec, scratchdir)); 
    return ndw; 
  } else if (typname == PulseAudioMonitorWave::TYPENAME) {
    pPulseAudioMonitorWave_t ndw(new PulseAudioMonitorWave(defaultname, ss->timer,
							   scratchdir)); 
    return ndw; 
  };
  
  throw std::runtime_error("Unknown source type " + typname); 
  
}
