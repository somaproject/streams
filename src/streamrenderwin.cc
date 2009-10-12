#include "streamrenderwin.h"
#include "glconfig.h"
#include "shaderutil/shaders.h"

#include <boost/format.hpp>

StreamRenderWin::StreamRenderWin(pVisControl_t pvc) : 
  decayRange_(100), 
  cutoffPos_(0), 
  trackLive(false), 
  viewT1_(0.0), 
  viewT2_(1.0), 
  viewX1_(-1000.0), 
  viewX2_(00.0),
  selT1_(0.0), 
  selT2_(1.0), 
  zoomLevel_(1.0),
  s1fact_(2), 
  pVisControl_(pvc), 
  timeGLstring_("sans", false, CENTER, BASELINE),
  viewType_(STRIPCHART)
{

  Glib::RefPtr<Gdk::GL::Config> glconfig;
  
  // Try double-buffered visual
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA   |
                                     Gdk::GL::MODE_DOUBLE | 
				     Gdk::GL::MODE_ACCUM | 
				     Gdk::GL::MODE_MULTISAMPLE);
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
  
  pvc->invStreamRenderSignal().connect(sigc::mem_fun(*this, &StreamRenderWin::invalidate)); 
  
  // View transformation signals.
  //   signal_button_release_event().connect(sigc::mem_fun(*this, 
  //  						    &StreamRenderWin::on_button_release_event)); 
  
  signal_motion_notify_event().connect(sigc::mem_fun(*this, 
						     &StreamRenderWin::on_motion_notify_event)); 

  signal_scroll_event().connect(sigc::mem_fun(*this, 
						     &StreamRenderWin::on_scroll_event)); 

  Glib::signal_timeout().connect(sigc::mem_fun(*this,
					       &StreamRenderWin::invalidateCallback), 
				 20);
  
}

StreamRenderWin::~StreamRenderWin()
{
}



void StreamRenderWin::on_realize()
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
  glEnable(GL_MULTISAMPLE_ARB);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_LINE_SMOOTH); 
  glEnable(GL_POINT_SMOOTH); 
  glEnable(GL_POLYGON_SMOOTH); 
  glEnableClientState(GL_VERTEX_ARRAY); 
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//   GLuint vshdr = loadGPUShader("rtgrad.vert", GL_VERTEX_SHADER); 
//   GLuint fshdr = loadGPUShader("rtgrad.frag", GL_FRAGMENT_SHADER); 
//   std::list<GLuint> shaders; 
//   shaders.push_back(vshdr); 
//   shaders.push_back(fshdr); 
//   gpuProgGradient_ = createGPUProgram(shaders); 

  // create font display lists
  fontListBase_ = glGenLists(128); 
  Pango::FontDescription font_desc("bitstream vera sans mono"); 
  Glib::RefPtr<Pango::Font> font =
    Gdk::GL::Font::use_pango_font(font_desc, 0, 128, fontListBase_);
  if (!font)
    {
      std::cerr << "*** Can't load font "
                << std::endl;
      Gtk::Main::quit();
    }
  
  Pango::FontMetrics font_metrics = font->get_metrics();

  fontHeight_ = font_metrics.get_ascent() + font_metrics.get_descent();
  fontHeight_ = PANGO_PIXELS(fontHeight_);

  updateViewingWindow(true); 

  gldrawable->gl_end();
  // *** OpenGL END ***
  
}

void StreamRenderWin::updateViewingWindow(bool reset = false)

{
  glLoadIdentity(); 

  if (reset ) {
    viewT1_ = 0.0; 
    viewT2_ = 1.0; 
  }
  viewX1_ = 0; 
  viewX2_ = get_height(); 
  glOrtho(0, viewT2_-viewT1_, viewX1_, viewX2_, -3, 3); 

  glViewport(0, 0, get_width(), get_height());
  //viewChanged_ = false; 
  
}

