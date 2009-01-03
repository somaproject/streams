
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

// #include "glspikes.h"
// #include "wavewin.h"
#include "streamrenderwin.h"
#include "networkdatacache.h"
#include "testviscontrol.h"
#include "vis/sinevis.h"
#include "vis/wavevis.h"
#include "sources/noisewave.h"
#include "timer.h"
#include "sourcestate.h"
#include "somanetcodec.h"
// #include "wavestreamvis.h"
// #include "wavestreamvisstatus.h"
// #include "triggerwin.h"

// typedef std::set<int> waveStreamVisStatusSet_t; 



class StreamsTestApp : public Gtk::Window
{
public:
  explicit StreamsTestApp(bool is_sync, 
			  pSourceState_t  ss);
  virtual ~StreamsTestApp();

protected:
  // signal handlers:
  void on_button_quit_clicked();
  virtual bool on_idle();
  pSourceState_t sourceState_;
protected:
  // member widgets:
  Gtk::VBox m_VBox;
//   Gtk::HBox hBox_; 
//   Gtk::VBox vBoxControls_;
//   Glib::Timer timer_; 
//   Glib::Timer dtimer_; 
  Gtk::Button m_ButtonQuit;
  boost::shared_ptr<TestVisControl> pTestVisControl_; 
  StreamRenderWin renderWin_; 
  pSineVis_t pSineVis_; 
  std::vector<pWaveVis_t> pWaveVisSet_; 
  pTimer_t pTimer_; 
  std::vector<pNoiseWave_t> pNoiseWaveSet_; 
  //   TriggerWin triggerWin_; 
  pNetworkDataCache_t pNDC_; 
//   uint64_t lastSpikeTime_; 
//   void updateSpikePosFromAdj(); 
//   sigc::connection m_ConnectionIdle;
//   virtual bool on_key_press_event(GdkEventKey* event); 
//   bool appendNewData(); 

//   // data elements
//   std::vector<WaveStreamSource * > wss_; 
//   std::vector<WaveStreamVis * > wsv_; 
//   std::vector<WaveStreamVisStatus *> wsvs_; 

//   sigc::connection conn_; 
//   waveStreamVisStatusSet_t wsvsSelSet_; 
//   void wsvsSelSetModify(bool append, int num); 

};

StreamsTestApp::StreamsTestApp(bool is_sync, pSourceState_t ss)
  :
  m_VBox(false, 0), 
//     hBox_(false, 0), 
//     vBoxControls_(false, 0), 
  m_ButtonQuit("Quit"), 
  pTestVisControl_(new TestVisControl),
  renderWin_(pTestVisControl_), 
  sourceState_(ss) 
  //pSineVis_(new SineVis("vis")),
{
  for (int i = 0; i < 4; i++) { 
    pNoiseWaveSet_.push_back(pNoiseWave_t(new NoiseWave("noiseSource", sourceState_->timer))); 
    pWaveVisSet_.push_back(pWaveVis_t(new WaveVis("waveVisTest")));
  }
  
  set_title("StreamsTestApp");
  
  // Get automatically redrawn if any of their children changed allocation.
  set_reallocate_redraws(true);
  
  add(m_VBox);
//   m_VBox.pack_start(hBox_); 
  //
  // StreamsTestApp scene.
  //
//   waveWin_.set_size_request(1000, 400);
//   triggerWin_.set_size_request(100, 400);


//   hBox_.pack_start(vBoxControls_); 


//   hBox_.pack_start(waveWin_); 
//   hBox_.pack_start(triggerWin_); 
  m_VBox.pack_start(renderWin_); 
  m_VBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK, 0);
  m_ButtonQuit.signal_clicked().connect( sigc::mem_fun(*this, &StreamsTestApp::on_button_quit_clicked)); 

