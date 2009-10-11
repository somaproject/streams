#include <iostream>
#include "spectdownsample.h"

namespace spectvis {

DownsampleCache::DownsampleCache(int bincnt, int dsfactor) : 
  ds_bincnt_(bincnt), 
  ds_factor_(dsfactor), 
  intbinsize_(ds_bincnt_ * ds_factor_), 
  uid_(0)
{



}


void DownsampleCache::reset()
{

  cache_.clear(); 
  fftcache_.clear(); 

}


void DownsampleCache::addFFT(pFFT_t fft)
{

  fftcache_.insert(std::make_pair(fft->bufferid, fft)); 
  intbin_t intbin; 
  intbin = fft->bufferid / intbinsize_; 
  if (fft->bufferid < 0) { 
    intbin--;
  }

  if (can_downsample(intbin)) {
    create_downsample(intbin); 
  }

}


std::list<pDSFFT_t> DownsampleCache::getDSFFTs(timeid_t starttime, timeid_t endtime)
{
  std::list<pDSFFT_t> output; 
  
  dscache_t::iterator dsi = cache_.lower_bound(starttime); 
  while ((dsi != cache_.end()) and (dsi->second->starttime <= endtime)) {
    output.push_back(dsi->second); 
    dsi++; 
  }
  
  return output; 

}

bool DownsampleCache::can_downsample(intbin_t b)
{
  // check if we can downsample for a given bin

  fftcache_t::iterator ffti = fftcache_.find(b * intbinsize_); 
  if (ffti == fftcache_.end()) {
    return false; 
  }
  
  bufferid_t bufferid_pos = ffti->first; 
  size_t const_N = ffti->second->N; 
  float const_maxfreq = ffti->second->maxfreq; 
  ffti++; 
  
  int maxcnt = 0; 

  for(ffti; ffti != fftcache_.end(); ++ffti) {
    if(ffti->second->bufferid != (bufferid_pos +1) ) {

      return false; 
    }
    
    if(ffti->second->N != const_N) {
      return false; 
    }
    
    if(ffti->second->maxfreq != const_maxfreq) { 

      return false; 
    }

    maxcnt++; 
    bufferid_pos++; 
    if (bufferid_pos >= ((b+1) * intbinsize_-1)) {
      return true; 
    }

    if (maxcnt > intbinsize_) { 
      return false; 
    }
  }
  return false; 

}


void DownsampleCache::create_downsample(intbin_t b) 
{
  /* 
     NOTE THIS ONLY WORKS IF YOU ARE SURE YOU can_downsample
     
  */ 

  pDSFFT_t dsfft(new DownsampledFFT); 
  
  
  fftcache_t::iterator ffti = fftcache_.find(b * intbinsize_); 
  dsfft->starttime = ffti->second->starttime; 
  dsfft->overlap = ffti->second->overlap; 
  dsfft->bufferid = b; 
  dsfft->N = ffti->second->N; 
  dsfft->maxfreq = ffti->second->maxfreq; 
  dsfft->uid = get_uid(); 


  dsfft->data.resize(boost::extents[dsfft->N][ds_bincnt_]); 
  for (int bin_i = 0; bin_i < ds_bincnt_; bin_i++) { 
      for (int freq_i = 0; freq_i < dsfft->N; ++freq_i) {
	dsfft->data[freq_i][bin_i]  = 0.0; 
// 	if(freq_i == 0) { 
// 	  dsfft->data[freq_i][bin_i]  = 1.0; 
// 	}
      }
  }
  
  for (int bin_i = 0; bin_i < ds_bincnt_; bin_i++) { 
    for (int samp_i = 0; samp_i < ds_factor_; ++samp_i) {
      for (int freq_i = 0; freq_i < dsfft->N; ++freq_i) {
	dsfft->data[freq_i][bin_i]  += ffti->second->data[freq_i]/ ds_factor_; 
      }
      
      ffti++; 
    }
  }

  ffti--; 
  dsfft->endtime = ffti->second->endtime; 

  cache_.insert(std::make_pair(dsfft->starttime, dsfft)); 
}




}
