#ifndef RATETIMELINE_H
#define RATETIMELINE_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <assert.h>

#include <stdlib.h>
#include <gtkmm.h>

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "glconfig.h"
#include "glspikes.h"

#include "wave.h" 

class RateTimeline : public Gtk::GL::DrawingArea
{
public:
  explicit RateTimeline(); 

  virtual ~RateTimeline();

  // Invalidate whole window.
  void inv(); 

  // Update window synchronously (fast).
  void update()
    { 
      get_window()->process_updates(false); 
    }
  void appendRenderer(WaveDraw * wd); 

protected:

  // signal handlers:
  virtual void on_realize();
  virtual bool on_configure_event(GdkEventConfigure* event);
  virtual bool on_expose_event(GdkEventExpose* event);
  virtual bool on_map_event(GdkEventAny* event);
  virtual bool on_unmap_event(GdkEventAny* event);
  virtual bool on_visibility_notify_event(GdkEventVisibility* event);
  bool on_button_press_event(GdkEventButton* event); 
  bool on_motion_notify_event(GdkEventMotion* event);
  bool on_scroll_event(GdkEventScroll* event);


  void updateViewingWindow(bool reset);
  void setZoom(float zoomval, float tcenter);

  // primary data source
  
  // state variables
  int majorTick_; 
  int minorTick_; 
  
  int lastX_; 
  int decayRange_; 
  int cutoffPos_; 
  bool viewLatest_; 
  float viewT1_, viewT2_, viewX1_, viewX2_; 
  float zoomLevel_; 
/*   std::vector<GLWavePoint_t> rates_;  */
/*   std::vector<GLWavePoint_t> ratesS1_;  */
  int s1fact_; 
  GLuint gpuProgGradient_; 
  std::list<WaveDraw*> pWaveRenderers_; 
};

#endif //RATETIMELINE_H
