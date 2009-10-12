#include <iostream> 
#include <boost/foreach.hpp>
#include "spectvisrenderer.h"
#include <assert.h>
#include "shaderutil/shaders.h"
#include "shaderprogs.h"
namespace spectvis { 

SpectVisRenderer::SpectVisRenderer(FFTEngine & eng, DownsampleCache  & dscache) : 
  fftengine_(eng),
  dscache_(dscache), 
  scale(1.0),
  gpuProgCompiled_(false)
{
  // initialize data from stream source buffer
  /* 
     this module loads the STFT data into a texture, 
  


  */ 
}



SpectVisRenderer::~SpectVisRenderer()
{
  
  
  
}

// void SpectVisRenderer::newSample()
// {
//   //boost::weak_ptr<SpectBlock_t> newsb = pSpectBlocks_->back(); 
// //   SpectBlockTextureCacheItem_t newitem; 
// //   newitem.hastexture = false; 
// //   newitem.textureName = 0; 

// //   texCache_.push_back(newitem); 
  
// //   int S1N = 10; 
  
// //   int N = pSamples_->size(); 
  
// //   if (ratesS2_.empty()) {
// //     GLWaveQuadStrip_t qs; 
// //     qs.xmin = p.x; 
// //     qs.xmax = p.x; 
// //     qs.tmin = p.t; 
// //     qs.tmax = p.t; 
// //     ratesS2_.push_back(qs); 
// //   } else if (N % S1N == 0) {
// //     // examine the last N points
// //     GLWavePoint_t minpoint, maxpoint; 
// //     minpoint.x = 100e6;
// //     maxpoint.x = -100e6; 
// //     for(int i = 0; i < S1N; i++) {
// //       int q =  N - i - 1; 
// //       GLWavePoint_t wp = (*pSamples_)[q]; 
// //       if (wp.x < minpoint.x)
// // 	minpoint.x = wp.x; 
      
// //       if (wp.x > maxpoint.x)
// // 	maxpoint.x = wp.x; 
// //     }
    
// //     // now we add a rectangle
// //     GLWaveQuadStrip_t pl, pr; 
    
// //     // left points
// //     pl.xmin = minpoint.x; 
// //     pl.xmax = maxpoint.x; 
// //     pl.tmin = (*pSamples_)[N-S1N].t; 
// //     pl.tmax = (*pSamples_)[N-S1N].t; 

// //     // right points
// //     pr.xmin = minpoint.x; 
// //     pr.xmax = maxpoint.x; 
// //     pr.tmin = (*pSamples_)[N-1].t; 
// //     pr.tmax = (*pSamples_)[N-1].t; 

// //     ratesS2_.push_back(pl); 
// //     ratesS2_.push_back(pr); 
      
// //   }
  
// //   if ( (mostRecentRenderT1_ <= p.t) and (mostRecentRenderT2_ >= p.t) )
// //     {
// //       invWaveSignal_.emit(); 
// //     }
  

// }

void SpectVisRenderer::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{

  checkGPUProgCompiled(); 
  // pixels is just a hint

//   mostRecentRenderT1_ = t1; 
//   mostRecentRenderT2_ = t2; 

  streamtime_t windowsize = t2 - t1; 
  timeid_t windowsize_ns = timeid_t(windowsize * 1e9);

  streamtime_t scalea = pixels / (t2 -t1); 
  
  timeid_t timeid_t1 = timeid_t(t1 * 1e9); 
  timeid_t timeid_t2 = timeid_t(t2 * 1e9); 

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
  glScalef(1.0, 1./scale, 1.0); 
  



  glEnable(GL_TEXTURE_RECTANGLE_ARB); 

  timeid_t threshold = 100 * elements::TIMEID_SEC;
  if ((timeid_t2 - timeid_t1) < threshold) { 
    // just render low threshold 
    std::cout << "rendering high res" << std::endl;
    render_high_res_stream(timeid_t1, timeid_t2, pixels); 
  } else {
    std::cout << "rendering low res res" << std::endl;
    render_low_res_stream(timeid_t1, timeid_t2, pixels); 
  }

  // clear the cache if necessary
  while(texturecache_.size() > MAX_TEXTURE) { 
    GLuint texval = texturecache_.pop_old(); 
    glDeleteTextures(1, &texval); 
  }
  while(dstexturecache_.size() > MAX_TEXTURE) { 
    GLuint texval = dstexturecache_.pop_old(); 
    glDeleteTextures(1, &texval); 
  }

  //   cursorp->close(); 
//   //  free(buffer); 
  glDisable(GL_TEXTURE_RECTANGLE_ARB); 

  glPopMatrix(); 

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

// void SpectVisRenderer::setScale(float scale, float pixheight)
// {
//   scale_ = scale; 
//   pixheight_ = pixheight; 
//   invWaveSignal_.emit(); 

// }

// void SpectVisRenderer::setColor(Gdk::Color c)
// {
//   color_ = c; 
//   invWaveSignal_.emit(); 

// }

// void SpectVisRenderer::setGLColor(float alpha)
// {
//   float div = 1.0;
//   glColor4f(color_.get_red_p()/div,
// 	    color_.get_green_p()/div,
// 	    color_.get_blue_p()/div, 
// 	    alpha); 
  
//   glColor4f(1.0, 1.0, 1.0, 1.0); // FIXME
  
// }

// void SpectVisRenderer::printStatus()
// {
// //   std::cout << "samples size() =" << pSamples_->size() 
// // 	    << " S2 size = " << ratesS2_.size() << std::endl; 


// }

void SpectVisRenderer::render_high_res_stream(timeid_t timeid_t1, timeid_t timeid_t2, int pix)
{
 
  std::list<pFFT_t> ffts = fftengine_.getFFT(timeid_t1, timeid_t2); 
  std::cout << "highres ffts count = " << ffts.size() << std::endl;

  streamtime_t t1 = double(timeid_t1)/1e9; 
  streamtime_t t2 = double(timeid_t2)/1e9; 

  useGPUProgram(gpuProg_); 

  BOOST_FOREACH(pFFT_t pfft, ffts) { 
    /* check for textures */ 

    if(!texturecache_.exists(pfft->uid))  {
      // not in cache, create
      GLuint texture; 
      glGenTextures(1, &texture); 
      glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture); 

      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
      glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
      glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

      glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0 ,GL_LUMINANCE,  // GL_RGBA, 
		   1, pfft->data.size(),  0, GL_LUMINANCE, 
		   GL_FLOAT, 
		   &(pfft->data[0])); 

      texturecache_.insert(pfft->uid, texture); 
    }
    
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texturecache_.get(pfft->uid)); 
    timeid_t starttime_id = pfft->starttime; 
    float starttime = double(starttime_id)/1e9; 
    timeid_t window_endtime = pfft->endtime; 
    timeid_t width = window_endtime - starttime_id; 
    timeid_t endtime_id = starttime_id + width / pfft->overlap; 
    float endtime = double(endtime_id)/1e9; 
//     std::cout << "plotting " << starttime 
// 	      << " window_endtime=" << window_endtime 
// 	      << " computed endtime " 
// 	      << " " << endtime 
// 	      << " overlap =" << pfft->overlap << std::endl;

