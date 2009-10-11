#ifndef __SPECTVIS_FFTENGINE_H__
#define __SPECTVIS_FFTENGINE_H__

#include <sigc++/sigc++.h>

#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>
#include <data/wave.h>
#include <list>
#include <map>
#include <set>
#include "helper.h"

#include "types.h"

namespace spectvis {

class FFTEngine 
{
  /*
    FFTEngine is responsible for keeping the FFT DB up-to-date, 
    and performs the FFT computations in a separate thread. 
    This way we can test performance independent of the wave vis
    element.  

    BIN: a chunk of contiguous data for FFT computation. Equal
    in length to windowsize / overlapfactor. 

    
    1. we use the Rangequery object to get the available wavebuffers
    for a particular window. 

    2. We use the "is_coverage_complete" function to examine these buffers
    and determine if they are "complete", that is, if we have samples
    for all of them. 
    
    3. 
  */
  
public:

  typedef boost::function<pFFT_t (float * ptr_to_data, int data_size, int N, float fs)> fft_op_t; 
  // arguments are: 
  //   float *   ptr_to_data
  //   int       length of data
  //   int N     length of FFT
  //   float fs  sampling rate

   FFTEngine(fft_op_t); 
  ~FFTEngine(); 

  void reset(); 

  void process(int MAXCNT); 
  /* Actually do the FFTs */ 


  void appendData(const WaveBuffer_t & wb); 
  void appendData(pWaveBuffer_t wb); 

  void set_fftN(int); 
  /* How many points is each FFT over */ 
  
  
  void set_windowsize(timeid_t); 
  /* How long is the window size, in ns?  */ 
  
  void set_overlapFactor(int); 
  /* how many times do we overlap? */ 
  

  std::list<pFFT_t> getFFT(timeid_t start, timeid_t end); 
  /* Return the FFTs in this window -- we might need
     a better interface to this at some point */ 
  typedef sigc::signal<void, pFFT_t>  newfft_signal_t; 

  inline newfft_signal_t & newfft_signal() {
    return newfft_signal_; 
  }

  void recompute() ; 


private:

  fft_op_t fftop_;  
  int fftN_; 
  timeid_t winsize_; 
  int overlap_; 
  size_t uid_;

  typedef std::map<timeid_t, pWaveBuffer_t> datacache_t; 
  datacache_t datacache_; 
  
  typedef std::map<bufferid_t, pFFT_t> fftcache_t; 
  fftcache_t cache_; 
  
  RangeQuery<pWaveBuffer_t> rangequery_; 
  
  typedef std::set<bufferid_t> pending_buffer_set_t; 
  pending_buffer_set_t pending_; 
  pending_buffer_set_t workqueue_; 
 
  void reload_pending_from_datacache();

  bool check_all_data_present(bufferid_t ); 

  std::vector<float> get_buffer_data(bufferid_t id, float * fs); 
  
  size_t gen_fft_id(); 
  newfft_signal_t newfft_signal_; 
  


};

}
#endif
