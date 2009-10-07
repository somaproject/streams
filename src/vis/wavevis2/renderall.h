#ifndef __VIS_WAVEVIS2_RENDERALL_H__
#define __VIS_WAVEVIS2_RENDERALL_H__

#include <db_cxx.h>
#include <boost/filesystem.hpp>

#include "irenderer.h"

#include "types.h"

namespace wavevis2 { 

namespace bf = boost::filesystem;

int renderall_compare_timeid(Db *db, const Dbt *a, const Dbt *b); 


class RenderAll : public IRenderer
{
  /* 
     Store and render all (lowest-level) data points
     
  */
public:
  RenderAll(bf::path scratchdir); 
  ~RenderAll(); 

  void renderStream(timeid_t t1, timeid_t t2, int pixels); 
  void newSample( WaveBuffer_t & ); 
  void reset(); 


private:
  DbEnv dbEnv_; 
  
  Db * db_; 

  void renderGLPointBuffer(timeid_t origintime, 
			  GLPointBuffer_t * bufptr); 


}; 

}

#endif
