#ifndef STREAMRENDERWIN_H
#define STREAMRENDERWIN_H

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
#include <elements/property.h>

#include "viscontrol.h"

#include <cairogltext/glstring.h>

enum ViewTypes {OVERWRITE, STRIPCHART, MANUAL}; 


class StreamRenderWin : public Gtk::GL::DrawingArea
{
  /* 
     
     StreamRenderWin has three modes: 
     MANUAL / STRIPCHART : use viewT1_, viewT2_; 
     OVERWRITE : render from overwriteT1_

     define the time window we are looking at in streamtimes. 
     
     However, we are actually always plotting openGL starting with 
     0. 

   */ 
public:
  explicit StreamRenderWin(pVisControl_t pvc); 
  
  virtual ~StreamRenderWin();
  
  // Invalidate whole window.
  void inv(); 
  
  // Update window synchronously (fast).
  void update()
  { 
    get_window()->process_updates(false); 
  }
  //elements::Property<bool> trackLive;
  elements::Property<ViewTypes>  viewType; 
  void invalidate(); 
  void invalidate2(); 
  void setCurrentTime(streamtime_t time); 
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
  bool invalidateCallback(); 

  // primary data source
  
  // state variables
  int lastX_; 
  int decayRange_; 
  int cutoffPos_; 

  float viewT1_, viewT2_, viewX1_, viewX2_; 
  float viewTimeWidth_; 

  float selT1_, selT2_; 
  
  float zoomLevel_; 
  int s1fact_; 
  
  pVisControl_t  pVisControl_; 
  
  float currentTime_; 
  
  // font-associated crap
  
  GLuint fontListBase_; 
  int fontHeight_; 
  
  GLString timeGLstring_; 

  renderRegion(float t1, float t2); 

};

#endif //STREAMRENDERWIN_H
