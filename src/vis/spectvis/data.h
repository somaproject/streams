#ifndef __SPECTVIS_DATA_H__
#define __SPECTVIS_DATA_H__

#include <boost/multi_array.hpp>
#include <db_cxx.h>
#include <string.h>

#include <data/wave.h>

typedef boost::multi_array<float, 2> spectgram_t;

typedef boost::shared_ptr<Db> pDb_t; 

// inline int spectvis_compare_double(Db *db, const Dbt *a, const Dbt *b)
// {
//   // Returns:
//   // < 0 if a < b
//   // = 0 if a = b
//   // > 0 if a > b
//   double ad; 
//   double bd; 
//   memcpy(&ad, a->get_data(), sizeof(ad));
//   memcpy(&bd, b->get_data(), sizeof(bd));
  
//   if (ad < bd) return -1; 
//   if (ad > bd) return 1;
//   return 0; 
// }


// struct SpectRenderBlock
// {
  
//   static const size_t MAXLEN = 4096; // maximum size of the data region, 
//   // in samples

//   SpectRenderBlock(int k, int N); 
//   ~SpectRenderBlock(); 

//   streamtime_t starttime;
//   streamtime_t endtime; 
//   float lowfreq; 
//   float highfreq; 
//   int k; // # number of frequency bins
//   int N; // # number of time samples
  
//   spectgram_t  * data; 

//   static const size_t maxbytes(); 

//   int marshall_to_buffer(void * dest) const; 

//   static void unmarshall_from_buffer(SpectRenderBlock * , const void * data); 

//   //  friend bool operator== ( const SpectRenderBlock & lhs, const SpectRenderBlock & rhs );
// };

// bool operator== ( const SpectRenderBlock & lhs, const SpectRenderBlock & rhs ) 
// {
//     if (lhs.starttime != rhs.starttime)
//       return false; 
    
//     if (lhs.endtime != rhs.endtime)
//       return false; 
    
//     if (lhs.lowfreq != rhs.lowfreq)
//       return false; 
    
//     if (lhs.highfreq != rhs.highfreq)
//       return false; 
    
//     if (lhs.k != rhs.k)
//       return false; 
    
//     if (lhs.N != rhs.N)
//       return false; 
    
//     if (*(lhs.data) != *(rhs.data)) 
//       return false; 
    
//     return true; 

//   }


#endif
