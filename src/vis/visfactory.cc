#include "visfactory.h"

#include "sinevis.h"
#include "wavevis.h"
#include "spectvis.h"

pIVis_t VisFactory::create(std::string typname, bf::path scratchdir) {
  
  std::string defaultname = "visname"; 
  
  if (typname == SineVis::TYPENAME) {
    pSineVis_t sv(new SineVis(defaultname, scratchdir)); 
    return sv; 

  } else if (typname == WaveVis::TYPENAME) {
    pWaveVis_t wv(new WaveVis(defaultname, scratchdir)); 
    return wv; 

  } else if (typname == SpectVis::TYPENAME) {
    pSpectVis_t sv(new SpectVis(defaultname, scratchdir)); 
    return sv; 

  } else {
    throw std::runtime_error("Unknown vis type " + typname); 
  }

}
