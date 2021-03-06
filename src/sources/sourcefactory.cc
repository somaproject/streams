#include "sourcefactory.h"

#include "noisewave2.h"
// #include "noisewaveconfig.h"
#include <sources/netdatawave/netdatawave.h>
// #include "netdataraw.h"
#include <sources/pamonitor/pamonitor.h>


pISource_t SourceFactory::create(std::string typname, pSourceState_t ss,
				 bf::path scratchdir, 
				 const json_util::mObject & config) {
  
  std::string defaultname = "sourcename"; 
  
  if (typname == NoiseWave2::TYPENAME) {
    pNoiseWave2_t nw(new NoiseWave2(defaultname, scratchdir)); 
    return nw; 

  } else if (typname == NetDataWave::TYPENAME) {
    pNetDataWave_t ndw(new NetDataWave(defaultname, ss->timer, ss->netdatacache, 
				       ss->somanetcodec, scratchdir)); 
    return ndw; 
// //   } else if (typname == NetDataRaw::TYPENAME) {
// //     pNetDataRaw_t ndw(new NetDataRaw(defaultname, ss->timer, ss->netdatacache, 
// // 				     ss->somanetcodec, scratchdir)); 
// //     return ndw; 
  } else if (typname == PulseAudioMonitorWave::TYPENAME) {
    pPulseAudioMonitorWave_t ndw(new PulseAudioMonitorWave(defaultname,
 							   scratchdir)); 
    return ndw; 
  };
  
  throw std::runtime_error("Unknown source type " + typname); 
  
}
