#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include <vis/spectvis/spectvis.h>
#include <vis/spectvis/data.h>


BOOST_AUTO_TEST_SUITE(spectvis); 

BOOST_AUTO_TEST_CASE(datamarhsall)
{
  
  SpectRenderBlock sbb(128, 4); 
  (*sbb.data)[0][3] = 123.456; 
  
  char * buffer = (char*)malloc(sbb.maxbytes()); 
  sbb.marshall_to_buffer(buffer); 
  
  SpectRenderBlock sbb2(10, 20); 
  SpectRenderBlock::unmarshall_from_buffer(&sbb2, buffer); 
  
  free(buffer); 
  BOOST_CHECK_EQUAL(sbb.N, sbb2.N);
  BOOST_CHECK_EQUAL(sbb.k, sbb2.k);
  for (int i = 0; i < sbb.N; i++) {
    for (int k = 0; k < sbb.k; k++) {
      BOOST_CHECK_EQUAL(((*sbb.data)[k][i]),
			((*sbb2.data)[k][i])); 
    }
  }
}


BOOST_AUTO_TEST_SUITE_END(); 