void StreamRenderWin::renderTimeTicks(float T1, float T2)
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

  } else if (winSize < 1000)  {
    minorScale = 10.0; 
    majorScale = 60.0;  
    scaletext = "min"; 
    scaletextdiv = 60.0;     

  } else if (winSize < 10000)  { 
    minorScale = 60.0; 
    majorScale = 600.0;  
    scaletext = "min"; 
    scaletextdiv = 60.0;     

  } else { 
    minorScale = 600.0; 
    majorScale = 3600.0;  
    scaletext = "hr"; 
    scaletextdiv = 3600.0;     

  }
  
 
  glLineWidth(1.0); 
  // draw minor ticks
  float mAlpha = winSize / 100.0 / minorScale; 

  glColor4f(0.0, 0.0, 1.0, 0.5 - mAlpha/2);

  int minorN = int(round((T2 - T1)/ minorScale)) ;

  // now remember, T1 is zero 

  // compute lower bound
  float T1n = round(T1 / minorScale); 

  for (int i = -1; i < (minorN+1) + 1; i++)
    {
      glBegin(GL_LINE_STRIP); 
      glVertex2f(float((i+T1n)*minorScale) - T1, viewX1_); 
      glVertex2f(float((i+T1n)*minorScale) - T1, viewX2_); 

      glEnd(); 
	
    }

  // draw major ticks
  glColor4f(0.6, 0.6, 1.0, 0.7); 
  glLineWidth(1.0); 

  int majorN = int(round((T2 - T1)/ majorScale)) ;
  float T1n2 = round(T1 / majorScale); 

  for (int i = -1; i < (majorN+1) + 1; i++)
    {
      glColor4f(0.6, 0.6, 1.0, 0.7); 
      GLfloat t =  (i+T1n2) * majorScale; 

      glBegin(GL_LINE_STRIP); 
      glVertex2f(t-T1, viewX1_); 
      glVertex2f(t-T1, viewX2_); 
      glEnd(); 
      
      timeformat % (t / scaletextdiv) % scaletext; 
      std::string timestr = timeformat.str(); 
      timeGLstring_.drawWorldText(t -  T1, 10.0, timestr, 15.0); 

      
    }


}

bool StreamRenderWin::on_configure_event(GdkEventConfigure* event)
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

  return true;
}

bool StreamRenderWin::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  // *** OpenGL BEGIN ***
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  updateViewingWindow(); 
  glClearColor(0.0, 0.0, 0.0, 1.0); 
  glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT ); 

  int N = 10000000; 
  
  renderTimeTicks(viewT1_, viewT2_); 


  std::list<pIVis_t>::iterator pwd; 
  
  int pixwidth = get_width(); 
  // get the windows own allocation and use that
  
  Gdk::Rectangle myalloc = get_allocation(); 
  int posaccum = 0; 
  std::list<pIVis_t> vl = pVisControl_->getVisList(); 
  for (pwd = vl.begin(); pwd != vl.end();  pwd++)
    {
      int height =  (*pwd)->getPixelHeight(); 
      float pos = - (height/2 + posaccum) + 
	myalloc.get_height() + myalloc.get_y(); 
      glPushMatrix(); 
      glTranslatef(0.0f, pos, 0.0); 
      (*pwd)->renderStream(viewT1_, viewT2_, pixwidth); 
      glPopMatrix(); 
      posaccum += height; 
    }


  // render selection
  glColor4f(0.2, 0.2, 1.0, 0.5); 

  glBegin(GL_POLYGON); 
  glVertex2f(selT1_ - viewT1_, viewX1_); 
  glVertex2f(selT1_ - viewT1_, viewX2_); 
  glVertex2f(selT2_ - viewT1_, viewX2_); 
  glVertex2f(selT2_ - viewT1_, viewX1_); 
  glEnd(); 

//   // render current position
  renderCurrentTimeCursor(); 
  
  // Swap buffers.
  gldrawable->swap_buffers();
  gldrawable->gl_end();

  return true;
}

bool StreamRenderWin::on_map_event(GdkEventAny* event)
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

bool StreamRenderWin::on_unmap_event(GdkEventAny* event)
{

  return true;
}

bool StreamRenderWin::on_visibility_notify_event(GdkEventVisibility* event)
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

void StreamRenderWin::setSelectionRegion(float t1, float t2)
{
  if (t1 < t2) {
    selT1_ = t1; 
    selT2_ = t2; 
  } else {
    selT1_ = t2; 
    selT2_ = t1; 
  }
  invalidate(); 

}

