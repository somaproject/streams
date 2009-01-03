#include "waveidentity.h"
#include "sourcebase.h" 

using namespace soma::streams; 

const std::string WaveIdentity::NAME = "Wave Identity Filter"; 


WaveIdentity::WaveIdentity() 
{
    
  
}


void WaveIdentity::connect(SourceBase<WaveBuffer_t>  & source) 
{

  source.newDataSignal().connect(sigc::mem_fun(*this, 
					       &WaveIdentity::newData)); 

  source.invalidateDataSignal().connect(sigc::mem_fun(*this, 
					       &WaveIdentity::invalidateData)); 
 
  sourceQueueView_ = source.getQueueView(); 

}


void WaveIdentity::newData()
{
  while(! sourceQueueView_.empty()) {
    data_.push_back(sourceQueueView_.front()); 
    sourceQueueView_.pop(); 
  }
  
  newDataSignal_.emit(); 

}

void WaveIdentity::invalidateData()
{
  // oh nos! 
  data_.clear(); 
  while(sourceQueueView_.empty()) {
    data_.push_back(sourceQueueView_.front()); 
    sourceQueueView_.pop(); 
  }
  invalidateDataSignal_.emit(); 
}
