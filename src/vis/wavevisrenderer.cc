#include <iostream> 
#include "wavevisrenderer.h"
#include <assert.h>


WaveVisRenderer::WaveVisRenderer() :
//   mostRecentRenderT1_(0.0), 
//   mostRecentRenderT2_(0.0),
//   emptyTriggerList_(), 
//   triggerQueueView_(emptyTriggerList_),
  scale_(1.0), 
  color_("red")
{
  for (int i = 0; i < SCALECNT; i++) {
    bufferScales_.push_back(pGLPointBufferList_t()); 
    bufferScales_.back().push_back(new GLPointBuffer_t()); 
    
    bufferScales_.back().back()->reserve(BUFFERN); 
    bufferPos_.push_back(0); 
    bufferScaleThresholds_.push_back(1000.0 / ((int)1 << i)); 
  }
  
}



WaveVisRenderer::~WaveVisRenderer()
{
  
  
  
}

void WaveVisRenderer::newSample(const WaveBuffer_t & wb)
{
  
  // construct and append new buffer
  std::vector<float>::const_iterator pdatum; 
  GLWavePoint_t p; 
  streamtime_t bufferstart = wb.time; 
  float T = 1.0 / wb.samprate; 
  int pos = 0; 
  for(pdatum = wb.data.begin();
      pdatum != wb.data.end(); pdatum++) {
    p.x = *pdatum; 
    p.t = wb.time + pos * T; 

    // for each scale: 
    for (int scale = 0; scale < SCALECNT; scale++) {
      if(scale ==0 || (bufferPos_[scale-1] % SCALEFACTOR == 0) ) {
	bufferScales_[scale].back()->push_back(p); 
	bufferPos_[scale]++; 
	
	if (bufferPos_[scale] == BUFFERN) {
	  bufferScales_[scale].push_back(new GLPointBuffer_t()); 
	  bufferScales_[scale].back()->reserve(BUFFERN); 
	  bufferPos_[scale] = 0; 
	}
	
      }
    }
    pos++; 
  }

//   GLWavePoint_t p = pSamples_->back(); 
 
//   int S1N = 10; 
  
//   int N = pSamples_->size(); 
  
//   if (ratesS2_.empty()) {
//     GLWaveQuadStrip_t qs; 
//     qs.xmin = p.x; 
//     qs.xmax = p.x; 
//     qs.tmin = p.t; 
//     qs.tmax = p.t; 
//     ratesS2_.push_back(qs); 
//   } else if (N % S1N == 0) {
//     // examine the last N points
//     GLWavePoint_t minpoint, maxpoint; 
//     minpoint.x = 100e6;
//     maxpoint.x = -100e6; 
//     for(int i = 0; i < S1N; i++) {
//       int q =  N - i - 1; 
//       GLWavePoint_t wp = (*pSamples_)[q]; 
//       if (wp.x < minpoint.x)
// 	minpoint.x = wp.x; 
      
//       if (wp.x > maxpoint.x)
// 	maxpoint.x = wp.x; 
//     }
    
//     // now we add a rectangle
//     GLWaveQuadStrip_t pl, pr; 
    
//     // left points
//     pl.xmin = minpoint.x; 
//     pl.xmax = maxpoint.x; 
//     pl.tmin = (*pSamples_)[N-S1N].t; 
//     pl.tmax = (*pSamples_)[N-S1N].t; 

//     // right points
//     pr.xmin = minpoint.x; 
//     pr.xmax = maxpoint.x; 
//     pr.tmin = (*pSamples_)[N-1].t; 
//     pr.tmax = (*pSamples_)[N-1].t; 

//     ratesS2_.push_back(pl); 
//     ratesS2_.push_back(pr); 
      
//   }
  
//   if ( (mostRecentRenderT1_ <= p.t) and (mostRecentRenderT2_ >= p.t) )
//     {
//       invWaveSignal_.emit(); 
//     }
  

}

