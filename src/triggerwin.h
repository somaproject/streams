#ifndef TRIGGERWIN_H
#define TRIGGERWIN_H

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

#include "wavestreamvis.h" 

class TriggerWin : public Gtk::GL::DrawingArea
{
public:
  explicit TriggerWin(); 

  virtual ~TriggerWin();

  // Update window synchronously (fast).
  void update()
    { 
      get_window()->process_updates(false); 
    }

  void invalidate(); 
  void appendVis(WaveStreamVis * wv); 

protected:

  // signal handlers:
  virtual void on_realize();
  virtual bool on_configure_event(GdkEventConfigure* event);
  virtual bool on_expose_event(GdkEventExpose* event);
  virtual bool on_map_event(GdkEventAny* event);
  virtual bool on_unmap_event(GdkEventAny* event);
  virtual bool on_visibility_notify_event(GdkEventVisibility* event);

  void updateViewingWindow(bool reset);
  void renderTimeTicks(float T1, float T2);

  // primary data source
  
  // state variables
  int lastX_; 

  int decayRange_; 
  float viewT1_, viewT2_, viewX1_, viewX2_; 

  std::list<WaveStreamVis*> pWaveVis_; 
};

#endif //TRIGGERWIN_H
