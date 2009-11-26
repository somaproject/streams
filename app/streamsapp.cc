/* Conversion to gtkglextmm by Naofumi Yasufuku */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <set>
#include <assert.h>
#include <boost/format.hpp>
#include <stdlib.h>
#include <gtkmm.h>

#define GL_GLEXT_PROTOTYPES


#include <gtkglmm.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "glspikes.h"

#include "streamsapp.h"


typedef std::set<int> waveStreamVisStatusSet_t; 


StreamsApp::StreamsApp(pSourceState_t pss, bf::path scratchdir) : 
  pSourceState_(pss), 
  pPipelineManager_(new PipelineManager(scratchdir)), 
  pSomaConfig_(new SomaConfig), 
  streamWin_(pPipelineManager_), 
  trackLive_("Track Live"), 
  ppm_(*this, pSomaConfig_), 
  pmg_(pPipelineManager_, pSomaConfig_, vBoxPipelines_, ppm_), 
  testLabel_("Hello world"),
  timeLabel_("SomaTime", Gtk::ALIGN_LEFT)
  //triggerWin_(pPipelineManager_)  
{
  
  pPipelineManager_->pSourceState_ = pss; 
  
  buildActions(); 
  
  pMenuBar_ = pUIManager_->get_widget("/MenuBar");
  
  
  add(vBoxMain_); 
  vBoxMain_.pack_start(*pMenuBar_); 
  vBoxMain_.pack_start(hBoxMain_, true, true); 
  vBoxMain_.pack_start(hBoxStatus_); 
  hBoxMain_.pack_start(vBoxPipelines_); 
  //vBoxPipelines_.pack_start(testLabel_); 
  hBoxMain_.pack_start(streamWin_, true, true); 
  trackLive_.addProperty(&(streamWin_.trackLive)); 
  //hBoxMain_.pack_start(triggerWin_, true, true); 
  streamWin_.set_size_request(1000, 700); 
  //triggerWin_.set_size_request(150, 1000); 
  // status componnets

  hBoxStatus_.pack_start(timeLabel_); 
  timeLabel_.set_size_request(100, -1); 
  hBoxStatus_.pack_start(trackLive_); 
  hBoxStatus_.pack_start(liveModeCombo_); 
  liveModeCombo_.append_text("manual"); 
  liveModeCombo_.append_text("stripchart"); 
  liveModeCombo_.append_text("overwrite"); 
  

  show_all(); 

  pSourceState_->timer->streamTimeSignal().connect(sigc::mem_fun(*this,
									&StreamsApp::setTime)); 
  add_events (Gdk::ALL_EVENTS_MASK); 
//   signal_realize().connect(sigc::mem_fun(*this, 
// 					 &StreamsApp::on_realize_event)); 
  
  
}

void StreamsApp::on_realize()
{
  Gtk::Widget::on_realize(); 

//   for (int i = 0; i < 2; i++) {
//     pStreamSource_t ssp(newStreamSource("wave", i));
//     pStreamVis_t svp = newStreamVis(ssp, "wave"); 
//   }
  

}

