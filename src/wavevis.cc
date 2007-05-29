/* Conversion to gtkglextmm by Naofumi Yasufuku */

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

#include "glspikes.h"
#include "spikewaveview.h"
#include "ttreader.h"
SpikeWaveList_t swl1; 
SpikeWaveList_t swl2; 
SpikeWaveList_t swl3; 
SpikeWaveList_t swl4; 

class Vis : public Gtk::Window
{
public:
  explicit Vis(bool is_sync = true);
  virtual ~Vis();

protected:
  // signal handlers:
  void on_button_quit_clicked();
  virtual bool on_idle();

protected:
  // member widgets:
  Gtk::VBox m_VBox;
  Gtk::Table table_; 
  SpikeWaveView spikeWaveView1_; 
  SpikeWaveView spikeWaveView2_; 
  SpikeWaveView spikeWaveView3_; 
  SpikeWaveView spikeWaveView4_; 
  Glib::Timer timer_; 
  Glib::Timer dtimer_; 
  Gtk::Button m_ButtonQuit;
  Gtk::Adjustment spikePosAdjustment_; 
  Gtk::HScale spikePosScale_; 
  void updateSpikePosFromAdj(); 
  sigc::connection m_ConnectionIdle;
  virtual bool on_key_press_event(GdkEventKey* event); 


};

Vis::Vis(bool is_sync)
  : m_VBox(false, 0), 
    table_(2, 2), 
    spikeWaveView1_(), 
    spikeWaveView2_(), 
    spikeWaveView3_(), 
    spikeWaveView4_(), 
    m_ButtonQuit("Quit"),
    spikePosAdjustment_(0.0, 0.0, 10.0, 10.0, 10.0, 0.0), 
    spikePosScale_(spikePosAdjustment_)
{

  set_title("Vis");

  // Get automatically redrawn if any of their children changed allocation.
  set_reallocate_redraws(true);

  add(m_VBox);

  //
  // Vis scene.
  //

  spikeWaveView1_.set_size_request(150, 175);
  spikeWaveView2_.set_size_request(150, 175);
  spikeWaveView3_.set_size_request(150, 175);
  spikeWaveView4_.set_size_request(150, 175);
    
  table_.attach(spikeWaveView1_, 0, 1, 0, 1);
  table_.attach(spikeWaveView2_, 1, 2, 0, 1);
  table_.attach(spikeWaveView3_, 0, 1, 1, 2);
  table_.attach(spikeWaveView4_, 1, 2, 1, 2);

  m_VBox.pack_start(table_); 

  spikePosScale_.set_update_policy(Gtk::UPDATE_DELAYED);
  spikePosScale_.set_digits(1);
  spikePosScale_.set_value_pos(Gtk::POS_TOP);
  spikePosScale_.set_draw_value();
  spikePosScale_.set_size_request(200, 30);
  m_VBox.pack_start(spikePosScale_); 

  //
  // Simple quit button.
  //

  m_ButtonQuit.signal_clicked().connect(
    sigc::mem_fun(*this, &Vis::on_button_quit_clicked));

  m_VBox.pack_start(m_ButtonQuit, Gtk::PACK_SHRINK, 0);

//   spikePosAdjustment_.signal_value_changed().connect(
// 					    sigc::mem_fun(*this, 
// 							  &Vis::updateSpikePosFromAdj ) ); 

  

  show_all();

  Glib::signal_idle().connect( sigc::mem_fun(*this, &Vis::on_idle) );
 timer_.start(); 
 dtimer_.start(); 
}
Vis::~Vis()
{
}

void Vis::on_button_quit_clicked()
{

}
void Vis::updateSpikePosFromAdj()
{
//   std::cout << "updateSpikePosFromAdj: " << spikePosAdjustment_.get_value() << std::endl; 
//   GLSPVect_tpList::iterator svpliter = spvl.begin(); 
  
//   for (int i = 0; i < spikePosAdjustment_.get_value(); i++)
//     {
//       svpliter++; 
//     }
}

bool Vis::on_key_press_event(GdkEventKey* event)
{
  std::cout << "Keypress" << std::endl; 
  switch (event->keyval)
    {
    case GDK_z:
      break;
    case GDK_Z:
      break;
    case GDK_Up:
      std::cout << "Changing zoom" << std::endl; 

      spikeWaveView4_.setViewingWindow(0, -700, 32, 700); 
      spikeWaveView4_.setTime(1000); 

      spikeWaveView3_.setViewingWindow(0, -700, 32, 700); 
      spikeWaveView3_.setTime(1000); 

      spikeWaveView2_.setViewingWindow(0, -700, 32, 700); 
      spikeWaveView2_.setTime(1000); 

      spikeWaveView1_.setViewingWindow(0, -700, 32, 700); 
      spikeWaveView1_.setTime(1000); 




      break;
    case GDK_Down: 
      std::cout << "Changing zoom down" << std::endl; 
      //clusterView12_.setViewingWindow(0, 1, 0, 1); 

      break; 
    default:
      break;
    }
  return true; 

}

void spikeWaves(void); 

bool Vis::on_idle()

{

  spikeWaveView1_.get_window()->invalidate_rect(get_allocation(), true);
  spikeWaveView2_.get_window()->invalidate_rect(get_allocation(), true);
  spikeWaveView3_.get_window()->invalidate_rect(get_allocation(), true);
  spikeWaveView4_.get_window()->invalidate_rect(get_allocation(), true);

  double seconds = timer_.elapsed();
  
  if (seconds >= 1.0)
    {
      std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
      std::cout.setf(std::ios::fixed, std::ios::floatfield);
      std::cout.precision(3);
      int m_Frames = spikeWaveView1_.getFrames(); 
      std::cout << m_Frames << " frames in "
                << seconds << " seconds = "
                << (m_Frames / seconds) << " FPS\n";
      timer_.reset();

    }

   double dseconds = dtimer_.elapsed();
   if (dseconds >= 0.01)
     {
       for (int i = 0; i < 5; i++) {
       spikeWaveView1_.newSpikeWave(swl1.front()); 
       swl1.pop_front(); 
       spikeWaveView2_.newSpikeWave(swl2.front()); 
       swl2.pop_front(); 
       spikeWaveView3_.newSpikeWave(swl3.front()); 
       swl3.pop_front(); 
       spikeWaveView4_.newSpikeWave(swl4.front()); 
       swl4.pop_front(); 
       }
       dtimer_.reset(); 
     }

  
  return true;
}


void spikeWaves(void)
{

  ttreader ttwaves("../d215.tt"); 

  for (int i = 0; i < 100000; i++)
    {
      SpikeWave_t sw1, sw2, sw3, sw4; 
      ttspike tts; 
      ttwaves.getSpike(&tts); 
      // copy
      for (int i = 0; i < 32; i++)
	{
	  sw1.wave.push_back(tts.w1[i]); 
	  sw2.wave.push_back(tts.w2[i]); 
	  sw3.wave.push_back(tts.w3[i]); 
	  sw4.wave.push_back(tts.w4[i]); 

	}

      swl1.push_back(sw1); 
      swl2.push_back(sw1); 
      swl3.push_back(sw1); 
      swl4.push_back(sw1); 
      
    }
} 

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);

  //
  // Init gtkglextmm.
  //

  Gtk::GL::init(argc, argv);

  
  spikeWaves(); 
  std::cout << "swl1.size() = " << swl1.size() << std::endl; 

  Vis vis(true);

  kit.run(vis);

  return 0;
}
