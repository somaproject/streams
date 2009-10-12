#ifndef __FFTW_OP_H__
#define __FFTW_OP_H__
#include <fftw3.h>
#include "types.h"
#include "fftengine.h"

namespace spectvis  {

class FFTW : public IFFTop
{
  
public:
  FFTW(int N, int MAX_DATA); 
  pFFT_t operator() (float * data, int data_size, int N, float fs) ;
  ~FFTW(); 
  
private:
  int N_; 
  int MAX_DATA_; 

  fftw_complex *out;
  double * in; 

  fftw_plan p;
};

}
#endif 

