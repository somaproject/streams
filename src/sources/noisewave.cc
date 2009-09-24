#include "noisewave.h"
#include <iostream>
#include <gtkmm.h>
#include <stdlib.h>

using namespace soma::streams; 

const std::string NoiseWave::TYPENAME = "NoiseWave"; 

NoiseWave::NoiseWave(std::string name, pTimer_t ptimer, bf::path scratch, 
		     float preload) :
  SourceBase(name), 
  amplitude(1.0), 
  noiseclass(WhiteNoise),
  pTimer_(ptimer), 
  fs_(1.0), 
  
  pSourcePad_(createSourcePad<WaveBuffer_t>(dataList_, "default"))
{

  lastTime_ = pTimer_->getStreamTime(); 
  pTimer_->streamTimeSignal().connect(sigc::mem_fun(*this, 
					      &NoiseWave::timeUpdate)); 
  
  for (double t = -preload; t < 0; t += 0.1) { 
    lastTime_ = t; 
    streamtime_t delta = 0.1; 
    nextData(delta); 
  }
}

NoiseWave::~NoiseWave()
{
  

}

void NoiseWave::nextData(streamtime_t delta)
{
 
  int FS = 1000; 
  dataList_.push_back(new WaveBuffer_t()); 
  dataList_.back().time = lastTime_; 
  dataList_.back().samprate = FS; 

  int N = delta * FS; 
  dataList_.back().data.reserve(N); 
  double freq = 1.0; 
  //freq += 10; 
  static int offset = 0; 
  double VSCALE = amplitude; 
  
  if (noiseclass == WhiteNoise ) { 
    for (int i = 0; i < N; i++) {
      float y = double(random()) / RAND_MAX - 0.5; 
      dataList_.back().data.push_back(y * VSCALE); 
    }
  } else if (noiseclass == NoisySine) {
    for (int i = 0; i < N; i++) {
      float y = 0; // = double(random()) / RAND_MAX - 0.5; 
      float x = VSCALE * sin (float(offset) / FS * 3.14152*2 * freq) + y;
      offset++; 
      dataList_.back().data.push_back(x); 
    }
  } else if (noiseclass == SquareWave) {
    for (int i = 0; i < N; i++) {
      //float y = double(random()) / RAND_MAX - 0.5; 
      //float x = VSCALE * sin (float(i) / FS * 3.14152*2 * freq) + y;
      float x; 
      if ((((i +offset) / 10) % 2) == 0) {
	x = VSCALE/2; 
      } else {

	x = -VSCALE/2; 
      }
      offset++; 
      dataList_.back().data.push_back(x); 
    }

  } else if (noiseclass == BiModal) {
    for (int i = 0; i < N; i++) {
      float y = double(random()) / RAND_MAX - 0.5; 
      float x = VSCALE * sin (float(i) / FS * 3.14152*2 * freq) + y;
      dataList_.back().data.push_back(x); 
    }
  }
  
  pSourcePad_->newData(); 
  
}

void NoiseWave::setFs(double fs) {
  fs_ = fs; 
}

void NoiseWave::timeUpdate(streamtime_t time) {
  streamtime_t oldtime = lastTime_; 
  lastTime_ = pTimer_->getStreamTime(); 
  streamtime_t delta = lastTime_ - oldtime; 
  nextData(delta); 
  
}
