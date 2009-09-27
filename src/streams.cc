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

typedef std::set<int> waveStreamVisStatusSet_t; 


class WavesApp : public Gtk::Window
{
public:
  explicit WavesApp(bool is_sync = true);
  virtual ~WavesApp();

protected:
  // signal handlers:
  void on_button_quit_clicked();
  virtual bool on_idle();

protected:
  // member widgets:
  Gtk::VBox m_VBox;
  Gtk::HBox hBox_; 
  Gtk::VBox vBoxControls_;
  Glib::Timer timer_; 
  Glib::Timer dtimer_; 
  Gtk::Button m_ButtonQuit;
  WaveWin waveWin_; 
  TriggerWin triggerWin_; 

  uint64_t lastSpikeTime_; 
  void updateSpikePosFromAdj(); 
  sigc::connection m_ConnectionIdle;
  virtual bool on_key_press_event(GdkEventKey* event); 
  bool appendNewData(); 

  // data elements
  std::vector<WaveStreamSource * > wss_; 
  std::vector<WaveStreamVis * > wsv_; 
  std::vector<WaveStreamVisStatus *> wsvs_; 

  sigc::connection conn_; 
  waveStreamVisStatusSet_t wsvsSelSet_; 
  void wsvsSelSetModify(bool append, int num); 

};

WavesApp::WavesApp(bool is_sync)
  : m_VBox(false, 0), 
    hBox_(false, 0), 
    vBoxControls_(false, 0), 
    m_ButtonQuit("Quit"), 
    waveWin_(),
    triggerWin_(), 
    lastSpikeTime_(0)
{

  set_title("WavesApp");

  // Get automatically redrawn if any of their children changed allocation.
  set_reallocate_redraws(true);

  add(m_VBox);
  m_VBox.pack_start(hBox_); 
  //
  // WavesApp scene.
  //
//   waveWin_.set_size_request(1000, 400);
//   triggerWin_.set_size_request(100, 400);


  hBox_.pack_start(vBoxControls_); 


  hBox_.pack_start(waveWin_); 
  hBox_.pack_start(triggerWin_); 

  m_VBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK, 0);
  m_ButtonQuit.signal_clicked().connect( sigc::mem_fun(*this, &WavesApp::on_button_quit_clicked)); 

  Glib::signal_idle().connect( sigc::mem_fun(*this, &WavesApp::on_idle) );
  timer_.start(); 
  dtimer_.start(); 

  show_all();
  WaveStreamSource * wss = new WaveStreamSource(); 
  wss_.push_back(wss); 
  //Glib::signal_idle().connect( sigc::mem_fun(*this, &TSpikeWin::on_idle) );
  for (int i = 0; i < 10; i++)
    {

      // now we create the 
      WaveStreamVis * wsv = new WaveStreamVis(wss); 
      wsv->setYOffset(float(i)*100); 
      WaveStreamVisStatus * wsvs = new WaveStreamVisStatus(wsv); 

      vBoxControls_.pack_start(*wsvs); 
      wsvs->show(); 
      wsvs->clickedSignal().connect(sigc::bind(sigc::mem_fun(*this, 
							     &WavesApp::wsvsSelSetModify), i));

      
      wsvs->show_all(); 

      wsv_.push_back(wsv); 
      wsvs_.push_back(wsvs); 
      
      waveWin_.appendVis(wsv); 
      triggerWin_.appendVis(wsv); 

    }
  
  wsv_[0]->enableTrigger(true); 
  wsv_[0]->setTriggerValue(30.0); 
  // now connect things up
  wsv_[0]->setTriggerSource(wsv_[0]->getTriggerQueueView()); 
  
  wsv_[0]->updateTriggersSignal().connect(sigc::mem_fun(*(wsv_[0]),
						     &WaveStreamVis::updateTriggers)); 


  wss_[0]->generateFakeData(1);   
  // setup fake data generation
  
   conn_ = Glib::signal_timeout().connect(sigc::mem_fun(*this, 
 						       &WavesApp::appendNewData), 
 					 30); 
}

bool WavesApp::appendNewData()
{
  wss_[0]->generateFakeData(1); 
  return true; 
  
}

WavesApp::~WavesApp()
{
}

void WavesApp::on_button_quit_clicked()
{
  // we should quit

  for (int i = 0; i < wsvs_.size(); i++)
    {
      int x, y; 
      Gdk::Rectangle a = wsvs_[i]->get_allocation(); 

      std::cout << "The position for " << i << " is " << a.get_y() << std::endl; 

    }
}

bool WavesApp::on_key_press_event(GdkEventKey* event)
{

  switch (event->keyval)
    {
    case GDK_z:
      break;
    case GDK_Z:
      break;
    case GDK_Up:
      std::cout << "Changing zoom" << std::endl; 
      break;
    case GDK_Down: 
      std::cout << "Changing zoom down" << std::endl; 
      break; 
    default:
      break;
    }
  return true; 

}


bool WavesApp::on_idle()

{

  double seconds = timer_.elapsed();
  
  if (seconds >= 1.00)
    {
      timer_.reset();
    }

  
  return true;
}


void WavesApp::wsvsSelSetModify(bool append, int num)
{
  std::cout << "append = " << append << " num=" << num << std::endl; 
  waveStreamVisStatusSet_t::iterator i = wsvsSelSet_.find(num); 
  if (append) {
    if (i == wsvsSelSet_.end()) {
      // it's not in the set; add
      wsvsSelSet_.insert(num); 
    } else {
      wsvsSelSet_.erase(i); 
    }
  } else {
    // we're not appending
    if (i == wsvsSelSet_.end()) {
      // not currently in 
      wsvsSelSet_.clear(); 
      wsvsSelSet_.insert(num); 
    } else {
      wsvsSelSet_.clear(); 

    }
  }

  // update
  for (int j = 0; j < wsvs_.size(); j++)
    {
      wsvs_[j]->setSelected(false); 
    }

  for (i = wsvsSelSet_.begin(); i != wsvsSelSet_.end(); i++)
    {
      wsvs_[*i]->setSelected(true); 
    }
  
  
}

void spikeWaves(void)
{

  

} 

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  //
  // Init gtkglextmm.
  //

  Gtk::GL::init(argc, argv);

  
  WavesApp vis(true);

  kit.run(vis);

  return 0;
}
