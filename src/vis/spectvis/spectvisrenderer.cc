#include <iostream> 
#include "spectvisrenderer.h"
#include <assert.h>


SpectVisRenderer::SpectVisRenderer(SpectBlockpVector_t * pSpectBlocks) :
  pSpectBlocks_(pSpectBlocks), 
  mostRecentRenderT1_(0.0), 
  mostRecentRenderT2_(0.0),
  scale_(1.0), 
  color_("red"), 
  lastRenderInterval_(interval_t::empty())
{
  // initialize data from stream source buffer
  /* 
     this module loads the STFT data into a texture, 
  


  */ 
}



SpectVisRenderer::~SpectVisRenderer()
{
  
  
  
}

void SpectVisRenderer::newSample()
{
  //boost::weak_ptr<SpectBlock_t> newsb = pSpectBlocks_->back(); 
//   SpectBlockTextureCacheItem_t newitem; 
//   newitem.hastexture = false; 
//   newitem.textureName = 0; 

//   texCache_.push_back(newitem); 
  
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

void SpectVisRenderer::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{
  // pixels is just a hint

  mostRecentRenderT1_ = t1; 
  mostRecentRenderT2_ = t2; 

  streamtime_t scalea = pixels / (t2 -t1); 
  
//   // draw the little range indicator
//   setGLColor(0.8); 


//   glBegin(GL_LINES); 
//   glVertex2f(t1+1/scale, -float(pixheight_)/2); 
//   glVertex2f(t1+1/scale,  float(pixheight_)/2); 
//   glEnd(); 
  

  glLineWidth(8.0); 
  glColor4f(1.0, 1.0, 1.0, 1.0);
  // perform scaling
  glPushMatrix(); 
  glScalef(1.0, 1./scale_, 1.0); 
  

//   glEnable(GL_TEXTURE_RECTANGLE_ARB); 

//   SpectBlockpVector_t::iterator  i1, i2;
//   SpectBlock_t p1, p2; 

//   p1.endtime = t1; 
//   p1.starttime = t2; 

//   i1 = lower_bound(pSpectBlocks_->begin(), pSpectBlocks_->end(), 
//  		   p1, compareEndTime); 
  
//   i2 = lower_bound(pSpectBlocks_->begin(), pSpectBlocks_->end(), 
//  		   p1, compareStartTime); 
  
//   int len = i2 - i1; 

  int starti = 0;
  int endi = pSpectBlocks_->size(); 

//   // get the actual posiiton s
//   int starti = i1 - pSpectBlocks_->begin(); 
//   int endi = i2 - pSpectBlocks_->begin(); 
//   if ( ! (endi < pSpectBlocks_->size())) {
//     endi = pSpectBlocks_->size() - 1; 
//   }

//   interval_t newRenderInterval(starti, endi); 
  
//   std::cout << "Currently len = " << len << " and starti = " << starti
// 	    << " and endi = " << endi << std::endl; 

//   // DELETE THE OLD TEXTURES 
//   if (!empty(lastRenderInterval_)) {
//     // figure out what to delete 
//     for (int i = lastRenderInterval_.lower(); i <= lastRenderInterval_.upper();
// 	 i++) 
//       {
// 	assert(texCache_[i].hastexture); // sanity check; 
// 	if (in(i, newRenderInterval)) { 
// 	  // in the new interval, don't delete
// 	} else {
// 	  texCache_[i].hastexture = false; 
// 	  GLuint texnames[1] = {0}; 
// 	  texnames[0] = texCache_[i].textureName; 
// 	  std::cout << "deleting texture for index i= " << i 
// 		    << " name = " << texnames[0] << std::endl; 
// 	  //glUnbind
// 	  glDeleteTextures(1, texnames); 
// 	}
//       }
//   }

//   // now render the relevant textures

//   if (!empty(newRenderInterval)) {
//     for (int i = newRenderInterval.lower(); i <= newRenderInterval.upper();
// 	 i++) 
//       {
	
// 	if (! texCache_[i].hastexture) {
// 	  GLuint texture; 
// 	  glGenTextures(1, &texture); 
// 	  texCache_[i].textureName = texture; 
// 	  texCache_[i].hastexture = true; 
	  
// 	  glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture); 
// 	  int width = (*pSpectBlocks_)[i].width; 
// 	  int height = (*pSpectBlocks_)[i].height; 
	  
// 	  glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0 , GL_RGB, width, height, 
// 		       0, GL_RGB, GL_UNSIGNED_BYTE, 
// 		       &((*pSpectBlocks_)[i].data[0])); 
	  
// 	}
//       }    
//   }

  

//   lastRenderInterval_ = newRenderInterval; 
  
//   assert(len > 0); 
  
//   GLuint texture; 
//   glEnable(GL_TEXTURE_RECTANGLE_ARB); 

//   glGenTextures(1, &texture);
  
//   char data[128 * 128 * 4]; 
//   for (int i = 0; i < 128; i++) {
//     for (int j = 0; j < 128; j++) {
//       data[i * (128 * 4) + j * 4 + 0 ] = j*10; 
//       data[i * (128 * 4) + j * 4 + 1 ] = 0; 
//       data[i * (128 * 4) + j * 4 + 2 ] = 0; 
//       data[i * (128 * 4) + j * 4 + 3 ] = 255;
//     }
//   }


//   glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture); 

//   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
  
//   glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
//   glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

    
  for (int i = starti; i < endi; i++) {

//     glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texCache_[i].textureName); 
    float starttime = (*pSpectBlocks_)[i].starttime; 
    float endtime = (*pSpectBlocks_)[i].endtime; 
    std::cout << "rendering lines " << starttime << " " 
	      << endtime << std::endl; 
    glBegin(GL_LINE_LOOP);
//     glTexCoord2i(0, 0 );
    glVertex2f(starttime, -100.0); 
    
//     glTexCoord2i((*pSpectBlocks_)[i].width, 0);
    glVertex2f(endtime, -100.0); 
    
//     glTexCoord2i((*pSpectBlocks_)[i].width, (*pSpectBlocks_)[i].height); 
    glVertex2f(endtime, 100.0); 
    
//     glTexCoord2i(0, (*pSpectBlocks_)[i].height); 
    glVertex2f(starttime, 100.0); 
    glEnd(); 
  }

  //  glBegin(GL_LINES);
//   glVertex2f(0, 0.0); 
//   glVertex2f(t2-t1, 0.0); 
//   glEnd(); 


//   glDisable(GL_TEXTURE_RECTANGLE_ARB); 
//   int len  = i2 - i1; 
//   float fadethold = 50.0; 
//   if (scale > fadethold) {
//     setGLColor(1.0); 
//    } else {
//      setGLColor(1.0 - (fadethold - scale)/200.0);
//    }
  
//   if (scale > 200.0) {  
//      glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
//  		    &(*i1)); 
//      glDrawArrays(GL_LINE_STRIP, 0, len); 
     
//   }


//   std::vector<GLWaveQuadStrip_t>::iterator  qi1, qi2;
//   GLWaveQuadStrip_t qs1, qs2; 

//   qs1.tmax = t1; 

//   qi1 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
// 		   qs1, compareTime2); 
  
//   qs2.tmax = t2; 
//   qi2 = lower_bound(ratesS2_.begin(), ratesS2_.end(), 
// 		   qs2, compareTime2); 
  

//   int len2 = qi2 - qi1; 


//   if (scale < fadethold) {

//     setGLColor((fadethold - scale)/fadethold  + 0.5); 
//     glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t), 
//  		    &(ratesS2_[0])); 
   
    
//     glDrawArrays(GL_QUAD_STRIP, 0, 2*ratesS2_.size()); 
//     glDrawArrays(GL_LINES, 0, 2*ratesS2_.size()); 
     
    
//   }
  
//   // stupid trigger rendering
//   std::vector<wavetime_t>::iterator trigi1, trigi2; 
//   trigi1 = lower_bound(trigTimeList_.begin(), 
//  		       trigTimeList_.end(), 
//  		       t1); 

//   trigi2 = lower_bound(trigTimeList_.begin(), 
//  		       trigTimeList_.end(), 
//  		       t2); 


//   //glColor4f(0.0, 1.0, 0.0, 1.0); 
//   printStatus();
//   //std::cout << "Rendering triggers!" << trigTimeList_.size() << std::endl; 
//    for(std::vector<wavetime_t>::iterator i = trigi1; 
//        i != trigi2; i++)
//      {

//        glBegin(GL_LINE_STRIP); 
//        glVertex2f(*i, -50); 
//        glVertex2f(*i, 50); 
//        glEnd(); 
//      }
      
  glPopMatrix(); 

 }

sigc::signal<void> & SpectVisRenderer::invWaveSignal()
{
  return invWaveSignal_;
}

// void SpectVisRenderer::updateTriggers(bool reset)
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

// void SpectVisRenderer::setTriggerSource(const QueueView<wavetime_t> & tqv)
// {
//   triggerQueueView_ = tqv; 
// }

void SpectVisRenderer::setScale(float scale, float pixheight)
{
  scale_ = scale; 
  pixheight_ = pixheight; 
  invWaveSignal_.emit(); 

}

void SpectVisRenderer::setColor(Gdk::Color c)
{
  color_ = c; 
  invWaveSignal_.emit(); 

}

void SpectVisRenderer::setGLColor(float alpha)
{
  float div = 1.0;
  glColor4f(color_.get_red_p()/div,
	    color_.get_green_p()/div,
	    color_.get_blue_p()/div, 
	    alpha); 
  
  glColor4f(1.0, 1.0, 1.0, 1.0); // FIXME
  
}

void SpectVisRenderer::printStatus()
{
//   std::cout << "samples size() =" << pSamples_->size() 
// 	    << " S2 size = " << ratesS2_.size() << std::endl; 


}
