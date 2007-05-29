#include <iostream> 
#include "wave.h"

WaveDraw::WaveDraw()
{
  
  int N = 40000000; 
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
      p.t = i; 
      p.x = y[i];
      append(p); 
    }
  
}


WaveDraw::~WaveDraw()
{
  
  
  
}

void WaveDraw::append(GLWavePoint_t p)
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
  
}
void WaveDraw::draw(float t1, float t2, int pixels)
{

  float scale = pixels / (t2 -t1); 
  std::cout << scale << ' ' << ratesS2_.size() << ' ' << ratesS3_.size() << std::endl; 
  if (scale > 0.1) 
    {
      int tpos1, tpos2; 

      tpos1 = int(t1); 
      tpos2 = int(t2); 
      if (t1 < 0) {
	t1 = 0; 
      }
      
      if (t2 > rates_.size() - 1) {
	
	t2 = rates_.size() - 1; 
      }
      
      glLineWidth(1.0); 
      
      glColor4f(1.0, 1.0, 1.0, 1.0); 
      glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
		      &(rates_[0])); 
      
      glDrawArrays(GL_LINE_STRIP, t1, t2 - t1); 
      
      
    } 
  else if (scale  > 0.01 )  
    {
      // now, convert
      glLineWidth(1.0); 
      glColor4f(1.0, 0.0, 0.0, 1.0); 
      glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
		      &(ratesS2_[0])); 

      timeindex_t::iterator lower, upper; 
      lower = indexS2_.lower_bound(t1); 
      
      upper = indexS2_.upper_bound(t2); 
      int l = 0; 
      
      
      if (lower != indexS2_.end()  and lower != indexS2_.begin())
	{
	  lower--; 
	  l = lower->second; 
	}
      
      int u = ratesS2_.size(); 
      
      if (upper != indexS2_.end() )
	{
	  u = upper->second; 
	}
      
      glDrawArrays(GL_LINE_STRIP, l, u-l); 
  
      } 
  else 
	{
	  // now, convert
      glLineWidth(1.0); 
      glColor4f(0.0, 0.0, 1.0, 1.0); 
      glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
		      &(ratesS3_[0])); 

      timeindex_t::iterator lower, upper; 
      lower = indexS3_.lower_bound(t1); 
      
      upper = indexS3_.upper_bound(t2); 
      int l = 0; 
      
      
      if (lower != indexS3_.end()  and lower != indexS3_.begin())
	{
	  lower--; 
	  l = lower->second; 
	}
      
      int u = ratesS3_.size(); 
      
      if (upper != indexS3_.end() )
	{
	  u = upper->second; 
	}
      
      glDrawArrays(GL_LINE_STRIP, l, u-l); 
      std::cout << u-l << std::endl; 
    }

}
