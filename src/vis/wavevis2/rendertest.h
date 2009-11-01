#ifndef __VIS_WAVEVIS2_RENDERTEST_H__
#define __VIS_WAVEVIS2_RENDERTEST_H__

#include <boost/itl/interval_set.hpp>
#include <map>

#include "irenderer.h"
#include "downsample.h"
#include "types.h"

namespace wavevis2 { 


class RenderTest : public IRenderer
{
  /* 
     Store and render all (lowest-level) data points. We basically
     save the naked waveBuffers to disk, and then
     read them out as needed. 
     
  */
public:
  RenderTest(bf::path scratchdir, int scale); 
  ~RenderTest(); 

  void renderStream(timeid_t t1, timeid_t t2, int pixels); 
  void newDataWindow(const elements::datawindow_t<pWaveBuffer_t> & ); 
  void reset(); 

  inline void newSample(pWaveBuffer_t wb) {


  }

private:
  void renderGLPointBuffer(timeid_t origintime, 
			  GLPointBuffer_t * bufptr); 

  typedef boost::shared_lock<boost::shared_mutex> shared_lock_t; 
  typedef boost::upgrade_lock<boost::shared_mutex> upgrade_lock_t; 
  typedef boost::upgrade_to_unique_lock<boost::shared_mutex> up_unique_lock_t; 

  boost::shared_mutex truncate_mutex_; 
  
  typedef  std::map<timeid_t, pWaveBuffer_t> datamap_t; 
  datamap_t data_; 

  typedef boost::itl::interval_set<timeid_t> intervals_t; 
  intervals_t observed_intervals_; 
  
  typedef std::map<timeid_t, pMinMaxVector_t> minmaxmap_t; 
  minmaxmap_t minmaxmap_; 

  int scale_; 

}; 

}

#endif
