#ifndef __PA_SOURCE_H__
#define __PA_SOURCE_H__

/*
  An object designed to encapsulate some of the pulse audio
  server functionality. 
  
  The PA API is very... static-function-focused, because it's a C API
  with a lot of async callbacks. 

  
*/ 
#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>
#include <sigc++/sigc++.h>

class PASource
{
  
public:
  PASource(); 
  ~PASource(); 

  // signal that's float, a float pointer, and a long buffer

  pa_mainloop * mainloop_;

  pa_context *context_;
  pa_stream *stream_;
  char * device_name_; 
  char * device_description_; 
  enum mode_t {
    PLAYBACK,
    RECORD
  };
  mode_t mode_; 
  virtual void pushData(const float *d, unsigned l);
  pa_sample_spec nss_;

  sigc::signal<void, const float *, unsigned> datasig; 

  void process(int timeout); 

}; 



#endif 
