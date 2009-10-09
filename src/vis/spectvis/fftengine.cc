#include <boost/foreach.hpp>

#include "fftengine.h"


namespace spectvis {

FFTEngine::FFTEngine(fft_op_t fftop) :
  fftop_(fftop),
  fftN_(128),
  winsize_(1000000000),
  overlap_(4)
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
      
      float * data; 

      size_t datasize; 
      
      float fs; 
      pFFT_t Y = fftop_(data, datasize, fftN_, fs); 
      cache_.insert(std::make_pair(bid, Y)); 
      
      pending_.erase(bid); 
      workqueue_.erase(bid);
    } else { 
      if (!pending_.empty()) {
	// nothing in the work queue, so just pick some value and do it
	bufferid_t bid = *workqueue_.begin(); 
	
	float * data; 
	
	size_t datasize; 
	
	float fs; 
	pFFT_t Y = fftop_(data, datasize, fftN_, fs); 
	cache_.insert(std::make_pair(bid, Y)); 
	
	workqueue_.erase(bid);
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


bool FFTEngine::check_all_data_present(bufferid_t x ) {


}

}
