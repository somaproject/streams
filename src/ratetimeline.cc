#include "ratetimeline.h"
#include "glconfig.h"
#include "shaders.h"

RateTimeline::RateTimeline() : 
  majorTick_(40000), 
  minorTick_(4000), 
  decayRange_(100), 
  viewLatest_(true), 
  cutoffPos_(0), 
  viewT1_(0.0), 
  viewT2_(1000), 
  viewX1_(-100.0), 
  viewX2_(100.0),
  zoomLevel_(1.0),
  s1fact_(2)
{

  rates_.reserve(1000000); 
  ratesS1_.reserve(1000000/s1fact_); 

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
	     Gdk::SCROLL_MASK |
	     Gdk::BUTTON_PRESS_MASK      |
	     Gdk::VISIBILITY_NOTIFY_MASK);
  

  // View transformation signals.
  signal_button_press_event().connect(sigc::mem_fun(*this, 
						    &RateTimeline::on_button_press_event)); 
  
  signal_motion_notify_event().connect(sigc::mem_fun(*this, 
						     &RateTimeline::on_motion_notify_event)); 

  signal_scroll_event().connect(sigc::mem_fun(*this, 
						     &RateTimeline::on_scroll_event)); 


}

RateTimeline::~RateTimeline()
{
}


void RateTimeline::on_realize()
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

  GLuint vshdr = loadGPUShader("rtgrad.vert", GL_VERTEX_SHADER); 
  GLuint fshdr = loadGPUShader("rtgrad.frag", GL_FRAGMENT_SHADER); 
  std::list<GLuint> shaders; 
  shaders.push_back(vshdr); 
  shaders.push_back(fshdr); 
  gpuProgGradient_ = createGPUProgram(shaders); 

  updateViewingWindow(true); 

  gldrawable->gl_end();
  // *** OpenGL END ***

}

void RateTimeline::updateViewingWindow(bool reset = false)

{
  glLoadIdentity(); 

  if (reset ) {
    viewT1_ = 0.0; 
    viewT2_ = get_width(); 
  }
  glOrtho(viewT1_, viewT2_, viewX1_, viewX2_, -3, 3); 

  glViewport(0, 0, get_width(), get_height());
  //viewChanged_ = false; 
  
}

bool RateTimeline::on_configure_event(GdkEventConfigure* event)
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

void RateTimeline::appendRate(GLWavePoint_t p)
{
  
  rates_.push_back(p); 
  // now, let's try and do something more advanced, like take 
  // the max point or the min point... 

  updateViewingWindow(); 
  get_window()->invalidate_rect(get_allocation(), true);
  
}

bool RateTimeline::on_expose_event(GdkEventExpose* event)
{

  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // *** OpenGL BEGIN ***
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  updateViewingWindow(); 
  glClearColor(0.0, 0.0, 0.0, 1.0); 
  glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT ); 

  int N = rates_.size(); 
  
  glLineWidth(1.0); 
  // draw minor ticks
  glColor4f(0.0, 0.0, 1.0, 0.5); 

  for (int i = 0; i < (N / minorTick_ + 1); i++)
    {
      glBegin(GL_LINE_STRIP); 
      glVertex2f(float(i*minorTick_), viewX1_); 
      glVertex2f(float(i*minorTick_), viewX2_); 

      glEnd(); 
	
    }

  // draw major ticks
  glColor4f(0.7, 0.7, 1.0, 0.9); 

  for (int i = 0; i < (N / majorTick_ + 1); i++)
    {
      glBegin(GL_LINE_STRIP); 
      glVertex2f(float(i*majorTick_), viewX1_); 
      glVertex2f(float(i*majorTick_), viewX2_); 

      glEnd(); 
	
    }

  // draw actual primary line


//   useGPUProgram(gpuProgGradient_); 
//   // now draw filled segment gradient
//   GLint vp = glGetUniformLocation(gpuProgGradient_, "rate"); 
//   glUniform1f(vp, float(decayRange_)); 
//   GLint vp2 = glGetUniformLocation(gpuProgGradient_, "cutoff"); 
//   if (viewLatest_) {
//     glUniform1f(vp2, float(rates_.size())); 
//   } else {
//     glUniform1f(vp2, float(cutoffPos_)); 
//   }
	       


//   // primary polygons (filled)

//   for (int i = 1; i < N ; i++)
//     {

//       glBegin(GL_POLYGON); 
//       float ratel = rates_[i-1]; 
//       float rater = rates_[i]; 
//       glVertex2f(float(i-1), 0.0);
//       glVertex2f(float(i-1), ratel); 
//       glVertex2f(float(i), rater); 
//       glVertex2f(float(i), 0.0); 
//       glEnd(); 
//     }

