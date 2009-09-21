#ifndef WAVEVIS_H
#define WAVEVIS_H

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>

#include "streamvis.h" 

#include "waves.h"
#include "wavevistrigger.h"
#include "wavevisrenderer.h"
#include "wavevistriggerrenderer.h"

class WaveVis; 

typedef boost::shared_ptr<WaveVis> pWaveVis_t; 

class WaveVis : public StreamVis<WaveBuffer_t>
{
 public: 
  WaveVis(); 
  ~WaveVis(); 

  // The generic StreamVis Interface :
  
  void drawMainWave(wavetime_t t1, wavetime_t t2, int pixels);
  void drawTriggerWave(wavetime_t tbefore, wavetime_t tafter, wavetime_t timepoint); 

  invWaveSignal_t  & invMainWaveSignal()
    { return streamRenderer_.invWaveSignal(); 
    }

  invWaveSignal_t & invTriggerWaveSignal()
    { return streamTriggerRenderer_.invWaveSignal(); 
    }

  // the trigger generator interface
 
  updateTriggersSignal_t & updateTriggersSignal(); 
  QueueView<wavetime_t> getTriggerQueueView(); 
  

  // trigger inputs
  void updateTriggers(bool reset); 

  void setTriggerSource(const QueueView<float> & tqv);

  void setTriggerValue(float); 
  void enableTrigger(bool value); 

  // properties to control parameters:

  void setVerticalScale(float);  
  float getVerticalScale(); 
  sigc::signal<void, float> & verticalScaleSignal() 
    {
      return verticalScaleSignal_; 
    }

  
  
  // pixel render offiset
  void setYOffset(float); 
  float getYOffset(); 
  void setYHeight(float); 
  
  void setColor(Gdk::Color c); 
  Gdk::Color getColor(); 
  sigc::signal<void, Gdk::Color> & colorSignal() 
    {
      return colorSignal_; 
    }
  
  
  
 private:
  void newData(); 
  void invalidateData(); 

  WaveVisRenderer streamRenderer_; 
  WaveVisTrigger  streamTrigger_; 
  WaveVisTriggerRenderer streamTriggerRenderer_; 

  std::vector<GLWavePoint_t> filteredSamples_; 
  
  float yoffset_; 
  float yheight_; 

  float verticalScale_; 
  sigc::signal<void, float> verticalScaleSignal_; 

  Gdk::Color color_; 
  sigc::signal<void, Gdk::Color> colorSignal_; 

  GLWavePoint_t filterNextPoint(GLWavePoint_t wp);

};

#endif // WAVESTREAMVIS_H
