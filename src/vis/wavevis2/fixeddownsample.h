#ifndef __VIS_WAVEVIS2_FIXEDDOWNSAMPLE_H__
#define __VIS_WAVEVIS2_FIXEDDOWNSAMPLE_H__

#include <set>

#include "irenderer.h"

#include "types.h"

namespace wavevis2 {


struct FixedDownSampleBuffer
{
  elements::timeid_t starttime; 
  elements::timeid_t endtime; 

  std::vector<float> mins; 
  std::vector<float> maxs; 
  std::vector<float> times; 
  std::vector<bool> present; 
}; 

typedef boost::shared_ptr<FixedDownSampleBuffer>  pFixedDownSampleBuffer_t; 

typedef int64_t bufferid_t; 

class FixedDownSample : public IRenderer
{
  /*
    FixedDownsample takes the input data, downsamples
    it to a fixed granularity. 

   */ 
public:

  FixedDownSample(timeid_t binsize); 
  ~FixedDownSample(); 

  void renderStream(timeid_t t1, timeid_t t2, int pixels); 
  void newDataWindow(const elements::datawindow_t<pWaveBuffer_t> & ); 

  void newSample(pWaveBuffer_t ); 
  void reset(); 

  static std::pair<bufferid_t, unsigned int> 
  getBinAndPos(timeid_t time, timeid_t binsize, 
	       timeid_t bufferlen) ; 

private:
  const timeid_t BINSIZE_; 
  static const timeid_t BUFFERLEN = 1000; 
  const timeid_t BUFFERDUR_; 


  bufferid_t computeBin(timeid_t times); 

  pFixedDownSampleBuffer_t createNewBuffer(bufferid_t bucket); 


  typedef std::map<bufferid_t, pFixedDownSampleBuffer_t> datamap_t; 
  datamap_t data_; 
  typedef std::set<timeid_t> seentimes_t; 
  seentimes_t seentimes_; 

}; 



}


#endif 
