#include "wavestreamsourcestatus.h"

WaveStreamSourceStatus::WaveStreamSourceStatus(streamSourcePtr_t wsv) :
  pStreamSource_(wsv), 
  label_("This is text")
{
  add(hbox_); 

  hbox_.pack_start(label_); 

}



WaveStreamSourceStatus::~WaveStreamSourceStatus()
{

}
