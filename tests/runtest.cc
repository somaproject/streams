#include <iostream>
#include <db_cxx.h>
#include <string.h>
#include <boost/timer.hpp>
#include "boost/filesystem.hpp"
#include <sys/time.h>
#include <time.h>
#include <iostream>

#include "bdbqueueview.h"
#include "mocksomanetcodec.h"
#include "networkdatacache.h"
#include "timer.h"

int main() { 
  pMockSomaNetCodec_t snc(new MockSomaNetCodec()); 
  
  pTimer_t timer = Timer::createNetworkTimer(snc, 0); 
  
  bf::path scratchdir("/tmp"); 
  NetworkDataCache ndc(snc, timer, scratchdir); 
  
  
  core::IQueueView<WaveBuffer_t >::ptr raw0 = ndc.getNetRawSource(1); 
  
  std::cout << "the returned ptr is " << raw0 << std::endl;
  std::cout << "raw0->front() = " << &(raw0->front()) << std::endl;
  for(int i = 0; i < RAWBUF_LEN; i++) {
    WaveBuffer_t wb = raw0->front(); 
    //std::cout << i << " " <<  &(raw0->data[i]) << std::endl; 
    std::cout << i << " " <<  wb.data[i] << std::endl; 
    //std::cout << i << " " <<  raw0->front().data[i] << std::endl; 
    //std::cout << i << " " <<  raw0->front().data[i] << std::endl; 
  }

  std::cout << "Done" << std::endl;
}