void StreamsApp::buildActions()
{
  
  pActionGroup_ = Gtk::ActionGroup::create(); 
  pActionGroup_->add( Gtk::Action::create("MenuFile", "_File") );
//   pActionGroup_->add( Gtk::Action::create("New WaveSource", "New WaveSource"),
// 		      sigc::bind(
// 				 sigc::mem_fun(*this, &StreamsApp::newStreamSource), 
// 				 "wave")); 

//   pActionGroup_->add( Gtk::Action::create("New WaveVis", "New WaveVis"),
//  		      sigc::bindst(
//  				 sigc::mem_fun(*this, &StreamsApp::newStreamVis1), 
// 				 "wave")); 


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

// pStreamSource_t StreamsApp::newStreamSource(std::string name)
// {
//   return newStreamSource(name, 0); 
// }

// pStreamSource_t StreamsApp::newStreamSource(std::string name, datasource_t ds)
// {
  
//   // first create the stream object
//   pStreamSource_t ss = streamControl_.newSourceFactory(name, ds); 
  
//   // now we need to wrap this in a status object
//   WaveStreamSourceStatus * wsss = new WaveStreamSourceStatus(ss); 
//   pSourceStatusWidgets_.push_back(wsss); 
//   Gtk::HPaned * pHPane = new Gtk::HPaned(); 
//   pHPane->pack1(*wsss); 
//   vBoxStreamStatus_.pack_start(*pHPane); 
//   pSignalPairWidgets_.push_back(pHPane); 
  
//   pHPane->show_all(); 
  
//   return ss; 
// }

// pStreamVis_t StreamsApp::newStreamVis1(std::string name)
// {
//   return newStreamVis(pSourceStatusWidgets_.front(), name); 


// }

// pStreamVis_t StreamsApp::newStreamVis(SourceStatus* src, std::string name)
// {
  
//   // first create the stream object
//   pStreamVis_t sv = streamControl_.newVisFactory(src->getSourcePtr(), name); 
  
//   // now we need to wrap this in a status object
//   WaveStreamVisStatus * wvss = new WaveStreamVisStatus(sv); 
//   wvss->clickedSignal().connect(sigc::bind(sigc::mem_fun(*this, 
// 							 &StreamsApp::svSelSetModify), sv)); 


//   pVisStatusWidgets_.push_back(wvss); 
  
//   vBoxStreamStatus_.pack_start(*wvss); 
//   wvss->show_all(); 
  
//   // how do we connect the vis to the waves? 
//   sv->invMainWaveSignal().connect(sigc::mem_fun(waveWin_, &WaveWin::invalidate)); 
//   return sv; 

// }

// pStreamVis_t StreamsApp::newStreamVis(pStreamSource_t src, std::string name)
// {
  
//   // first create the stream object
//   pStreamVis_t sv = streamControl_.newVisFactory(src, name); 
  
//   // now we need to wrap this in a status object
//   WaveStreamVisStatus * wvss = new WaveStreamVisStatus(sv); 
//   wvss->clickedSignal().connect(sigc::bind(sigc::mem_fun(*this, 
// 							 &StreamsApp::svSelSetModify), sv)); 

//   pVisStatusWidgets_.push_back(wvss); 
  
//   vBoxStreamStatus_.pack_start(*wvss); 
//   wvss->show_all(); 
  
//   // how do we connect the vis to the waves? 
//   sv->invMainWaveSignal().connect(sigc::mem_fun(waveWin_, &WaveWin::invalidate)); 
//   return sv; 

// }


StreamsApp::~StreamsApp()
 {

 }

// void StreamsApp::svSelSetModify(bool append, pStreamVis_t v)
// {

//   std::set<pStreamVis_t>::iterator i = streamVisSelSet_.find(v); 

//   if (append) {
//     if (i == streamVisSelSet_.end()) {
//       // it's not in the set; add
//       streamVisSelSet_.insert(v); 
//     } else {
//       streamVisSelSet_.erase(i); 
//     }
//   } else {
//     // we're not appending
//     if (i == streamVisSelSet_.end()) {
//       // not currently in 
//       streamVisSelSet_.clear(); 
//       streamVisSelSet_.insert(v); 
//     } else {
//       streamVisSelSet_.clear(); 

//     }
//   }

//   // update
//   for (int j = 0; j < pVisStatusWidgets_.size(); j++)
//     {
//       i = streamVisSelSet_.find(pVisStatusWidgets_[j]->getVisPtr()); 
//       if (i != streamVisSelSet_.end()) {
// 	pVisStatusWidgets_[j]->setSelected(true); 
//       } else {
// 	pVisStatusWidgets_[j]->setSelected(false); 
//       }
//     }


//   visProperty_.show();
//   visProperty_.updateSet(); 
// }


void StreamsApp::setTime(streamtime_t time)
{
  timeLabel_.set_text(boost::str(boost::format("Soma Time: %d") % time)); 

  streamWin_.setCurrentTime(time); 
}

pPipelineManager_t StreamsApp::getPipelineManager()
{
  return pPipelineManager_; 
}

