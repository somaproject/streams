#ifndef __SOURCES_NOISEWAVE2_H__
#define __SOURCES_NOISEWAVE2_H__

#include <vector>
#include <stack>
#include <sigc++/sigc++.h>
#include <boost/shared_ptr.hpp>
#include <data/wave.h>
#include <boost/filesystem.hpp>

#include <elements/property.h>
#include <elements/element.h>
#include <sources/sourcebase.h>

namespace bf = boost::filesystem; 

class NoiseWave2 : public SourceBase
{
public:
  static const std::string TYPENAME;

  typedef elements::timeid_t timeid_t; 

  // this is just a prototype source
  NoiseWave2(std::string, bf::path); 
  
  ~NoiseWave2(); 
  
  // emit updatedData
  elements::Property<float> amplitude; 
  
  enum NoiseClass {WhiteNoise, NoisySine, SquareWave, BiModal}; 
  elements::Property<NoiseClass> noiseclass; 

  elements::Property<int> preload; 

  elements::Property<elements::timewindow_t> activetime; 

  void process(elements::timeid_t currenttime); 
  
  void accept(pISourceVisitor_t sv) { 
    if (sv != NULL) 
      sv->visit(this); 
  }


private:
  typedef boost::shared_ptr<WaveBuffer_t> pWaveBuffer_t; 
  
  elements::SourcePad<WaveBuffer_t>::pSourcePad_t pSourcePad_; 
  
  elements::timeid_t lasttime_; 
  float FS_; 

  class SentBufferWrapper {
  public:
    inline SentBufferWrapper(pWaveBuffer_t data) : 
      data_(data),
      sent_(false)
    {
    }

    inline bool sent() { 
      return sent_; 
    }

    inline void set_sent(bool s) { 
      sent_ = s; 
    }
    
    inline pWaveBuffer_t data() { 
      return data_; 
    }

  private:
    pWaveBuffer_t data_;     
    bool sent_; 
  }; 
  typedef boost::shared_ptr<SentBufferWrapper> pSentBufferWrapper_t; 

  
  typedef std::map<elements::timeid_t, pSentBufferWrapper_t> datamap_t; 

  elements::timeid_t remaining_preload_pos_; 
  
  datamap_t preload_data_; 
  std::stack<datamap_t::iterator> preload_iters_;

  datamap_t data_; 
  std::stack<datamap_t::iterator> data_iters_; 

  void reset_preload_data(); 
  void reset_sent_flags(); 
  void send_reset_token(); 


  std::pair<elements::timeid_t, pWaveBuffer_t> 
  createDataBuffer(elements::timeid_t starttime, elements::timeid_t endtime ); 

  void create_outstanding_preload_data();
  void create_new_data(elements::timeid_t tid);
  
  void send_preload(); 
  void send_data(); 
};

typedef boost::shared_ptr<NoiseWave2> pNoiseWave2_t; 

#endif // FAKEWAVE_H
