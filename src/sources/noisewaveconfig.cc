#include "json_spirit.h"
#include "jsonutil.h"

#include "noisewaveconfig.h"

using namespace json_util; 
using namespace json_spirit; 

pNoiseWave_t NoiseWave_config(std::string name, pTimer_t timer, 
			     bf::path scratchdir, 
			     const mObject & inval)
{
  mValue foundval; 

  pNoiseWave_t nw(new NoiseWave(name, timer, scratchdir)); 

  
  int preload = 0; 
  if (opt_find_value(inval, "preload", foundval)) {
    preload = foundval.get_int(); 
    nw->preload = preload; 
  }
  


  
  return nw; 
  
}
