#ifndef STREAMSAPP_H
#define STREAMSAPP_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <set>
#include <assert.h>

#include <stdlib.h>
#include <gtkmm.h>

#define GL_GLEXT_PROTOTYPES
#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>


#include "streamrenderwin.h"
#include "sourcestate.h"
#include "pipelinemanager.h"
//#include "visproperty.h"

#include "streamrenderwin.h"

#include "pipelinemanagergui.h"
#include "propertypanemanager.h" 
#include "somaconfig.h"
#include "propwidgets/combobox.h"

namespace bf = boost::filesystem; 

class StreamsApp : public Gtk::Window
{
public:
  explicit StreamsApp(pSourceState_t ss, bf::path scratchdir); 
  virtual ~StreamsApp(); 

  pPipelineManager_t getPipelineManager(); // to enable external loading
  
protected:
  
  pPipelineManager_t pPipelineManager_; 
  pSourceState_t pSourceState_; 
  
  pSomaConfig_t pSomaConfig_; 

  Glib::RefPtr<Gtk::ActionGroup> pActionGroup_;
  
  Glib::RefPtr<Gtk::UIManager> pUIManager_;
  
  // widgets
  Gtk::VBox  vBoxMain_; 
  Gtk::HBox  hBoxMain_; 
  Gtk::HBox  hBoxStatus_; 
  Gtk::Label timeLabel_; 
  Gtk::VBox  vBoxStreamStatus_; 
  Gtk::Widget* pMenuBar_; 

  PropertyWidgets::ComboBox<ViewTypes> viewTypeCombo_; 

  //Gtk::ComboBoxText liveModeCombo_; 
  Gtk::ProgressBar statusProgressBar_; 
  Gtk::VBox vBoxPipelines_; 
  PropertyPaneManager ppm_; 
  PipelineManagerGUI pmg_; 


  void buildActions(); 
//   pStreamSource_t newStreamSource(std::string name, datasource_t); 
//   pStreamSource_t newStreamSource(std::string name); 
//   pStreamVis_t  newStreamVis(SourceStatus *, std::string name); 
//   pStreamVis_t  newStreamVis(pStreamSource_t, std::string name); 
//   pStreamVis_t newStreamVis1(std::string name); 
  
  void on_realize(); 
  
  // signal handlers:
  /*   void on_button_quit_clicked(); */
  /*   virtual bool on_idle(); */
  
  //StreamControl streamControl_; 
  
  //std::set<pStreamVis_t> streamVisSelSet_; 
  
  
  StreamRenderWin streamWin_; 
  //TriggerWin triggerWin_; 
  
  
  //sigc::connection conn_; 
  //waveStreamVisStatusSet_t wsvsSelSet_; 
//   void svSelSetModify(bool append, pStreamVis_t v); 
//   VisProperty visProperty_;
  
  void setTime(streamtime_t time); 
  Gtk::Label testLabel_; 
  
};

#endif // STREAMSAPP_H