    float alpha=1.0; 
    GLint unialpha = glGetUniformLocation(gpuProg_,"alpha");
    glUniform1f(unialpha, alpha); 

    float max=1.0; 
    GLint unimax = glGetUniformLocation(gpuProg_,"maxval");
    glUniform1f(unimax, max); 

    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0 );
    glVertex2f(starttime - t1, -100.0); 
    
    glTexCoord2f(pfft->N, 0 );
    glVertex2f(endtime - t1, -100.0); 
    
    glTexCoord2f(pfft->N, pfft->N );
    glVertex2f(endtime -t1, 100.0); 
    
    glTexCoord2f(0, pfft->N );
    glVertex2f(starttime -t1, 100.0); 
    glEnd(); 

  }

  useGPUProgram(0); 

}

void SpectVisRenderer::render_low_res_stream(timeid_t timeid_t1, timeid_t timeid_t2, int pix)
{
  // first, ask the downsampled cache for any elements it might have
  
  std::list<pDSFFT_t> dsffts =  dscache_.getDSFFTs(timeid_t1, timeid_t2); 
  std::cout << "Lowres DSFFT count = " << dsffts.size() << std::endl;
  // create / use textures

  streamtime_t t1 = double(timeid_t1)/1e9; 
  streamtime_t t2 = double(timeid_t2)/1e9; 

  BOOST_FOREACH(pDSFFT_t pfft, dsffts) { 
    if(!dstexturecache_.exists(pfft->uid)) { 
      
      GLuint texture; 
      glGenTextures(1, &texture); 
      glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture); 
      
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
      glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
      glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
      
      size_t height = pfft->data.shape()[0]; 
      size_t width = pfft->data.shape()[1]; 
      glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0 ,GL_LUMINANCE,  // GL_RGBA, 
		   width, height,  0, GL_LUMINANCE, 
		   GL_FLOAT, 
		   pfft->data.data()); 
      dstexturecache_.insert(pfft->uid, texture); 
    }

    
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, dstexturecache_.get(pfft->uid)); 

    timeid_t starttime_id = pfft->starttime; 
    float starttime = double(starttime_id)/1e9; 
    timeid_t window_endtime = pfft->endtime; 
    timeid_t width = window_endtime - starttime_id; 
    timeid_t endtime_id = starttime_id + width; 
    float endtime = double(endtime_id)/1e9; 
//     std::cout << "plotting " << starttime 
// 	      << " window_endtime=" << window_endtime 
// 	      << " computed endtime " 
// 	      << " " << endtime 
// 	      << " overlap =" << pfft->overlap << std::endl;
    
    size_t texwidth = pfft->data.shape()[1]; 
    size_t texheight = pfft->data.shape()[0]; 
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0 );
    glVertex2f(starttime - t1, -100.0); 
    
    glTexCoord2f(texwidth, 0 );
    glVertex2f(endtime - t1, -100.0); 
    
    glTexCoord2f(texwidth, texheight);
    glVertex2f(endtime -t1, 100.0); 
    
    glTexCoord2f(0, texheight);
    glVertex2f(starttime -t1, 100.0); 
    glEnd(); 

  }
  
}

void SpectVisRenderer::checkGPUProgCompiled() { 
  if (!gpuProgCompiled_) {
    
    GLuint vshdr = loadGPUShader(glstring_vertex, GL_VERTEX_SHADER); 
    GLuint fshdr = loadGPUShader(glstring_fragment, GL_FRAGMENT_SHADER); 
    std::list<GLuint> shaders; 
    shaders.push_back(vshdr); 
    shaders.push_back(fshdr); 
    gpuProg_ = createGPUProgram(shaders); 

    GLint texSampler; 
    texSampler = glGetUniformLocation(gpuProg_, "tex"); 
    glUniform1i(texSampler, GL_TEXTURE0); 

    gpuProgCompiled_  = true; 
  }
}
}
