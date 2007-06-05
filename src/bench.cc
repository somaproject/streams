#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std; 
using namespace boost::posix_time;

struct GLWavePoint_t
{
  float t; 
  float x; 
}; 

const int N = 43000000; 
int main(void)
{
  // This is an attempt to benchmark various computationally-intensive
  // bits of our oscope-like app, including triggering and waveform
  // averaging.
  std::cout << "Number of data points: " << N << std::endl; 
  std::cout << "Duration (hours) at 2ksps: " << N / 2000.0 / (60*60) 
	    << std::endl; 

  // generate fake data

  std::vector<GLWavePoint_t> wp(N); 

  // populate it with random data
  for (int i = 0; i < N; i++)
    {
      wp[i].t = i; 
      wp[i].x = (float(rand())/( RAND_MAX + 1.0))*2.0 -1.0; 
    }

  ptime t1 =  microsec_clock::local_time(); 
  
  std::vector<int> triggerPoints; 
  triggerPoints.reserve(N); 
  float triggerVal = 0.0; 
  // find triggers
  int s = N/4; 
  for (int i = 1; i < N/4; i++)
    {
      if (wp[i-1].x  < triggerVal and wp[i].x >= triggerVal)
	{
	  triggerPoints.push_back(i); 
	}
    }
  
  ptime t2 =  microsec_clock::local_time(); 
  
  // now, we average the trigger points
  float sum = 0.0; 
  for (std::vector<int>::iterator it = triggerPoints.begin(); 
       it !=  triggerPoints.end(); it++)
    {
      sum += wp[*it].x; 
    }

  ptime t3=  microsec_clock::local_time(); 

  std::cout << "Trigger detection took " 
	    << to_simple_string(t2 - t1) << std::endl; 
  std::cout << "There were " << triggerPoints.size() << " trigger points"
	    << std::endl; 
  std::cout << "summing took took " 
	    << to_simple_string(t3 - t2) << std::endl; 

  std::cout << "the sum was" << sum << std::endl; 

}
