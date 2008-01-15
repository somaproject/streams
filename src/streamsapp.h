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
#include "wavestreamsource.h"
#include "wavestreamvis.h"
#include "wavestreamvisstatus.h"
#include "triggerwin.h"
#include "streamcontrol.h" 
#include "sourcestatus.h"
#include "visproperty.h"


class StreamsApp : public Gtk::Window
{
public:
  explicit StreamsApp(NetworkInterface * ni); 
  virtual ~StreamsApp(); 

protected:

   Glib::RefPtr<Gtk::ActionGroup> pActionGroup_;

   Glib::RefPtr<Gtk::UIManager> pUIManager_;
   
   // widgets
   Gtk::VBox  vBoxMain_; 
   Gtk::HBox  hBoxMain_; 
   Gtk::HBox  hBoxStatus_; 
   Gtk::VBox  vBoxStreamStatus_; 
   Gtk::Widget* pMenuBar_; 
   Gtk::Statusbar statusBar_; 
   Gtk::ToggleButton liveButton_; 
   Gtk::ProgressBar statusProgressBar_; 
   // stream source widgets
   std::vector<SourceStatus * >  pSourceStatusWidgets_; 
   std::vector<VisStatus *  >  pVisStatusWidgets_; 
   std::vector<Gtk::HPaned *> pSignalPairWidgets_; 
   
   
  void buildActions(); 
  pStreamSource_t newStreamSource(std::string name, datasource_t); 
  pStreamSource_t newStreamSource(std::string name); 
  pStreamVis_t  newStreamVis(SourceStatus *, std::string name); 
  pStreamVis_t  newStreamVis(pStreamSource_t, std::string name); 
  pStreamVis_t newStreamVis1(std::string name); 
  
  void on_realize(); 

  // signal handlers:
  /*   void on_button_quit_clicked(); */
  /*   virtual bool on_idle(); */
  
  StreamControl streamControl_; 
  
  std::set<pStreamVis_t> streamVisSelSet_; 
  

   WaveWin waveWin_; 
   TriggerWin triggerWin_; 


  //sigc::connection conn_; 
  //waveStreamVisStatusSet_t wsvsSelSet_; 
   void svSelSetModify(bool append, pStreamVis_t v); 
   VisProperty visProperty_;

   void setTime(float time); 

};

