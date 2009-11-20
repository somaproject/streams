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
  or always-true data.

  We have two places for our output data, data_ and preload_data. 

  A change of preload causes a complete invalidation of the entire
  stack, going forward. 

  NosieWave2 has a process method that gets called periodically
  on a glib timeout to push data into the output queue. 

 */ 


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
  
  enum NoiseClass {WhiteNoise, NoisySine, SquareWave, BiModal, Chirp, CarrierSine}; 
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

  elements::datawindow_t<pWaveBuffer_t> get_src_data(const elements::timewindow_t & tw); 
  size_t get_sequence(); 


private:
  
  elements::SourcePad<pWaveBuffer_t>::pSourcePad_t pSourcePad_; 
  
  elements::timeid_t lasttime_; 

  typedef std::map<elements::timeid_t, pWaveBuffer_t> datamap_t; 
  
  elements::timeid_t remaining_preload_pos_; 
  
  datamap_t preload_data_; 
  datamap_t data_; 

  std::pair<elements::timeid_t, pWaveBuffer_t> 
  createDataBuffer(elements::timeid_t starttime, elements::timeid_t endtime ); 

  void create_outstanding_preload_data(); 
  void create_new_data(elements::timeid_t tid);
  
  size_t seqid_; 


  void copy_data_map_range_to_output(datamap_t & dm, 
				     timeid_t start, timeid_t end,
				     elements::datawindow_t<pWaveBuffer_t> & wb); 
  
};

typedef boost::shared_ptr<NoiseWave2> pNoiseWave2_t; 

#endif // FAKEWAVE_H
