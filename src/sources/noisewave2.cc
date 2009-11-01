#include "noisewave2.h"
#include <iostream>
#include <boost/bind.hpp>
#include <gtkmm.h>
#include <stdlib.h>

const std::string NoiseWave2::TYPENAME = "NoiseWave2"; 

NoiseWave2::NoiseWave2(std::string name, bf::path scratch) :
  SourceBase(name), 
  amplitude(1.0), 
  noiseclass(WhiteNoise),
  preload(0.0), 
  samplingrate(1000.), 
  activetime(elements::timewindow_t(0, 0)),
  pSourcePad_(createSourcePad<pWaveBuffer_t>("default", 
					     boost::bind(&NoiseWave2::get_src_data, this, _1), 
					     boost::bind(&NoiseWave2::get_sequence, this))), 
  frequency(1000.0),
  lasttime_(0),
  seqid_(0), 
  remaining_preload_pos_(0)
{
  
}

NoiseWave2::~NoiseWave2()
{
  

}

void NoiseWave2::process(elements::timeid_t tid)
{
  /*

    This is an example pipeline element. There are three
    properties: 
    1. amplitude
    2. noiseclass
    3. preload

    A change in preload is the only one that causes a complete-and-total
    recomputation, for which we send a RESET packet. 
    
    there are two components to our data strategy: what to send, and
    what to use? 

    how do we prioritize sending the "active window's data"? Well,
    compared to everything else, sending is really easy! Generating
    is hard. 

   */

  // Process properties
  if(amplitude.pendingRequest() ) {
    amplitude.set_value(amplitude.get_req_value()); 
  }

  if(noiseclass.pendingRequest() ) {
    noiseclass.set_value(noiseclass.get_req_value()); 
  }

  if(samplingrate.pendingRequest()) {
    samplingrate.set_value(samplingrate.get_req_value()); 
  }

  if(frequency.pendingRequest()) {
    frequency.set_value(frequency.get_req_value()); 
  }

  bool reset = false; 
  if(preload.pendingRequest() ) {
    std::cout << "Changed preload value, resetting" << std::endl; 
    preload.set_value(preload.get_req_value()); 
    remaining_preload_pos_ = - preload.get_req_value() * 60 * elements::TIMEID_SEC; 
    reset = true; 
  }


  if (reset) { 
    preload_data_.clear(); 
    seqid_++; 
  }

  // always generate the new data
  create_new_data(tid); 
  
  create_outstanding_preload_data(); 
  

}

size_t NoiseWave2::get_sequence()
{
  return seqid_; 

}

std::pair<elements::timeid_t, pWaveBuffer_t> 
NoiseWave2::createDataBuffer(elements::timeid_t starttime, elements::timeid_t endtime )
{
  /* create a data buffer that begins at start time and
     tries as hard as possible to end at end time; 
     
     return a pointer to the buffer, and the remaining time
  */ 
  
  assert(endtime > starttime); 
  // how many data points
  elements::timeid_t timedelta = endtime - starttime; 
  double fdelta = double(timedelta) / elements::TIMEID_PER_SECF; 
  
  double FS = samplingrate; 

  double Nf = fdelta / (1./ FS); 
  int N = int(floor(Nf)); 

  pWaveBuffer_t wb(new WaveBuffer_t); 
  wb->data.reserve(N); 
  wb->time = starttime; 
  wb->samprate = FS; 

  // integer fs
  elements::timeid_t period_ns = long(round((1./FS)*elements::TIMEID_PER_SECF)); 

  double freq = 100.0; 
  //freq += 10; 
  double VSCALE = amplitude; 
  
  if (noiseclass == WhiteNoise ) { 
    for (int i = 0; i < N; i++) {
      float y = double(random()) / RAND_MAX - 0.5; 
      wb->data.push_back(y * VSCALE); 
    }
  } else if (noiseclass == NoisySine) {
    for (int i = 0; i < N; i++) {
      float y = 0; // = double(random()) / RAND_MAX - 0.5; 
      float x = VSCALE * sin ((starttime/elements::TIMEID_PER_SECF + i* 1.0/ FS) * 3.14152*2 * freq) + y;
      wb->data.push_back(x); 
    }
  } else if (noiseclass == SquareWave) {
    for (int i = 0; i < N; i++) {
      //float y = double(random()) / RAND_MAX - 0.5; 
      //float x = VSCALE * sin (float(i) / FS * 3.14152*2 * freq) + y;
      float x; 
      //if ((((starttime + i * period_ns ) / 1000000) % 2) == 0) {
      if(i %2 == 0) { 
	x = VSCALE/2; 
      } else {

	x = -VSCALE/2; 
      }
      wb->data.push_back(x); 
    }

  } else if (noiseclass == BiModal) {
    for (int i = 0; i < N; i++) {
      float y = double(random()) / RAND_MAX - 0.5; 
      float x = VSCALE * sin (float(i) / FS * 3.14152*2 * freq) + y;
      wb->data.push_back(x); 
    }
  }
  
  //

  assert(timedelta >= (N * period_ns)); 
  return std::make_pair(N*period_ns, wb); 

}


