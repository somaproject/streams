#ifndef __VIS_WAVEVIS2_RENDERDOWNSAMPLE_H__
#define __VIS_WAVEVIS2_RENDERDOWNSAMPLE_H__

#include <db_cxx.h>
#include <boost/filesystem.hpp>
#include <set>
#include <boost/thread.hpp>


#include "irenderer.h"

#include "types.h"

namespace wavevis2 { 

namespace bf = boost::filesystem;

int renderdownsample_compare_timeid(Db *db, const Dbt *a, const Dbt *b); 


class RenderDownSample : public IRenderer
{
  /* 
     Downsample the data, using buffers that are binsize in length
     with samples that are sampledur long. 
     
     e.g. if sampledur = 1000 we use 1 us long samples, 
     and if binsize = 1000 then each sample buffer is 1 ms. 

     This lets us, for a given data-point-in-time,
     compute precisely which bin it should be in. 

     to reduce some of the burden on the BDB interface,
     we keep an in-memory std::set of every current active
     bin's starting time, so we can do O(1) lookup of
     which bins exist before we search for them. 
     
  */
public:
  RenderDownSample(timeid_t sampledur, size_t binsize, 
		   bf::path scratchdir); 
  ~RenderDownSample(); 

  void renderStream(timeid_t t1, timeid_t t2, int pixels); 
  void newSample( pWaveBuffer_t  ); 
  void reset(); 


private:
  const timeid_t sampledur_; 
  static const size_t binsize_ = GLPointBuffer_t::BUFFERN; 
  const timeid_t bindur_; 
  DbEnv dbEnv_; 
  
  Db * db_; 

  std::set<timeid_t> presentBins_; 
  timeid_t getBinStart(timeid_t tid); 


  void renderGLPointBuffer(timeid_t origintime, 
			  GLPointBuffer_t * bufptr); 

  typedef boost::shared_lock<boost::shared_mutex> shared_lock_t; 
  typedef boost::upgrade_lock<boost::shared_mutex> upgrade_lock_t; 
  typedef boost::upgrade_to_unique_lock<boost::shared_mutex> up_unique_lock_t; 

  boost::shared_mutex truncate_mutex_; 

}; 

}

#endif
