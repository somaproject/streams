#ifndef __SPECTVIS_DOWNSAMPLE_H__
#define __SPECTVIS_DOWNSAMPLE_H__

#include <list> 
#include <set>
#include <map>

#include "types.h"


namespace spectvis {

/* A spect-vis cache that downsamples the added
   data, aggregates it, and returns an image. 

   Relies on bin ID, which means that it's invalidated whenever
   we have a change in bin-related parameters. Also, it requires
   the FFTs that come in have the same sampling rate parameters. 
   
   bincnt : how many samples we want in the output
   dsfactor: how many samples we combine into one
   
   
*/ 

class DownsampleCache {
  
public:
  DownsampleCache(int bincnt, int dsfactor); 
  void reset(); 
  void addFFT(pFFT_t); 
  
  std::list<pDSFFT_t>  getDSFFTs(timeid_t starttime, timeid_t endtime); 
private:
  int ds_bincnt_; 
  int ds_factor_; 
  int intbinsize_; 
  typedef int64_t intbin_t; // internal bin
  
  typedef std::map<intbin_t, pDSFFT_t> dscache_t; 
  dscache_t cache_; 

  typedef std::map<bufferid_t, pFFT_t> fftcache_t; 
  fftcache_t fftcache_; // can be unordered, woot.
  
  bool can_downsample(intbin_t); 

  void create_downsample(intbin_t); 
  
  size_t uid_; 

  size_t get_uid() { 
    return uid_++; 

  }
  
};
   





}; 


#endif
