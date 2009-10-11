#include <boost/foreach.hpp>
#include <iostream>
#include "fftengine.h"


namespace spectvis {

FFTEngine::FFTEngine(fft_op_t fftop)  : 
  fftop_(fftop),
  fftN_(128),
  winsize_(100000000),
  overlap_(1),
  uid_(0)
{


}

FFTEngine::~FFTEngine()
{

}

void FFTEngine::reset() {
  // the reset signal, from upstream 

  rangequery_.reset(); 
  datacache_.clear(); 

  cache_.clear(); 
  pending_.clear(); 

}

void FFTEngine::appendData(const WaveBuffer_t & wb) {
  // get our own 
  pWaveBuffer_t pwb(new WaveBuffer_t); 
  *pwb = wb; 

  appendData(pwb); 

}

void FFTEngine::appendData(pWaveBuffer_t pwb) {
  /* 
     1. add the buffer to the range list
     2. find out if any buffers are now complete. 
     3. if they are, schedule them for processing. 

  */

  datacache_.insert(std::make_pair(pwb->time, pwb)); 

  // compute the range for this wavebuffer. Right now we assume
  // that the time means the left-most corner of the sample period. 
  timeid_t start = pwb->time; 
  
  double period = 1. / pwb->samprate; 
  double duration = pwb->data.size() * period; 
  timeid_t duration_ns = timeid_t(duration*1e9); 
  timeid_t end = pwb->time + duration_ns; 
  
  rangequery_.insert(pwb, start, end); 
  
  // now find out if any buffers are completed by this: 
  // 1. get a list of all the buffers that this data implicates

  bufferlist_t bufferlist = get_buffers_that_depend_on_times(start, end,
							     winsize_, overlap_); 
  BOOST_FOREACH(bufferid_t bid, bufferlist) {
    // 2. see if any of them are done. 
    bool present = check_all_data_present(bid); 
    // 3. if so, add them to the "can now FFT" list. 
    if (present) {

      pending_.insert(bid); 
    }
  }



}
void FFTEngine::process(int MAXCNT )
{
  /* Actually perform some of the processing  of the underlying
     function.

  */ 

  int cnt = 0; 
  while (cnt < MAXCNT) {
    if (!workqueue_.empty()) {
      bufferid_t bid = *workqueue_.begin(); 
      std::pair<timeid_t, timeid_t> times = get_bin_bounds(bid, winsize_, overlap_); 

      float fs; 
      
      std::vector<float> data = get_buffer_data(bid, &fs); 
      size_t datasize = data.size(); 
      
      pFFT_t Y = fftop_(&data[0], datasize, fftN_, fs); 
      Y->uid = gen_fft_id(); 

      Y->starttime = times.first; 
      Y->endtime = times.second; 
      Y->bufferid = bid; 
      Y->overlap = overlap_; 
      
      cache_.insert(std::make_pair(bid, Y)); 
      newfft_signal_.emit(Y); 
      pending_.erase(bid); 
      workqueue_.erase(bid);
    } else { 

      if (!pending_.empty()) {
// 	std::cout << "process: pending not empty, size=" << pending_.size() 
// 		  << std::endl;

	bufferid_t bid = *pending_.begin(); 
	std::pair<timeid_t, timeid_t> times = get_bin_bounds(bid, winsize_, overlap_); 

	float fs; 
	
	std::vector<float> data = get_buffer_data(bid, &fs); 
	size_t datasize = data.size(); 

	pFFT_t Y = fftop_(&data[0], datasize, fftN_, fs); 
	Y->uid = gen_fft_id(); 
	Y->overlap = overlap_; 
	Y->bufferid = bid; 
	Y->starttime = times.first; 
	Y->endtime = times.second; 

	newfft_signal_.emit(Y); 
	
	cache_.insert(std::make_pair(bid, Y)); 
	
	workqueue_.erase(bid);
	pending_.erase(bid);
      }
    }
    cnt++; 
  }

}

void FFTEngine::set_fftN(int n) {
  /* How many points is each FFT over */ 
  fftN_ = n; 
  cache_.clear(); 
  pending_.clear(); 
  
  // for all existing data, update pending 
  reload_pending_from_datacache(); 
  
}


void FFTEngine::set_windowsize(timeid_t size) { 
  /* How long is the window size, in ns?  

     1. if we change the window size, it means we have to rewalk through
     our range query and rediscover what work needs to be done

   */ 
  winsize_ = size; 
  cache_.clear(); 
  pending_.clear(); 

  reload_pending_from_datacache(); 
  
}


void FFTEngine::set_overlapFactor(int factor) {
  /* how many times do we overlap? 

     If these are only factors of two (2, 4, 8, 16) then
     life is potentially a lot easier, because we can later
     just store the finest-resolution bins and query against those. 
  */ 
  
  overlap_ = factor; 
  cache_.clear(); 
  pending_.clear(); 

  reload_pending_from_datacache(); 

}


std::list<pFFT_t> FFTEngine::getFFT(timeid_t start, timeid_t end) {

/* 
   Return the FFTs in this window -- we might need
   a better interface to this at some point, and some thread-safety
   guarantees 

*/ 


  // 1. find all of the implicated buffers. 
  bufferlist_t bufferlist = get_buffers_that_depend_on_times(start, end, 
							     winsize_, overlap_); 
  
  std::list<pFFT_t> results; 
  
  BOOST_FOREACH(bufferid_t bid, bufferlist) {

    // 2. if FFT has been computed, append to list
    fftcache_t::iterator ffti = cache_.find(bid); 
    
    if(ffti == cache_.end()) { 
      // miss
      if (pending_.find(bid) != pending_.end()) { 
	// we could compute this if we wanted -- move up the queue
	workqueue_.insert(bid); 
      } else { 
	// we couldn't compute even if we wanted, thus must wait; 
      }

    } else { 
      // cache HIT! 
      results.push_back(ffti->second); 
    }

  }

  return results; 

}


std::vector<float> FFTEngine::get_buffer_data(bufferid_t x, float * fs ) {
  /* 
     Get contiguous vector of buffer data. 

     Right now we assume that the sample rate is continuous, and
     so at the interface we just end up double-sampling the data. Oh well. 

     
  */ 

  // safety
  if (datacache_.empty()) {
    return std::vector<float>(); 
  }
  
  std::pair<timeid_t, timeid_t> times = get_bin_bounds(x, winsize_, overlap_); 
  
  timeid_t start = times.first; 
  timeid_t end = times.second; 
  
  datacache_t::iterator i = datacache_.lower_bound(start); 
  
  if (i != datacache_.begin()) {
    i--;
  
  }
  *fs =  i->second->samprate; 
  double period = 1.0 / i->second->samprate; 
  timeid_t period_ns = int(period * elements::TIMEID_PER_SECF); 

  std::vector<float> output; 
  output.reserve(winsize_ / period_ns + 5); // preallocate the data

  bool pushing_data = false; 

  timeid_t tpos = i->first; 
  while((i != datacache_.end()) and (i->first <= end)) {
    for (int j = 0; j < i->second->data.size(); j++) { 
      if (tpos >= start and tpos <= end) { 
	output.push_back(i->second->data[j]); 
      }
      tpos += period_ns; 
    }
    i++; 
  }

  return output; 
}


bool FFTEngine::check_all_data_present(bufferid_t x ) {
  /* 
     A heuristic to make sure we have data covering x. 
     
     
  */ 
  
  std::pair<timeid_t, timeid_t> times = get_bin_bounds(x, winsize_, overlap_); 
  
  timeid_t start = times.first; 
  timeid_t end = times.second; 
  
  datacache_t::iterator i = datacache_.lower_bound(start); 

  if (i != datacache_.begin()) {
    i--;
  }
  
  timeid_t lasttime; 
  // the real solution here is to make sure there aren't any big gaps? 
  while((i != datacache_.end()) and (i->first <= end)) {
    timeid_t tpos = i->first; 

    double period = 1.0 / i->second->samprate; 
    timeid_t period_ns = int(period * elements::TIMEID_PER_SECF); 
    double len = period * i->second->data.size(); 
    timeid_t duration = int(len * elements::TIMEID_PER_SECF); 
    lasttime = tpos + duration; 

    i++; 
    if ((i != datacache_.end()) and (i->first <= end)) {
      timeid_t gap =   i->first - (tpos + duration);

      if (gap > period_ns * 2) { 
	return false; 
      }
    }
  }
  if (lasttime >= end) { 
    return true;   
  } else { 
    return false; 
  }

}

void FFTEngine::reload_pending_from_datacache()
{

}

size_t FFTEngine::gen_fft_id()
{
  return uid_++; 
}

}
