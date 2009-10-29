#ifndef PAMONITOR_H
#define PAMONITOR_H

#include <vector>
#include <sigc++/sigc++.h>
#include <boost/shared_ptr.hpp>
#include <data/wave.h>
#include <core/element.h>
#include <core/sourcepad.h>
#include <properties.h>

#include <sources/sourcebase.h>
#include "pasource.h"

class PulseAudioMonitorWave : public SourceBase
{
public:
  static const std::string TYPENAME;
  // this is just a prototype source
  PulseAudioMonitorWave(std::string, bf::path); 
  
  ~PulseAudioMonitorWave(); 
  
  void process(elements::timeid_t currenttime); 

  void accept(pISourceVisitor_t sv) { 
    if (sv != NULL) 
      sv->visit(this); 
  }
  
  elements::datawindow_t<pWaveBuffer_t> get_src_data(const elements::timewindow_t & tw); 

  size_t get_sequence() {
    return 0; // only ever one sequence. 
  }


private:

  elements::SourcePad<pWaveBuffer_t>::pSourcePad_t pSourcePad_; 

  //boost::ptr_list<WaveBuffer_t> dataList_; 
  double fs_;

  //  pTimer_t pTimer_; 
  void nextData(const float * f, unsigned l); 
  
//   void timeUpdate(streamtime_t t); 
//   streamtime_t lastTime_; 
  PASource pasource_; 
  boost::posix_time::ptime start_time; 
  size_t samppos_; 

  boost::shared_mutex data_mutex_;
  typedef std::map<timeid_t, pWaveBuffer_t> datamap_t; 
  datamap_t data_; 
  
  pWaveBuffer_t current_pwb_; 

};

typedef boost::shared_ptr<PulseAudioMonitorWave> pPulseAudioMonitorWave_t; 

#endif // FAKEWAVE_H