bool StreamRenderWin::on_button_press_event(GdkEventButton* event)
{
//   m_BeginX = event->x;
//   m_BeginY = event->y;
  if (event->type == GDK_BUTTON_PRESS)  
    {
      lastX_ = event->x; 
      trackLive = false; 
    } 

  // don't block
  return false;
}

bool StreamRenderWin::on_button_release_event(GdkEventButton* event)
{

  if (event->button == 3) {
	// update the zoom selection 
    viewT1_ = selT1_; 
    viewT2_ = selT2_; 
    selT2_ = selT1_; 
    invalidate(); 
    
    
  }

  // don't block
  return false;
}

void StreamRenderWin::setZoom(float zoomval, float tcenter)
{
  /*
    tcenter == the position along the t axis under the cursor

  
  */


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

  
}

bool StreamRenderWin::on_scroll_event(GdkEventScroll* event)
{

  float x = event->x;
  float y = event->y;
  
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

  invalidate(); 
  
  update();
  
  return false; 
}
bool StreamRenderWin::on_motion_notify_event(GdkEventMotion* event)
{

  float x = event->x;
  float y = event->y;

  float movePixDelta = float(x) - float(lastX_); 
  
  
  //float windowDelta = moveDelta * zoomLevel_; 
  float pixWidth = (viewT2_ - viewT1_) / get_width(); 
  float windowDelta = movePixDelta * pixWidth; 
 


  if (event->state & GDK_BUTTON1_MASK) {
    viewT1_ -= windowDelta; 
    viewT2_ -= windowDelta;
    invalidate(); 
    lastX_ = x; 
    //invalidate(); 

    update();
  } 

  else if (event->state & GDK_BUTTON3_MASK) {
    setSelectionRegion(viewT1_ + lastX_*pixWidth, viewT1_ + x*pixWidth); 
  }

  else if (event->state & GDK_SCROLL_MASK) {
    
  }


  // don't block propagation
  return true;
}


void StreamRenderWin::invalidate() {
//   Glib::RefPtr<Gdk::Window> win = get_window();
// //   Gdk::Rectangle r(0, 0, get_allocation().get_width(),
// // 		   get_allocation().get_height());
//   Gdk::Rectangle rect = get_allocation(); 
//   win->invalidate_rect(rect, false); 
  
}


bool StreamRenderWin::invalidateCallback() {
  Glib::RefPtr<Gdk::Window> win = get_window();
//   Gdk::Rectangle r(0, 0, get_allocation().get_width(),
// 		   get_allocation().get_height());
  Gdk::Rectangle rect = get_allocation(); 
  win->invalidate_rect(rect, false); 
  
}

void StreamRenderWin::setViewType(ViewTypes val)
{
  std::cout << "Setting view type" << std::endl;
  viewType_ = val; 
  invalidate(); 

}

void StreamRenderWin::setCurrentTime(streamtime_t time)
{
  float tdelta = time - currentTime_; 

  currentTime_ = time; 
//   std::cout << "currentTime_ = " << currentTime_ << " viewT2_ =" 
// 	    << viewT2_ << std::endl;
  
  float twidth = viewT2_ - viewT1_; 
  
  // update tracklive
  if (trackLive.pendingRequest()) {
    trackLive.set_value(trackLive.get_req_value()); 
  }

  if (trackLive) { 
    if (viewType_ == STRIPCHART ) {
      //    if (currentTime_ > viewT2_) {
      // this is the sweep-view
      viewT1_ = currentTime_ - twidth;
      viewT2_ = currentTime_;
      invalidate();
      //    }
    }
  } else if ((currentTime_ < viewT2_) and (currentTime_ > viewT1_)) {
    invalidate(); 
  } 
  
  
}

void StreamRenderWin::renderCurrentTimeCursor()
{

  glLineWidth(4.0);
  glColor4f(0.5, 0.5, 1.0, 1.0); 
  glBegin(GL_LINES); 
  glVertex2f(currentTime_ - viewT1_, viewX1_); 
  glVertex2f(currentTime_ - viewT1_, viewX2_); 
  glEnd(); 
    

}

