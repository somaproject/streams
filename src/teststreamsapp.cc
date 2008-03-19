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
//#include "wavestreamsource.h"
//#include "wavestreamsourcestatus.h"

#include "wavevis.h"
//#include "wavestreamvisstatus.h"

#include "triggerwin.h"

#include "teststreamsapp.h"



TestStreamsApp::TestStreamsApp(pSourceControl_t psc, pVisControl_t pvc) :
  pSourceControl_(psc), 
  pVisControl_(pvc), 
  waveWin_(pvc),
  triggerWin_(pvc)  
{

  
  add(hBoxMain_); 
  hBoxMain_.pack_start(waveWin_, true, true); 
  hBoxMain_.pack_start(triggerWin_, true, true); 
  waveWin_.set_size_request(1300, 1000); 
  triggerWin_.set_size_request(150, 1000); 

  show_all(); 

//   streamControl_.timeSignal().connect(sigc::mem_fun(*this,
// 						   &TestStreamsApp::setTime)); 
  add_events (Gdk::ALL_EVENTS_MASK); 
//   signal_realize().connect(sigc::mem_fun(*this, 
// 					 &TestStreamsApp::on_realize_event)); 


}

void TestStreamsApp::on_realize()
{
  Gtk::Widget::on_realize(); 

//    for (int i = 0; i < 2; i++) {
//     pStreamSource_t ssp(newStreamSource("wave", i));
//     pStreamVis_t svp = newStreamVis(ssp, "wave"); 
//   }
  

}


// pStreamSource_t TestStreamsApp::newStreamSource(std::string name)
// {
//   return newStreamSource(name, 0); 
// }

// pStreamSource_t TestStreamsApp::newStreamSource(std::string name, datasource_t ds)
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

// pStreamVis_t TestStreamsApp::newStreamVis1(std::string name)
// {
//   return newStreamVis(pSourceStatusWidgets_.front(), name); 


// }

// pStreamVis_t TestStreamsApp::newStreamVis(SourceStatus* src, std::string name)
// {
  
//   // first create the stream object
//   pStreamVis_t sv = streamControl_.newVisFactory(src->getSourcePtr(), name); 
  
//   // now we need to wrap this in a status object
//   WaveStreamVisStatus * wvss = new WaveStreamVisStatus(sv); 
//   wvss->clickedSignal().connect(sigc::bind(sigc::mem_fun(*this, 
// 							 &TestStreamsApp::svSelSetModify), sv)); 


//   pVisStatusWidgets_.push_back(wvss); 
  
//   vBoxStreamStatus_.pack_start(*wvss); 
//   wvss->show_all(); 
  
//   // how do we connect the vis to the waves? 
//   sv->invMainWaveSignal().connect(sigc::mem_fun(waveWin_, &WaveWin::invalidate)); 
//   return sv; 

// }

// pStreamVis_t TestStreamsApp::newStreamVis(pStreamSource_t src, std::string name)
// {
  
//   // first create the stream object
//   pStreamVis_t sv = streamControl_.newVisFactory(src, name); 
  
//   // now we need to wrap this in a status object
//   WaveStreamVisStatus * wvss = new WaveStreamVisStatus(sv); 
//   wvss->clickedSignal().connect(sigc::bind(sigc::mem_fun(*this, 
// 							 &TestStreamsApp::svSelSetModify), sv)); 

//   pVisStatusWidgets_.push_back(wvss); 
  
//   vBoxStreamStatus_.pack_start(*wvss); 
//   wvss->show_all(); 
  
//   // how do we connect the vis to the waves? 
//   sv->invMainWaveSignal().connect(sigc::mem_fun(waveWin_, &WaveWin::invalidate)); 
//   return sv; 

// }


TestStreamsApp::~TestStreamsApp()
 {

 }

// void TestStreamsApp::svSelSetModify(bool append, pStreamVis_t v)
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


void TestStreamsApp::setTime(float time)
{
  
  //waveWin_.setCurrentTime(time);  
  
}