//   glEnd(); 
//   useGPUProgram(0); 

  // main line
  
//   float basealpha = 1.0; 
//   if (zoomLevel_ > 20) {
//     basealpha = 1 - (zoomLevel_ - 20)/20; 
//    }
  
  //  if (zoomLevel_ < 40 ) {
    glColor4f(1.0, 1.0, 1.0, 1.0); 
    glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t),
		    &(rates_[0])); 
    
    glDrawArrays(GL_LINE_STRIP, 0, rates_.size()); 

//   }
  
//   float alphas1 = 0.0; 
//   if (zoomLevel_ > 10) {
//     alphas1 =  (zoomLevel_ - 10)/30; 
//   }
//   if (zoomLevel_ > 10) {
//     glColor4f(1.0, 0.0, 0.0, 1.0); 
//     glBegin(GL_LINE_STRIP); 
//     for (int i = 0; i < ratesS1_.size() ; i++)
//       {
// 	glVertex2f(float(i)*s1fact_, ratesS1_[i]); 
//       }
//     glEnd(); 


//     glColor4f(1.0, 1.0, 1.0, 1.0); 
//     glPointSize(4.0); 
//     glBegin(GL_POINTS); 
//     for (int i = 0; i < ratesS1_.size() ; i++)
//       {
// 	glVertex2f(float(i)*s1fact_, ratesS1_[i]); 
//       }
//     glEnd(); 
    
    //  }

  // Swap buffers.
  gldrawable->swap_buffers();
  gldrawable->gl_end();

  return true;
}

bool RateTimeline::on_map_event(GdkEventAny* event)
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

bool RateTimeline::on_unmap_event(GdkEventAny* event)
{

  return true;
}

bool RateTimeline::on_visibility_notify_event(GdkEventVisibility* event)
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

bool RateTimeline::on_button_press_event(GdkEventButton* event)
{
//   m_BeginX = event->x;
//   m_BeginY = event->y;
  lastX_ = event->x; 

  // don't block
  return false;
}

void RateTimeline::setZoom(float zoomval, float tcenter)
{
  /*
    tcenter == the position along the t axis under the cursor

  
  */


  std::cout << "zoomVal = " << zoomval 
	    << " tcenter = " << tcenter
	    << std::endl; 
  float oldviewwidth = (viewT2_ - viewT1_); 
  float oldZoom = zoomLevel_; 
  float zoomdelta = zoomval / zoomLevel_; 
  float newViewWidth  = zoomdelta * oldviewwidth; 

  // now we just need to position the width such that the tcenter stays
  // fixed

  float newT1 = tcenter - (tcenter - viewT1_) / (viewT2_ - viewT1_) * newViewWidth; 
  float newT2 = tcenter + (viewT2_ - tcenter) / (viewT2_ - viewT1_) * newViewWidth; 
  viewT1_ = newT1; 
  viewT2_ = newT2; 

  zoomLevel_ = zoomval; 

  
  //std::cout << newviewwidth/oldviewwidth - zoomval / oldZoom << std::endl; 
}

bool RateTimeline::on_scroll_event(GdkEventScroll* event)
{

  float x = event->x;
  float y = event->y;
  
  std::cout << "Scroll event" << x <<  y << event->direction <<  std::endl; 


  float centerPos = (x / get_width()) * (viewT2_ - viewT1_)  +  viewT1_; 

  float newzoom = 1.0; 

  if (event->direction == 0) {
    // UP? zoom out
    newzoom = zoomLevel_ *  15.0/16.0; 
    
  } else {
    newzoom = zoomLevel_ *  16.0/15.0; 

  }

  setZoom(newzoom, centerPos); 

  updateViewingWindow(); 

  get_window()->invalidate_rect(get_allocation(), true);
  
  update();
  
  return false; 
}
bool RateTimeline::on_motion_notify_event(GdkEventMotion* event)
{

  float x = event->x;
  float y = event->y;

  float moveDelta = float(x) - float(lastX_); 
  
  float windowDelta = moveDelta * zoomLevel_; 

  if (event->state & GDK_BUTTON1_MASK) {
    viewT1_ -= windowDelta; 
    viewT2_ -= windowDelta;
    get_window()->invalidate_rect(get_allocation(), true);
    lastX_ = x; 
    //std::cout << " Moving..." << std::endl; 
    //invalidate(); 
    update();
  } 
  else if (event->state & GDK_SCROLL_MASK) {
    std::cout << "Button 4!" << std::endl; 
    
  }

  // don't block
  return true;
}

