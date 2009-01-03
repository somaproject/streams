#include <iostream>
#include <sys/time.h>
#include <time.h>
#include "timer.h"




Timer::Timer(somatime_t starttime) :
  startSomaTime_(starttime)
{
  timeval starttimeval; 
  gettimeofday(&starttimeval, NULL) ;
  
  startWallTime_ = starttimeval.tv_sec * 1000000;   
  
  currentStreamTime_ = 0.0; 
  currentSomaTime_ = 0; 
  expTimeOffset_ = 0.0; 

}

bool Timer::dummytimeout() 
{
  updateSomaTime(currentSomaTime_ + 5000); 
  return true; 
}


void Timer::updateSomaTime(somatime_t newsomatime) 
{
  // set the new soma time, compute the other times, 
  // and perform the necessary signalling
  currentSomaTime_ = newsomatime; 
  currentWallTime_ = somaTimeToWallTime(currentSomaTime_); 
  currentStreamTime_ = somaTimeToStreamTime(currentSomaTime_); 
  currentExpTime_ = somaTimeToExpTime(currentSomaTime_); 
  streamTimeSignal_.emit(currentStreamTime_); 

}


walltime_t Timer::somaTimeToWallTime(somatime_t st)
{
  return startWallTime_ + walltime_t(double(st - startSomaTime_) / SOMATIME_PER_SEC); 

}

streamtime_t Timer::somaTimeToStreamTime(somatime_t st)
{
  return double(st - startSomaTime_) / SOMATIME_PER_SEC; 

}

streamtime_t Timer::somaTimeToExpTime(somatime_t st)
{
  return somaTimeToWallTime(st) + expTimeOffset_; 
}

streamtime_t Timer::getStreamTime()
{
  return currentStreamTime_; 

}

Timer::~Timer()
{


}
