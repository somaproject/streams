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


StreamsApp::StreamsApp(bool is_sync) : 
  waveWin_(&streamControl_),
  triggerWin_(),
  visProperty_(&streamVisSelSet_)
{

  buildActions(); 
  
  pMenuBar_ = pUIManager_->get_widget("/MenuBar");
  
  
  add(vBoxMain_); 
  vBoxMain_.pack_start(*pMenuBar_); 
  vBoxMain_.pack_start(hBoxMain_); 
  vBoxMain_.pack_start(hBoxStatus_); 
  hBoxMain_.pack_start(vBoxStreamStatus_); 
  hBoxMain_.pack_start(waveWin_); 

  waveWin_.set_size_request(700, 500); 
  triggerWin_.set_size_request(70, 500); 

  hBoxMain_.pack_start(triggerWin_); 
  
  // status componnets
  hBoxStatus_.pack_start(statusBar_); 
  
  show_all(); 

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
  streamSourcePtr_t ss = streamControl_.newSourceFactory(name); 
  
  // now we need to wrap this in a status object
  WaveStreamSourceStatus * wsss = new WaveStreamSourceStatus(ss); 
  pSourceStatusWidgets_.push_back(wsss); 
  
  vBoxStreamStatus_.pack_start(*wsss); 
  wsss->show_all(); 
  
  
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

//   set_title("StreamsApp");

//   // Get automatically redrawn if any of their children changed allocation.
//   set_reallocate_redraws(true);

//   add(m_VBox);
//   m_VBox.pack_start(hBox_); 
//   //
//   // StreamsApp scene.
//   //


//   hBox_.pack_start(vBoxControls_); 


//   hBox_.pack_start(waveWin_); 
//   hBox_.pack_start(triggerWin_); 

//   m_VBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK, 0);
//   m_ButtonQuit.signal_clicked().connect( sigc::mem_fun(*this, &StreamsApp::on_button_quit_clicked)); 

//   Glib::signal_idle().connect( sigc::mem_fun(*this, &StreamsApp::on_idle) );
//   timer_.start(); 
//   dtimer_.start(); 

//   show_all();
//   WaveStreamSource * wss = new WaveStreamSource(); 
//   wss_.push_back(wss); 
//   //Glib::signal_idle().connect( sigc::mem_fun(*this, &TSpikeWin::on_idle) );
//   for (int i = 0; i < 10; i++)
//     {

//       // now we create the 
//       WaveStreamVis * wsv = new WaveStreamVis(wss); 
//       wsv->setYOffset(float(i)*100); 
//       WaveStreamVisStatus * wsvs = new WaveStreamVisStatus(wsv); 

//       vBoxControls_.pack_start(*wsvs); 
//       wsvs->show(); 
//       wsvs->clickedSignal().connect(sigc::bind(sigc::mem_fun(*this, 
// 							     &StreamsApp::wsvsSelSetModify), i));

      
//       wsvs->show_all(); 

//       wsv_.push_back(wsv); 
//       wsvs_.push_back(wsvs); 
      
//       waveWin_.appendVis(wsv); 
//       triggerWin_.appendVis(wsv); 

//     }
  
//   wsv_[0]->enableTrigger(true); 
//   wsv_[0]->setTriggerValue(30.0); 
//   // now connect things up
//   wsv_[0]->setTriggerSource(wsv_[0]->getTriggerQueueView()); 
  
//   wsv_[0]->updateTriggersSignal().connect(sigc::mem_fun(*(wsv_[0]),
// 						     &WaveStreamVis::updateTriggers)); 


//   wss_[0]->generateFakeData(1);   
//   // setup fake data generation
  
//    conn_ = Glib::signal_timeout().connect(sigc::mem_fun(*this, 
//  						       &StreamsApp::appendNewData), 
//  					 30); 
// }

// bool StreamsApp::appendNewData()
// {
//   wss_[0]->generateFakeData(1); 
//   return true; 
  
// }

StreamsApp::~StreamsApp()
 {

 }

// void StreamsApp::on_button_quit_clicked()
// {
//   // we should quit

//   for (int i = 0; i < wsvs_.size(); i++)
//     {
//       int x, y; 
//       Gdk::Rectangle a = wsvs_[i]->get_allocation(); 

//       std::cout << "The position for " << i << " is " << a.get_y() << std::endl; 

//     }
// }

// bool StreamsApp::on_key_press_event(GdkEventKey* event)
// {
//   std::cout << "Keypress" << std::endl; 
//   switch (event->keyval)
//     {
//     case GDK_z:
//       break;
//     case GDK_Z:
//       break;
//     case GDK_Up:
//       std::cout << "Changing zoom" << std::endl; 
//       break;
//     case GDK_Down: 
//       std::cout << "Changing zoom down" << std::endl; 
//       break; 
//     default:
//       break;
//     }
//   return true; 

// }


// bool StreamsApp::on_idle()

// {

//   double seconds = timer_.elapsed();
  
//   if (seconds >= 1.00)
//     {
//       timer_.reset();
//     }

  
//   return true;
// }


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

// void spikeWaves(void)
// {

  

// } 

