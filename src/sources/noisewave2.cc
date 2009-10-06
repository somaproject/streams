#include "noisewave2.h"
#include <iostream>
#include <gtkmm.h>
#include <stdlib.h>

const std::string NoiseWave2::TYPENAME = "NoiseWave2"; 

NoiseWave2::NoiseWave2(std::string name, bf::path scratch) :
  Element(name), 
  amplitude(1.0), 
  noiseclass(WhiteNoise),
  preload(0), 
  activetime(std::make_pair(0, 0)), 
  pSourcePad_(createSourcePad<WaveBuffer_t>("default")), 
  lasttime_(0), 
  FS_(1000)
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
  bool reset = false; 

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
  preload_iters_ = std::stack<datamap_t::iterator>(); 
  preload_data_.clear(); 
  
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

  double Nf = fdelta / (1./ FS_); 
  int N = int(floor(Nf)); 

  pWaveBuffer_t wb(new WaveBuffer_t); 
  wb->data.reserve(N); 
  wb->time = starttime; 
  wb->samprate = FS_; 

  // integer fs
  elements::timeid_t fs_ns = long(round((1./FS_)*elements::TIMEID_PER_SECF)); 

  double freq = 1.0; 
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
      float x = VSCALE * sin ((starttime/elements::TIMEID_PER_SECF + i* 1.0/ FS_) * 3.14152*2 * freq) + y;
      wb->data.push_back(x); 
    }
  } else if (noiseclass == SquareWave) {
    for (int i = 0; i < N; i++) {
      //float y = double(random()) / RAND_MAX - 0.5; 
      //float x = VSCALE * sin (float(i) / FS * 3.14152*2 * freq) + y;
      float x; 
      if ((((starttime + i * fs_ns ) / 1000000) % 2) == 0) {
	x = VSCALE/2; 
      } else {

	x = -VSCALE/2; 
      }
      wb->data.push_back(x); 
    }

  } else if (noiseclass == BiModal) {
    for (int i = 0; i < N; i++) {
      float y = double(random()) / RAND_MAX - 0.5; 
      float x = VSCALE * sin (float(i) / FS_ * 3.14152*2 * freq) + y;
      wb->data.push_back(x); 
    }
  }
  
  //

  assert(timedelta >= (N * fs_ns)); 
  elements::timeid_t remainder = timedelta - (N * fs_ns); 
  
  return std::make_pair(remainder, wb); 

}


void NoiseWave2::create_outstanding_preload_data()
{

  const int BUFSIZE = 256; 
  elements::timeid_t fs_ns = long(round((1./FS_)*elements::TIMEID_PER_SECF)); 

  if (remaining_preload_pos_ < 0) {
    elements::timeid_t endpos = remaining_preload_pos_ + BUFSIZE * fs_ns; 
    
    std::pair<timeid_t, pWaveBuffer_t> newdata
      = createDataBuffer(remaining_preload_pos_, endpos); 
    
    pSentBufferWrapper_t sb(new SentBufferWrapper(newdata.second)); 

    preload_data_.insert(std::make_pair(remaining_preload_pos_, sb)); 

    remaining_preload_pos_ += newdata.first; 
    
  }

}

void NoiseWave2::create_new_data(elements::timeid_t tid)
{
  const int BUFSIZE = 256; 
  // integer fs
  elements::timeid_t fs_ns = long(round((1./FS_)*elements::TIMEID_PER_SECF)); 

  if (tid > ((fs_ns) * BUFSIZE + lasttime_)) {
    std::pair<timeid_t, pWaveBuffer_t> newdata
      = createDataBuffer(lasttime_, tid); 
    
    pSentBufferWrapper_t sb(new SentBufferWrapper(newdata.second)); 

    data_.insert(std::make_pair(lasttime_, sb)); 
    
    // now actually send? 

    pSourcePad_->newData(lasttime_, lasttime_ + (tid - newdata.first), 
			 *(sb->data())); // FIXME double-copy? 
    sb->set_sent(true); 
    
    lasttime_ += (tid - newdata.first); 
        
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
      std::cout << "sending"  << std::endl; 
    }
    if (sendcnt == MAXSEND) {
      break; 
    }
  }
  
}
