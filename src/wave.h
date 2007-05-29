#ifndef WAVE_H
#define WAVE_H

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <map>

struct GLWavePoint_t
{
  float t; 
  float x; 
}; 

typedef std::map<float, int> timeindex_t; 

class WaveDraw
{
 public: 
  WaveDraw(); 
  ~WaveDraw(); 
  void draw(float t1, float t2, int pixels); 
  void append(GLWavePoint_t p);

 protected: 
  std::vector<GLWavePoint_t> rates_; 
  std::vector<GLWavePoint_t> ratesS2_; 
  std::vector<GLWavePoint_t> ratesS3_; 
  timeindex_t indexS2_, indexS3_; 

};

#endif // WAVE_H
