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

typedef std::vector<float> TriggerTimeList_t; 
typedef sigc::signal<void, const TriggerTimeList_t> newTriggersSignal_t; 
typedef sigc::signal<void> invalidateTriggersSignal_t; 

struct GLWavePoint_t
{
  float t; 
  float x; 
}; 


#endif // WAVE_H
