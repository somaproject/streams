#include "filterfactory.h"
#include "filters/waveidentity.h"
#include "streampipeline.h"

using namespace soma::streams; 


std::list<std::string> availableFilters(SourceBase<WaveBuffer_t> * source)
{
  std::list<std::string> results; 
  results.push_back(WaveIdentity::NAME); 

  return results; 

}


pWaveFilterBase_t createFilter(SourceBase<WaveBuffer_t> * parent, std::string name)
{

  pWaveFilterBase_t pwf; 
  if (name == WaveIdentity::NAME) {
    pwf.reset(new WaveIdentity()); 
  } else {
    throw std::runtime_error("named filter not found"); 
  }
  
  // now the connection-specific information

  pwf->connect(*parent); 
  parent->getPipeline()->addFilter(pwf); 
  
  return pwf; 
}

