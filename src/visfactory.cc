#include "visfactory.h"
#include "vis/sinevis.h"
#include "streampipeline.h"

using namespace soma::streams; 


std::list<std::string> availableViss(FilterBase<WaveBuffer_t> * source)
{
  std::list<std::string> results; 
  results.push_back(SineVis::NAME); 

  return results; 

}


pWaveVisBase_t createVis(FilterBase<WaveBuffer_t> * parent, std::string name)
{

  pWaveVisBase_t pwf; 
  if (name == SineVis::NAME) {
    pwf.reset(new SineVis()); 
  } else {
    throw std::runtime_error("named vis not found"); 
  }
  
  // now the connection-specific information

  pwf->connect(*parent); 
  parent->getPipeline()->addVis(pwf); 
  
  return pwf; 
}

