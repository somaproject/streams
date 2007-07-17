/* Conversion to gtkglextmm by Naofumi Yasufuku */

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

#include "glspikes.h"
#include "wavewin.h"
#include "wavestreamsource.h"
#include "wavestreamsourcestatus.h"

#include "wavestreamvis.h"
#include "wavestreamvisstatus.h"

#include "triggerwin.h"

#include "streamsapp.h"

typedef std::set<int> waveStreamVisStatusSet_t; 


StreamsApp::StreamsApp(NetworkInterface * ni) : 
  liveButton_("Live View"), 
  streamControl_(ni), 
  waveWin_(&streamControl_),
  triggerWin_(),
  visProperty_(&streamVisSelSet_)
  
{

  buildActions(); 
  
  pMenuBar_ = pUIManager_->get_widget("/MenuBar");
  
  
  add(vBoxMain_); 
  vBoxMain_.pack_start(*pMenuBar_); 
  vBoxMain_.pack_start(hBoxMain_, true, true); 
  vBoxMain_.pack_start(hBoxStatus_); 
  hBoxMain_.pack_start(vBoxStreamStatus_); 
  
  hBoxMain_.pack_start(waveWin_, true, true); 
  
  hBoxMain_.pack_start(triggerWin_, true, true); 
  waveWin_.set_size_request(1300, 1000); 
  triggerWin_.set_size_request(150, 1000); 
  // status componnets
  hBoxStatus_.pack_start(statusProgressBar_); 
  statusProgressBar_.set_size_request(100, 0); 

  hBoxStatus_.pack_start(statusBar_); 
  hBoxStatus_.pack_start(liveButton_); 

  show_all(); 

  streamControl_.timeSignal().connect(sigc::mem_fun(*this,
						   &StreamsApp::setTime)); 


}
void StreamsApp::buildActions()
{
  
  pActionGroup_ = Gtk::ActionGroup::create(); 
  pActionGroup_->add( Gtk::Action::create("MenuFile", "_File") );
  pActionGroup_->add( Gtk::Action::create("New WaveSource", "New WaveSource"),
		      sigc::bind(
				 sigc::mem_fun(*this, &StreamsApp::newStreamSource), 
				 "wave")); 

  pActionGroup_->add( Gtk::Action::create("New WaveVis", "New WaveVis"),
 		      sigc::bind(
 				 sigc::mem_fun(*this, &StreamsApp::newStreamVis1), 
				 "wave")); 


  pUIManager_ = Gtk::UIManager::create();
  pUIManager_->insert_action_group(pActionGroup_);
  add_accel_group(pUIManager_->get_accel_group());

  Glib::ustring ui_info =
    "<ui>"
    "  <menubar name='MenuBar'>"
    "    <menu action='MenuFile'>"
    "      <menuitem action='New WaveSource'/>"
    "      <menuitem action='New WaveVis'/>" 
    "    </menu>"
    "  </menubar>"
    "  <toolbar  name='ToolBar'>"
    "  </toolbar>"
    "</ui>";

  pUIManager_->add_ui_from_string(ui_info);

}

void StreamsApp::newStreamSource(std::string name)
{
  
  std::cout << "Creating a new source " << name << std::endl; 
  
  // first create the stream object
  streamSourcePtr_t ss = streamControl_.newSourceFactory(name, 1, WAVE); 
  
  // now we need to wrap this in a status object
  WaveStreamSourceStatus * wsss = new WaveStreamSourceStatus(ss); 
  pSourceStatusWidgets_.push_back(wsss); 
  Gtk::HPaned * pHPane = new Gtk::HPaned(); 
  pHPane->pack1(*wsss); 
  vBoxStreamStatus_.pack_start(*pHPane); 
  pSignalPairWidgets_.push_back(pHPane); 
  
  pHPane->show_all(); 
  
  
}

void StreamsApp::newStreamVis1(std::string name)
{
  newStreamVis(pSourceStatusWidgets_.front(), name); 


}

void StreamsApp::newStreamVis(SourceStatus* src, std::string name)
{
  
  std::cout << "Creating a new vis" << name << std::endl; 
  
  // first create the stream object
  streamVisPtr_t sv = streamControl_.newVisFactory(src->getSourcePtr(), name); 
  
  // now we need to wrap this in a status object
  WaveStreamVisStatus * wvss = new WaveStreamVisStatus(sv); 
  wvss->clickedSignal().connect(sigc::bind(sigc::mem_fun(*this, 
							 &StreamsApp::svSelSetModify), sv)); 


  pVisStatusWidgets_.push_back(wvss); 
  
  vBoxStreamStatus_.pack_start(*wvss); 
  wvss->show_all(); 
  
  // how do we connect the vis to the waves? 
  sv->invMainWaveSignal().connect(sigc::mem_fun(waveWin_, &WaveWin::invalidate)); 

}

StreamsApp::~StreamsApp()
 {

 }

void StreamsApp::svSelSetModify(bool append, streamVisPtr_t v)
{
  std::cout << "appending" << std::endl; 

  std::set<streamVisPtr_t>::iterator i = streamVisSelSet_.find(v); 

  if (append) {
    if (i == streamVisSelSet_.end()) {
      // it's not in the set; add
      streamVisSelSet_.insert(v); 
    } else {
      streamVisSelSet_.erase(i); 
    }
  } else {
    // we're not appending
    if (i == streamVisSelSet_.end()) {
      // not currently in 
      streamVisSelSet_.clear(); 
      streamVisSelSet_.insert(v); 
    } else {
      streamVisSelSet_.clear(); 

    }
  }

  // update
  for (int j = 0; j < pVisStatusWidgets_.size(); j++)
    {
      i = streamVisSelSet_.find(pVisStatusWidgets_[j]->getVisPtr()); 
      if (i != streamVisSelSet_.end()) {
	pVisStatusWidgets_[j]->setSelected(true); 
      } else {
	pVisStatusWidgets_[j]->setSelected(false); 
      }
    }


  visProperty_.show();
  visProperty_.updateSet(); 
}


void StreamsApp::setTime(float time)
{
  
  waveWin_.setCurrentTime(time);  
  
}
