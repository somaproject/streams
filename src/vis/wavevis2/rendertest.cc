#include <boost/foreach.hpp>
#include "rendertest.h"
#include "types.h"

namespace wavevis2 {


RenderTest::RenderTest(bf::path scratch, int scale) :
  scale_(scale)
{
  
}

RenderTest::~RenderTest()
{

}

void RenderTest::renderStream(timeid_t t1, timeid_t t2, int pixels)
{
  /* Must be thread-safe!
   */

  timeid_t searchval = t1; 
  
  minmaxmap_t::iterator mmi = minmaxmap_.lower_bound(t1); 
  if(mmi != minmaxmap_.begin()) 
    {
      mmi--; 
    }
  
  glColor4f(1.0, 0.0, 0.0, 1.0); 
  GLint vals[2]; 
  glGetIntegerv(GL_POLYGON_MODE, vals); 

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
  
  for(mmi; (mmi != minmaxmap_.end()) and (mmi->first < t2); mmi++) {
    // quads!
    
    // right now, just plot a box
//     float starttime = float(mmi->second->start - t1)/1e9; 
//     float endtime = float(mmi->second->end - t1)/1e9;
//     std::cout << " rendering mmi, starttime= "
// 	      << starttime << " end time = " << endtime 
// 	      << " min = " << mmi->second->min 
// 	      << " max = " << mmi->second->max << std::endl;
    glPushMatrix();
    
    pMinMaxVector_t mmv = mmi->second; 
    glTranslatef(double(mmi->second->start - t1)/1e9, 0, 0); 
    //glLineWidth(1.0)n
    glColor4f(1.0, 1.0, 1.0, 1.0); 
    //glBegin(GL_POINTS);
    glBegin(GL_QUAD_STRIP); 
    
    for(int i = 0; i < mmv->times.size(); i++) { 
      glVertex2f(mmv->times[i], mmv->mins[i]); 
      glVertex2f(mmv->times[i], mmv->maxs[i]); 

    }
    glEnd(); 
    glPopMatrix(); 
    
  }
  glPolygonMode(GL_FRONT, vals[0]); 
  glPolygonMode(GL_BACK, vals[1]); 
  
}

void RenderTest::reset() {
  // FIXME

}


void RenderTest::newDataWindow(const elements::datawindow_t<pWaveBuffer_t> & datawindow ) 
{
  BOOST_FOREACH(pWaveBuffer_t wb, datawindow.data) {
    if (data_.find(wb->time) == data_.end()) { 
      data_.insert(std::make_pair(wb->time, wb)); 
      pMinMaxVector_t mmv = downsample_minmax_var(wb, scale_); 
      minmaxmap_.insert(std::make_pair(wb->time, mmv)); 
    }
  }
  
}

}
