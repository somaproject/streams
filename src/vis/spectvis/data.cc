#include "vis/spectvis/data.h"
#include <string.h>
// SpectRenderBlock::SpectRenderBlock(int kin, int Nin) :
//   k(kin),
//   N(Nin)
// {
//   if (k*N > MAXLEN) {
//     throw std::runtime_error("Spect render block too big"); 
//   }

//   data = new spectgram_t(boost::extents[k][N]); 
  
// }

// SpectRenderBlock::~SpectRenderBlock()
// {
//   delete data; 
// }

// size_t const SpectRenderBlock::maxbytes()
// {
//   return sizeof(SpectRenderBlock) + MAXLEN; 

// }


// int SpectRenderBlock::marshall_to_buffer(void * dest)  const
// {
//   spectgram_t * ptr = data; 
//   // zero our pointer for serialization
//   //data = NULL; 

//   char * p = (char *)dest; 
//   memcpy(p, this, sizeof(SpectRenderBlock)); 
//   p += sizeof(SpectRenderBlock); 

//   // restore our pointer
//   //data = ptr; 
  
//   size_t databytes = sizeof(float) * k * N; 
//   memcpy(p, data->origin(), databytes); 
  
//   return sizeof(SpectRenderBlock) + databytes;   

// }


// void SpectRenderBlock::unmarshall_from_buffer(SpectRenderBlock * srb, 
// 					      const void * buffer)
// {
//   const char * p = (char *) buffer; 
//   // first, delete the current data
//   delete srb->data; 
//   // copy the header
//   memcpy(srb,p, sizeof(SpectRenderBlock)); 
  
//   p += sizeof(SpectRenderBlock); 
  
//   srb->data = new spectgram_t(boost::extents[srb->k][srb->N]); 
  
//   memcpy(srb->data->origin(), p, srb->k * srb->N * sizeof(float)); 
  
  
// }
