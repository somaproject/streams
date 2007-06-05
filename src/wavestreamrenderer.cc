#include <iostream> 
#include "wave.h"

WaveStreamRenderer::WaveStreamRenderer(StreamSource * ss) :
{
  pStreamSource_ = ss; 
  

  streamSourceDataBegin_ = ss->getDataPointer()->begin(); 
 streamSourceDataEnd_ != 

  // initialize data from stream source buffer
  while (
  // connect up "new data handler" 


}



WaveStreamRenderer::~WaveStreamRenderer()
{
  
  
  
}


void WaveStreamRenderer::generateFakeData()
{
  int N = 4000000; 
  std::vector<float> x(N); 
  //rateTimeline_.appendRate(5.0); 
  for (int i = 0; i < N; i++) {
    x[i] = float(rand())/RAND_MAX - 0.5; 
    //x[i] = float(sin
  }
  
  std::vector<float> y(N); 
  int M = 20; 
  for (int i = M; i < N; i++) {
    float sum = 0.0; 
    for (int j = 0; j < M; j++) {
      sum += x[i -j]; 
    }
    y[i-M] = sum*10; 
  }
  
  for (int i = 0; i < N-M; i++)
    {
      GLWavePoint_t p; 
      p.t = float(i) / 2000.0; 
      p.x = y[i];
      append(p); 
    }
  
}

void WaveStreamRenderer::append(GLWavePoint_t p)
{
  rates_.push_back(p); 

  if (rates_.size() % 8 == 0)
    {
      // every 100 points, insert a bookmark
      if (ratesS2_.size() % 10 == 0) {
	indexS2_[p.t] = ratesS2_.size(); 
      }

      ratesS2_.push_back(p); 
      
      
    }

  if (rates_.size() % (8*8) == 0)
    {
      // every 100 points, insert a bookmark
      if (ratesS3_.size() % 10 == 0) {
	indexS3_[p.t] = ratesS3_.size(); 
      }

      ratesS3_.push_back(p); 
    }
  
  // check for triggers:
  if (rates_.size() > 2) {
    if (rates_.back().x > triggerLevel_ and 
	rates_[rates_.size() - 2].x <= triggerLevel_)
      {
	
	TriggerTimeList_t ttl; 
	ttl.push_back(p.t); 
	newTriggerSignal_.emit(ttl); 
      }
  }
}

void WaveStreamRenderer::draw(float t1, float t2, int pixels)
{

  float scale = pixels / (t2 -t1); 

  std::vector<GLWavePoint_t>::iterator  i1, i2;
  GLWavePoint_t p1, p2; 

  p1.t = t1; 

  i1 = lower_bound(rates_.begin(), rates_.end(), 
		   p1, compareTime); 
  
  p2.t = t2; 
  i2 = lower_bound(rates_.begin(), rates_.end(), 
		   p2, compareTime); 
  
  glColor4f(1.0, 1.0, 1.0, 1.0); 

  //int pos1 = i1 - rates_.begin(); 
  int len  = i2 - i1; 

  glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
		  &(*i1)); 

  glDrawArrays(GL_LINE_STRIP, 0, len); 

    
  p1.t = t1; 

  i1 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
		   p1, compareTime); 
  
  p2.t = t2; 
  i2 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
		   p2, compareTime); 
  
  glColor4f(1.0, 0.0, 0.0, 1.0); 

  //int pos1 = i1 - rates_.begin(); 
  len  = i2 - i1; 

  glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
		  &(*i1)); 

  glDrawArrays(GL_LINE_STRIP, 0, len); 


  // stupid trigger rendering
  std::vector<float>::iterator trigi1, trigi2; 
  trigi1 = lower_bound(trigTimeList_.begin(), 
		       trigTimeList_.end(), 
		       t1); 

  trigi2 = lower_bound(trigTimeList_.begin(), 
		       trigTimeList_.end(), 
		       t2); 

  glColor4f(0.0, 1.0, 0.0, 1.0); 


  for(std::vector<float>::iterator i = trigi1; 
      i != trigi2; i++)
    {
      glBegin(GL_LINE_STRIP); 
      glVertex2f(*i, -10); 
      glVertex2f(*i, 10); 
      glEnd(); 
    }
      

}

newTriggersSignal_t WaveStreamRenderer::newTriggers()
{

  return newTriggerSignal_; 

}

invalidateTriggersSignal_t WaveStreamRenderer::invalidateTriggers()
{

  return invalidateTriggersSignal_; 

}


void WaveStreamRenderer::setTriggerLevel(float tv)
{

  


}

void WaveStreamRenderer::appendTriggers(const TriggerTimeList_t & ttl)
{
  // copy the waves

  for (TriggerTimeList_t::const_iterator i = ttl.begin(); 
       i != ttl.end(); i++)
    {

      trigTimeList_.push_back(*i);
    }

}
