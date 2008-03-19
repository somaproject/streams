#ifndef WAVEWIN_H
#define WAVEWIN_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <assert.h>

#include <stdlib.h>
#include <gtkmm.h>

#define GL_GLEXT_PROTOTYPES

#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "glconfig.h"
#include "glspikes.h"

#include "wavevisrenderer.h"
#include "streamvis.h"

#include "viscontrol.h"


class WaveWin : public Gtk::GL::DrawingArea
{
public:
  explicit WaveWin(pVisControl_t pvc); 

  virtual ~WaveWin();

  // Invalidate whole window.
  void inv(); 

  // Update window synchronously (fast).
  void update()
    { 
      get_window()->process_updates(false); 
    }

  void invalidate(); 
  void setCurrentTime(float time); 
  void setLiveView(bool val); 
protected:

  // signal handlers:
  virtual void on_realize();
  virtual bool on_configure_event(GdkEventConfigure* event);
  virtual bool on_expose_event(GdkEventExpose* event);
  virtual bool on_map_event(GdkEventAny* event);
  virtual bool on_unmap_event(GdkEventAny* event);
  virtual bool on_visibility_notify_event(GdkEventVisibility* event);
  virtual bool on_button_press_event(GdkEventButton* event); 
  virtual bool on_button_release_event(GdkEventButton* event); 
  bool on_motion_notify_event(GdkEventMotion* event);
  bool on_scroll_event(GdkEventScroll* event);


  void updateViewingWindow(bool reset);
  void setZoom(float zoomval, float tcenter);
  void renderTimeTicks(float T1, float T2);
  void setSelectionRegion(float t1, float t2); 

  void renderCurrentTimeCursor(); 

  // primary data source
  
  // state variables
  int lastX_; 
  int decayRange_; 
  int cutoffPos_; 
  bool viewLatest_; 
  float viewT1_, viewT2_, viewX1_, viewX2_; 
  float selT1_, selT2_; 

  float zoomLevel_; 
  int s1fact_; 

  bool liveView_; 
  pVisControl_t  pVisControl_; 

  float currentTime_; 
  
  // font-associated crap
  
  GLuint fontListBase_; 
  int fontHeight_; 

};

#endif //RATETIMELINE_H