//   Glib::signal_idle().connect( sigc::mem_fun(*this, &StreamsTestApp::on_idle) );
//   timer_.start(); 
//   dtimer_.start(); 
  //pTestVisControl_->addVis(pSineVis_); 
  for (int i = 0; i < 4; i++) {
    pTestVisControl_->addVis(pWaveVisSet_[i]); 
    
    core::SourcePad<WaveBuffer_t> * ps1 = 
      pNoiseWaveSet_[i]->getSource<WaveBuffer_t>("default");
    core::SinkPad<WaveBuffer_t> * ps2 = 
      pWaveVisSet_[i]->getSink<WaveBuffer_t>("default"); 
    ps1->connect(ps2);

  }

  pNoiseWaveSet_[0]->setNoiseClass(NoiseWave::WhiteNoise); 
  pNoiseWaveSet_[1]->setNoiseClass(NoiseWave::NoisySine); 
  pNoiseWaveSet_[2]->setNoiseClass(NoiseWave::SquareWave); 
  pNoiseWaveSet_[3]->setNoiseClass(NoiseWave::BiModal); 
  
    show_all();
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
// 							     &StreamsTestApp::wsvsSelSetModify), i));

      
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
//  						       &StreamsTestApp::appendNewData), 
// 30); 
}

// bool StreamsTestApp::appendNewData()
// {
// //   wss_[0]->generateFakeData(1); 
// //   return true; 
  
// }

StreamsTestApp::~StreamsTestApp()
{
}

void StreamsTestApp::on_button_quit_clicked()
{
  // we should quit

}

// bool StreamsTestApp::on_key_press_event(GdkEventKey* event)
// {
// //   std::cout << "Keypress" << std::endl; 
// //   switch (event->keyval)
// //     {
// //     case GDK_z:
// //       break;
// //     case GDK_Z:
// //       break;
// //     case GDK_Up:
// //       std::cout << "Changing zoom" << std::endl; 
// //       break;
// //     case GDK_Down: 
// //       std::cout << "Changing zoom down" << std::endl; 
// //       break; 
// //     default:
// //       break;
// //     }
// //   return true; 

// }


bool StreamsTestApp::on_idle()

{

//   double seconds = timer_.elapsed();
  
//   if (seconds >= 1.00)
//     {
//       timer_.reset();
//     }

  
//   return true;
}


// void StreamsTestApp::wsvsSelSetModify(bool append, int num)
// {
//   std::cout << "append = " << append << " num=" << num << std::endl; 
//   waveStreamVisStatusSet_t::iterator i = wsvsSelSet_.find(num); 
//   if (append) {
//     if (i == wsvsSelSet_.end()) {
//       // it's not in the set; add
//       wsvsSelSet_.insert(num); 
//     } else {
//       wsvsSelSet_.erase(i); 
//     }
//   } else {
//     // we're not appending
//     if (i == wsvsSelSet_.end()) {
//       // not currently in 
//       wsvsSelSet_.clear(); 
//       wsvsSelSet_.insert(num); 
//     } else {
//       wsvsSelSet_.clear(); 

//     }
//   }

//   // update
//   for (int j = 0; j < wsvs_.size(); j++)
//     {
//       wsvs_[j]->setSelected(false); 
//     }

//   for (i = wsvsSelSet_.begin(); i != wsvsSelSet_.end(); i++)
//     {
//       wsvs_[*i]->setSelected(true); 
//     }
  
  
// }

// void spikeWaves(void)
// {

  

// } 

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  //
  // Init gtkglextmm.
  //

  Gtk::GL::init(argc, argv);

  pSourceState_t pss(new SourceState()); 
  pss->timer = Timer::createDummyTimer(); 
  
  pNetworkInterface_t ni(new Network("127.0.0.1")); 
  
  pSomaNetCodec_t snc(new SomaNetCodec(ni)); 
  
  pss->netdatacache = 
    pNetworkDataCache_t(new NetworkDataCache( snc, pss->timer)); 
  
  
  StreamsTestApp vis(true, pss);

  kit.run(vis);

  return 0;
}
