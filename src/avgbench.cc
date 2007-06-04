#include <iostream>
#include <vector>

using namespace std; 

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

  // generate fake data

  std::vector<GLWavePoint_t> wp(N); 

  
  

}
