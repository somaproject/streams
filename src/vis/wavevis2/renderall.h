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
     Store and render all (lowest-level) data points. We basically
     save the naked waveBuffers to disk, and then
     read them out as needed. 
     
  */
public:
  RenderAll(bf::path scratchdir); 
  ~RenderAll(); 

  void renderStream(timeid_t t1, timeid_t t2, int pixels); 
  void newSample( pWaveBuffer_t ); 
  void reset(); 


private:

  void renderGLPointBuffer(timeid_t origintime, 
			  GLPointBuffer_t * bufptr); 

  typedef  std::map<timeid_t, pWaveBuffer_t> datamap_t; 
  datamap_t data_; 


  
}; 

}

#endif
