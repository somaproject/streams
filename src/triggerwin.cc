#include "triggerwin.h"
#include "glconfig.h"
#include "shaders.h"

#include <boost/format.hpp>

TriggerWin::TriggerWin() : 
  viewT1_(-0.01000), 
  viewT2_(0.0100), 
  viewX1_(-1000.0), 
  viewX2_(100.0)
{

  Glib::RefPtr<Gdk::GL::Config> glconfig;
  
  // Try double-buffered visual
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA   |
                                     Gdk::GL::MODE_DOUBLE | 
				     Gdk::GL::MODE_ACCUM);
  if (!glconfig)
    {
      throw std::runtime_error("Could not acquire double-buffered visual"); 
    }

  // print frame buffer attributes.
  GLConfigUtil::examine_gl_attrib(glconfig);

  set_gl_capability(glconfig);

  // Add events.
  add_events(Gdk::BUTTON1_MOTION_MASK    |
	     Gdk::BUTTON2_MOTION_MASK    |
	     Gdk::BUTTON3_MOTION_MASK    | 
	     Gdk::SCROLL_MASK |
	     Gdk::BUTTON_PRESS_MASK      |
	     Gdk::BUTTON_RELEASE_MASK  | 
	     Gdk::VISIBILITY_NOTIFY_MASK);
  

}

TriggerWin::~TriggerWin()
{
}


void TriggerWin::appendVis(StreamVis * wv)
{
  pStreamVis_.push_back(wv); 
  //connect the invalidate signal 
  wv->invTriggerWaveSignal().connect(sigc::mem_fun(*this, 
 							 &TriggerWin::invalidate)); 

}

void TriggerWin::on_realize()
{

  // We need to call the base on_realize()
  Gtk::DrawingArea::on_realize();

  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // *** OpenGL BEGIN ***
  if (!gldrawable->gl_begin(get_gl_context()))
    return;

  //glEnable(GL_NORMALIZE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
  glEnable(GL_BLEND); 

  glEnable(GL_LINE_SMOOTH); 
  glEnable(GL_POINT_SMOOTH); 
  glEnableClientState(GL_VERTEX_ARRAY); 
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


  updateViewingWindow(true); 

  gldrawable->gl_end();
  // *** OpenGL END ***

}

void TriggerWin::updateViewingWindow(bool reset = false)

{
  glLoadIdentity(); 

  if (reset ) {
    viewT1_ = -0.01; 
    viewT2_ = 0.01; 
  }
  glOrtho(viewT1_, viewT2_, viewX1_, viewX2_, -3, 3); 

  glViewport(0, 0, get_width(), get_height());
  //viewChanged_ = false; 
  
}

