#include "visfactory.h"

#include "sinevis.h"
#include "wavevis.h"

pIVis_t VisFactory::create(std::string typname) {
  
  std::string defaultname = "visname"; 
  
  if (typname == SineVis::TYPENAME) {
    pSineVis_t sv(new SineVis(defaultname)); 
    return sv; 

  } else if (typname == WaveVis::TYPENAME) {
    pWaveVis_t wv(new WaveVis(defaultname)); 
    return wv; 

  } else {
    throw std::runtime_error("Unknown vis type " + typname); 
  }

}
