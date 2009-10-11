#include "visfactory.h"

// #include "sinevis.h"
// #include "wavevis/wavevis.h"
#include "spectvis/spectvis.h"
#include "wavevis2/wavevis2.h"

pIVis_t VisFactory::create(std::string typname, bf::path scratchdir) {
  
  std::string defaultname = "visname"; 
  
//   if (typname == WaveVis::TYPENAME) {
//     pSineVis_t sv(new SineVis(defaultname, scratchdir)); 
//     return sv; 

//   } else if (typname == WaveVis::TYPENAME) {
//     pWaveVis_t wv(new WaveVis(defaultname, scratchdir)); 
//     return wv; 

  if (typname == SpectVis::TYPENAME) {
    pSpectVis_t sv(new SpectVis(defaultname, scratchdir)); 
    return sv; 
  }  else if (typname == WaveVis2::TYPENAME) {
    pWaveVis2_t wv(new WaveVis2(defaultname, scratchdir)); 
    return wv; 
  } else {
    throw std::runtime_error("Unknown vis type " + typname); 
  }

}
