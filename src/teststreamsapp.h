#include <iostream>
#include <cstdlib>
#include <cmath>
#include <set>
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

#include "glspikes.h"
#include "wavewin.h"
#include "wavevis.h"
//#include "wavestreamvisstatus.h"
#include "triggerwin.h"
#include "sourcecontrol.h" 
#include "viscontrol.h" 
//#include "sourcestatus.h"


class TestStreamsApp : public Gtk::Window
{
public:
  explicit TestStreamsApp(pSourceControl_t psc, pVisControl_t pvc); 
  virtual ~TestStreamsApp(); 
  
protected:
  
  // widgets
  Gtk::HBox  hBoxMain_; 

  void on_realize(); 
  
  // signal handlers:
  /*   void on_button_quit_clicked(); */
  /*   virtual bool on_idle(); */
  
  pVisControl_t pVisControl_; 
  pSourceControl_t pSourceControl_; 
  
  //std::set<pStreamVis_t> streamVisSelSet_; 
  
  
  WaveWin waveWin_; 
  TriggerWin triggerWin_; 
  
  
  //sigc::connection conn_; 
  //waveStreamVisStatusSet_t wsvsSelSet_; 
//   void svSelSetModify(bool append, pStreamVis_t v); 
//   VisProperty visProperty_;
  
  void setTime(float time); 
  
};