void TriggerWin::renderTimeTicks(float T1, float T2)
{
  // render the necessary ticks for a window between T1 and T2

  // compute tick scale major and minor
  float majorScale = 0.0; 
  float minorScale = 0.0; 
  
  float winSize = T2 - T1; 

  boost::format timeformat ("%1% %2%"); 
  std::string scaletext; 
  float scaletextdiv = 1.0; 
  if (winSize < 100e-3) {
    
    minorScale = 1e-3; 
    majorScale = 10e-3; 
    scaletext = "ms"; 
    scaletextdiv = 1e-3; 
    
  } else if ( winSize < 1 ) {
    minorScale = 10e-3; 
    majorScale = 100e-3; 
    scaletext = "ms"; 
    scaletextdiv = 1e-3; 
    
  } else if ( winSize < 10 ) {
    minorScale = 100e-3; 
    majorScale = 1000e-3; 
    scaletext = "sec"; 
    scaletextdiv = 1.0;     
  } else if ( winSize < 100) {
    minorScale = 1.0;
    majorScale = 10.0; 
    scaletext = "sec"; 
    scaletextdiv = 1.0;     

  } else {
    minorScale = 10.0; 
    majorScale = 60.0;  
    scaletext = "min"; 
    scaletextdiv = 60.0;     

  }
  
 
  glLineWidth(1.0); 
  // draw minor ticks
  float mAlpha = winSize / 100.0 / minorScale; 

  glColor4f(0.0, 0.0, 1.0, 0.5 - mAlpha/2);

  int minorN = int(round((T2 - T1)/ minorScale)) ;

  // compute lower bound
  int T1n = round(T1 / minorScale); 

  for (int i = -1; i < (minorN+1) + 1; i++)
    {
      glBegin(GL_LINE_STRIP); 
      glVertex2f(float((i+T1n)*minorScale), viewX1_); 
      glVertex2f(float((i+T1n)*minorScale), viewX2_); 

      glEnd(); 
	
    }

  // draw major ticks
  glColor4f(0.6, 0.6, 1.0, 0.7); 
  glLineWidth(1.0); 

  int majorN = int(round((T2 - T1)/ majorScale)) ;
  int T1n2 = round(T1 / majorScale); 

  for (int i = -1; i < (majorN+1) + 1; i++)
    {
      glColor4f(0.6, 0.6, 1.0, 0.7); 
      GLfloat t =  (i+T1n2) * majorScale; 

      glBegin(GL_LINE_STRIP); 
      glVertex2f(t, viewX1_); 
      glVertex2f(t, viewX2_); 
      glEnd(); 
      
//       // render associated text
//       glColor3f(1.0, 1.0, 1.0);
//       float pixx = (t - T1) / (T2-T1) * get_width(); 
//       glRasterPos2f(t, 70);
//       glListBase(fontListBase_);
      
//       timeformat % (t / scaletextdiv) % scaletext; 
//       std::string timestr = timeformat.str(); 

//       glCallLists(timestr.size(), GL_UNSIGNED_BYTE, timestr.c_str()); 
    }


}

bool TriggerWin::on_configure_event(GdkEventConfigure* event)
{


  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();


  // *** OpenGL BEGIN ***

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;
  gldrawable->wait_gdk(); 
  glDrawBuffer(GL_BACK); 

  updateViewingWindow(); 

  glClearColor(0.0, 0.0, 0.0, 1.0); 
  glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT ); 
  

  gldrawable->wait_gl(); 
  //gldrawable->wait_gdk(); 
  
  gldrawable->gl_end();
  // *** OpenGL END ***

  //invalidate(); 
  return true;
}

bool TriggerWin::on_expose_event(GdkEventExpose* event)
{

  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // *** OpenGL BEGIN ***
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  updateViewingWindow(); 
  glClearColor(0.0, 0.0, 0.0, 1.0); 
  glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT ); 

  int N = 10000000; 
  // configure matrices
//   glMatrixMode(GL_MODELVIEW); 
//   glLoadIdentity(); 
  
  
  renderTimeTicks(viewT1_, viewT2_); 


  std::list<StreamVis*>::iterator pwd; 
  int pixwidth = get_width(); 
  for (pwd = pStreamVis_.begin(); pwd != pStreamVis_.end(); pwd++)
    {
      (*pwd)->drawTriggerWave(0.01, 0.01, 0.0); 
      glTranslatef(0.0f, -120.0f, 0.0);

    }
  glTranslatef(0.0f, 120.0f * pStreamVis_.size(), 0.0);

  
  // Swap buffers.
  gldrawable->swap_buffers();
  gldrawable->gl_end();

  return true;
}

bool TriggerWin::on_map_event(GdkEventAny* event)
{

  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;
  gldrawable->wait_gdk(); 

  glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT ); 
  
  updateViewingWindow(); 

  gldrawable->wait_gl(); 
  //gldrawable->wait_gdk(); 
  
  gldrawable->gl_end();
  // *** OpenGL END ***


  return true;
}

bool TriggerWin::on_unmap_event(GdkEventAny* event)
{

  return true;
}

bool TriggerWin::on_visibility_notify_event(GdkEventVisibility* event)
{

  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;
  gldrawable->wait_gdk(); 
  glDrawBuffer(GL_BACK); 

  glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT ); 
  

  gldrawable->wait_gl(); 
  //gldrawable->wait_gdk(); 
  
  gldrawable->gl_end();

  return true;
}

void TriggerWin::invalidate() {
  Glib::RefPtr<Gdk::Window> win = get_window();
  Gdk::Rectangle r(0, 0, get_allocation().get_width(),
		   get_allocation().get_height());
  win->invalidate_rect(r, false);
  
}

