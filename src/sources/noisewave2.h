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

/*
  NoiseWave is our generic test source element, and is capable
  of generating a variety of noise data sets. It is
  driven by process() to create new data, which is "canonical" 
  or always-true data. We can also 
  change the amount of "preloaded" data, and this new data
  is generated at preload-init time. 

  We have two places for our output data, data_ and preload_data. 

  A change of preload causes a complete invalidation of the entire
  stack, going forward. 

  There's the assumption that 

 */ 


class NoiseWave2 : public SourceBase, 
		   public elememnts::IElementSource<WaveBuffer_t>
{
public:
  static const std::string TYPENAME;
  
  typedef elements::timeid_t timeid_t; 

  // this is just a prototype source
  NoiseWave2(std::string, bf::path); 
  
  ~NoiseWave2(); 
  
  // emit updatedData
  elements::Property<float> amplitude; 
  
  enum NoiseClass {WhiteNoise, NoisySine, SquareWave, BiModal, Chirp}; 
  elements::Property<NoiseClass> noiseclass; 

  elements::Property<float> preload; // in minutes

  elements::Property<elements::timewindow_t> activetime; 

  elements::Property<float> samplingrate; 
  elements::Property<float> frequency; // frequency for parameters that involve a freq


  void process(elements::timeid_t currenttime); 
  
  void accept(pISourceVisitor_t sv) { 
    if (sv != NULL) 
      sv->visit(this); 
  }

  void get_src_data(std::list<DataWrapper<WaveBuffer_t> > &, padid_t id, 
		    const timewindow_t & tw); 


private:
  typedef boost::shared_ptr<WaveBuffer_t> pWaveBuffer_t; 
  
  elements::SourcePad<WaveBuffer_t>::pSourcePad_t pSourcePad_; 
  
  elements::timeid_t lasttime_; 

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