void WaveVisRenderer::renderStream(streamtime_t t1, streamtime_t t2, 
				   int pixels)
{
  // pixels is just a hint
  mostRecentRenderT1_ = t1; 
  mostRecentRenderT2_ = t2; 

  streamtime_t scale = pixels / (t2 -t1); 
  
  // draw the little range indicator
  setGLColor(0.8); 


  glBegin(GL_LINES); 
  glVertex2f(t1+1/scale, -float(pixheight_)/2); 
  glVertex2f(t1+1/scale,  float(pixheight_)/2); 
  glEnd(); 
  

  glLineWidth(1.0); 
  
  // perform scaling
  glPushMatrix(); 
  glScalef(1.0, 1.0/scale_, 1.0); 
  

  //   // draw horizontal axis
  glColor4f(1.0, 1.0, 1.0, 1.0); 

  glBegin(GL_LINES);
  glVertex2f(t1, 0.0); 
  glVertex2f(t2, 0.0); 
  glEnd(); 

  
  int iscale = 0; 
  for (int i = 0; i < SCALECNT; i++) {
    if (scale < bufferScaleThresholds_[i]) {
      iscale = i; 
    }
  }
//   std::cout << "pixels=" << pixels 
// 	    <<  " scale" << scale 
// 	    << " iscale=" << iscale << std::endl; 

  // at our current scale, find the relevant buffer
  // FIXME: Right now linear! UGH! 
  
  pGLPointBufferList_t::const_iterator buff, buffstart, buffend; 
  buffstart = bufferScales_[iscale].begin(); 
  buffend = buffstart; 
  
  for(buff = bufferScales_[iscale].begin(); buff != bufferScales_[iscale].end(); 
      ++buff) 
    {
      if (!(*buff)->empty()) {
	  
	// look at front and back elements
	if ((*buff)->front().t < t1) {
	  buffstart = buff; 
	}

	if ((*buff)->front().t < t2) {
	  buffend = buff; 
	}
	
      }
    }


  //std::cout << "t1 =" << t1 << "   t2=" << t2 << std::endl;
  // now we should be able to plot!
  streamtime_t dstartt, dendt; 
  //std::cout << "plotting from" << (*buffstart)->front().t << " to " << (*buffend)->back().t << std::endl;
  buffend++; 
  for (buff = buffstart; buff !=buffend; buff++) {
    glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
		    &(*(*buff))[0]); 
  glDrawArrays(GL_LINE_STRIP, 0, (*buff)->size()); 
  
  }
  //

  glPopMatrix(); 

}

sigc::signal<void> & WaveVisRenderer::invWaveSignal()
{
  //  return invWaveSignal_;
}

// void WaveVisRenderer::updateTriggers(bool reset)
// {
//   if (reset) {

//     trigTimeList_.clear(); 
//     triggerQueueView_.reset(); 
//   }
//   else 
//     {
//       while ( not triggerQueueView_.empty()) {
// 	trigTimeList_.push_back(triggerQueueView_.front()); 
// 	triggerQueueView_.pop(); 
//       }
      
//     }
// }

// void WaveVisRenderer::setTriggerSource(const QueueView<wavetime_t> & tqv)
// {
//   triggerQueueView_ = tqv; 
// }

void WaveVisRenderer::setScale(float scale, float pixheight)
{
  scale_ = scale; 
  pixheight_ = pixheight; 
  invWaveSignal_.emit(); 

}

void WaveVisRenderer::setColor(Gdk::Color c)
{
  color_ = c; 
  invWaveSignal_.emit(); 

}

void WaveVisRenderer::setGLColor(float alpha)
{
  float div = 1.0;
  glColor4f(color_.get_red_p()/div,
	    color_.get_green_p()/div,
	    color_.get_blue_p()/div, 
	    alpha); 
  
  glColor4f(1.0, 1.0, 1.0, 1.0); // FIXME
  
}

void WaveVisRenderer::printStatus()
{
//   std::cout << "samples size() =" << pSamples_->size() 
// 	    << " S2 size = " << ratesS2_.size() << std::endl; 


}
