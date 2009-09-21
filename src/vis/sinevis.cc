#include "sinevis.h"

const std::string SineVis::TYPENAME = "SineVis"; 

SineVis::SineVis(std::string name, bf::path scratchdir) :
  VisBase(name)
{
  

}

SineVis::~SineVis()
{


}

void SineVis::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{

  float fs = 10.0; 
  float samprate = 1000; 

  glLineWidth(1.0); 
  glColor4f(1.0, 1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP); 
  streamtime_t curtime = t1; 
  while(curtime < t2) {
    glVertex2f(curtime , sin(curtime * fs * 3.14* 2) * 100); 
    curtime += 1/samprate; 
  }
  
  glEnd(); 
  

}

// TriggerRenderer interface:
void SineVis::renderTrigger(streamtime_t deltapre, streamtime_t deltapost, 
			    int pixels)
{


}

int SineVis::getPixelHeight() {

  return 100; 

}

void SineVis::setPixelHeight(int x) {
  // FIXME: do something


}