void NoiseWave2::create_outstanding_preload_data()
{
  double FS = samplingrate; 

  const int BUFSIZE = 256; 
  elements::timeid_t period_ns = long(round((1./FS)*elements::TIMEID_PER_SECF)); 

  if (remaining_preload_pos_ < 0) {
    elements::timeid_t endpos = remaining_preload_pos_ + BUFSIZE * period_ns; 
    
    std::pair<timeid_t, pWaveBuffer_t> newdata
      = createDataBuffer(remaining_preload_pos_, endpos); 

    preload_data_.insert(std::make_pair(remaining_preload_pos_, newdata.second )); 

    remaining_preload_pos_ += newdata.first; 
    
  }

}

void NoiseWave2::create_new_data(elements::timeid_t tid)
{
  double FS = samplingrate; 

  const int BUFSIZE = 128; 
  // integer fs
  elements::timeid_t period_ns = long(round((1./FS)*elements::TIMEID_PER_SECF)); 


  elements::timeid_t threshold = (period_ns) * BUFSIZE + lasttime_; 
  while (tid > threshold) { 
    std::pair<timeid_t, pWaveBuffer_t> newdata
      = createDataBuffer(lasttime_, threshold); 
    

    data_.insert(std::make_pair(lasttime_, newdata.second)); 
    
    // now actually send? 

    
    lasttime_ += newdata.first; 
    threshold = (period_ns) * BUFSIZE + lasttime_;      
  }

}

elements::datawindow_t<pWaveBuffer_t> NoiseWave2::get_src_data(const elements::timewindow_t & tw)
{
  /* 
     The heuristics here are actually a bit tricky, because we can
     only send one interval's worth of data, and of course, we need to
     do it consistently.
     
     
  */ 

  elements::datawindow_t<pWaveBuffer_t> wb; 
  wb.sequenceid = seqid_; 
  
  if (remaining_preload_pos_ >= 0) {
    // we're doing with preloading, so you can include the gap
    // 
    // fixme
    copy_data_map_range_to_output(preload_data_, tw.start, tw.end, wb); 
    copy_data_map_range_to_output(data_, tw.start, tw.end, wb); 

    if (!wb.data.empty()) {
      wb.interval = elements::timeinterval_t(tw.start, 
					     wb.data.back()->time); 
    } else {
      /* we did the query and there was no data. At the very least, if
	 this was a query of data before time zero, 
	 then we know it was true
      */ 
      wb.interval = elements::timeinterval_t(tw.start, -1); 

    }


  } else {
    /* 
       We're still preloading, so if the interval contains time < 0, 
       then we need to do some work. 
    */ 

    bool do_positive = false; 
    
    if(tw.start >= 0) { 
      // only positive
      do_positive = true; 
    } else if (tw.end < 0) { 
      //only negative, thus preload
      do_positive = false; 
    } else { 
      // randomly pick 
      if(double(rand())/RAND_MAX > 0.5) {
	do_positive = true; 
      }
    }
    
    
    if (do_positive) { 
      copy_data_map_range_to_output(data_, tw.start, tw.end, 
				    wb); 
    } else { 
      copy_data_map_range_to_output(preload_data_, tw.start, tw.end, 
				    wb); 

    }
    if (!wb.data.empty()) {
      wb.interval = elements::timeinterval_t(wb.data.front()->time, 
					     wb.data.back()->time); 
    }
  }
  return wb; 
  
}

void NoiseWave2::copy_data_map_range_to_output(datamap_t & dm, 
					       timeid_t start, timeid_t end,
					       elements::datawindow_t<pWaveBuffer_t> & wb) {
  if (dm.empty()) {
    return; 
  }
  datamap_t::iterator i = dm.lower_bound(start); 
    
  if (i != dm.begin()) {
    i--; 
  }
  
  

  for (i; ((i != dm.end() ) and (i->first <= end)); ++i) {
    if (i->first >= start) {
      wb.data.push_back(i->second); 
    }
  }
  
}
