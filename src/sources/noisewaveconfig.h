#ifndef __NOISEWAVE_CONFIG_H__
#define __NOISEWAVE_CONFIG_H__

#include "jsonutil.h"
#include "noisewave.h"


pNoiseWave_t NoiseWave_config(std::string, pTimer_t, 
			      bf::path path, 
			      const json_util::mObject & val); 

#endif
