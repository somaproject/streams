#include "noisewave2.h"
#include <iostream>
#include <gtkmm.h>
#include <stdlib.h>

const std::string NoiseWave2::TYPENAME = "NoiseWave2"; 

NoiseWave2::NoiseWave2(std::string name, bf::path scratch) :
  SourceBase(name), 
  amplitude(1.0), 
  noiseclass(WhiteNoise),
  preload(0), 
  samplingrate(1000.), 
  activetime(std::make_pair(0, 0)), 
  pSourcePad_(createSourcePad<WaveBuffer_t>("default")), 
  frequency(1000.0),
  lasttime_(0)
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
    reset = true; 
  }


  if (reset) { 
    reset_preload_data(); 
    reset_sent_flags(); // warning, might take a very long time
    send_reset_token(); 
    
    data_iters_ = std::stack<datamap_t::iterator>(); 
  }

  // always generate the new data
  create_new_data(tid); 
  
  create_outstanding_preload_data(); 
  
  // now, what to send? this is actually an easy part? 
  send_preload(); 
  send_data(); 

}

void NoiseWave2::reset_preload_data()
{
  std::cout << "RESETTING PRELOAD DATA" << std::endl; 
  preload_iters_ = std::stack<datamap_t::iterator>(); 
  preload_data_.clear(); 
  remaining_preload_pos_ = -int(preload) * 60 * elements::TIMEID_PER_SEC; 
}

void NoiseWave2::reset_sent_flags()
{
 
  BOOST_FOREACH(datamap_t::value_type & x, data_) 
    {
      x.second->set_sent(false);       
    }
  
  BOOST_FOREACH(datamap_t::value_type & x, preload_data_) 
    {
      x.second->set_sent(false); 
    }
}

void NoiseWave2::send_reset_token()
{
  pSourcePad_->reset(); 
  
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
      if ((((starttime + i * period_ns ) / 1000000) % 2) == 0) {
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
    
    pSentBufferWrapper_t sb(new SentBufferWrapper(newdata.second)); 

    preload_data_.insert(std::make_pair(remaining_preload_pos_, sb)); 

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
    
    pSentBufferWrapper_t sb(new SentBufferWrapper(newdata.second)); 

    data_.insert(std::make_pair(lasttime_, sb)); 
    
    // now actually send? 

    pSourcePad_->newData(lasttime_, lasttime_+ newdata.first, 
			 *(sb->data())); // FIXME double-copy? 
    sb->set_sent(true); 
    
    lasttime_ += newdata.first; 
    threshold = (period_ns) * BUFSIZE + lasttime_;      
  }
  


}

void NoiseWave2::send_preload()
{
  int sendcnt = 0; 
  int MAXSEND = 10; 
  // this is the stupidest policy ever
  BOOST_FOREACH(datamap_t::value_type & v, preload_data_) {
    if(!(v.second->sent())) {
      // send the data 
      v.second->set_sent(true); 
      sendcnt++; 
      
      pWaveBuffer_t wb = v.second->data(); 
      pSourcePad_->newData(wb->time, 
			   wb->time + (wb->samprate * wb->data.size()) *
			   elements::TIMEID_PER_SEC, 
			   *wb);
      

    }
    if (sendcnt == MAXSEND) {
      break; 
    }
  }
  

}


void NoiseWave2::send_data()
{
  int sendcnt = 0; 
  int MAXSEND = 10; 
  // this is the stupidest policy ever
  BOOST_FOREACH(datamap_t::value_type & v, data_) {
    if(!(v.second->sent())) {
      // send the data 
      v.second->set_sent(true); 
      sendcnt++; 
      pWaveBuffer_t wb = v.second->data(); 
      pSourcePad_->newData(wb->time, 
			   wb->time + (wb->samprate * wb->data.size()) *
			   elements::TIMEID_PER_SEC, 
			   *wb);

    }
    if (sendcnt == MAXSEND) {
      break; 
    }
  }
  
}
