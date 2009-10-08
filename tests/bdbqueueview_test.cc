#include <iostream>
#include <db_cxx.h>
#include <string.h>
#include <boost/timer.hpp>
#include "boost/filesystem.hpp"
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "bdbqueueview.h"

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;


BOOST_AUTO_TEST_SUITE(BDB_queue_view);

const size_t benchbuf_n = 128; 


struct benchbuff
{
  std::vector<float> x;
  std::vector<float> t; 

};

struct benchbuff_disk
{
  float x[benchbuf_n]; 
  float t[benchbuf_n]; 
  void copy(benchbuff * bb) {
    bb->x.resize(benchbuf_n); 
    bb->t.resize(benchbuf_n); 
    for (int i = 0; i < benchbuf_n; i++) {
      bb->x[i] = x[i]; 
      bb->t[i] = t[i]; 
    }
  }
}; 

void create_canonical(benchbuff_disk * bb, size_t index)
{
  
  for(int i = 0; i < benchbuf_n; i++) {
    bb->x[i] = index + i; 
    bb->t[i] = index * 100 + i; 
  }
}

BOOST_AUTO_TEST_CASE(simpletest) {


  size_t ITERS = 100; 

  Db db(NULL, 0);               // Instantiate the Db object
  u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE; // Open flags;
 
  std::string dbname("test.db"); 
  boost::filesystem::remove_all(dbname );
  
  try {
    // Open the database
    db.set_pagesize(1<<16); 
    db.set_flags(DB_INORDER); 
    db.set_re_len(sizeof(benchbuff_disk)); 
    db.open(NULL,                // Transaction pointer
            dbname.c_str(),          // Database file name
            NULL,                // Optional logical database name
            DB_QUEUE,            // Database access method
            oFlags,              // Open flags
            0);                  // File mode (using defaults)
    // DbException is not subclassed from std::exception, so
    // need to catch both of these.

    BDBQueueView<benchbuff, benchbuff_disk> qv1(&db); 
    BDBQueueView<benchbuff, benchbuff_disk> qv2(&db); 

    BOOST_CHECK_EQUAL(qv1.empty(), true); 

    db_recno_t rid; 
    
    benchbuff_disk bufdata; 
    create_canonical(&bufdata, 0); 
      
    Dbt key(&rid, sizeof(rid));
    Dbt data(&bufdata, sizeof(bufdata)); 
      
    int ret = db.put(NULL, &key, &data,  DB_APPEND);
    std::cout << "put value" << *((db_recno_t*)key.get_data())
	      << std::endl; 
    BOOST_CHECK_EQUAL(qv1.empty(), false); 
    BOOST_CHECK_EQUAL(qv2.empty(), false); 
    
    BOOST_CHECK_EQUAL(qv1.front().x[5], bufdata.x[5]);
    
    for (int j = 1; j < 10; j++) {
      benchbuff_disk bufdata; 
      create_canonical(&bufdata, j); 
      
      Dbt key(&rid, sizeof(rid));
      Dbt data(&bufdata, sizeof(bufdata)); 
      
      int ret = db.put(NULL, &key, &data,  DB_APPEND);
    }

    int pos = 0; 
    while (!qv1.empty() ) { 
      benchbuff_disk canonical; 
      create_canonical(&canonical, pos); 
      BOOST_CHECK_EQUAL(qv1.front().x[5], canonical.x[5]);
      BOOST_CHECK_EQUAL(qv1.front().x[20], canonical.x[20]);
      BOOST_CHECK_EQUAL(qv1.front().x[50], canonical.x[50]);
      qv1.pop(); 
      pos++; 
    }

    BOOST_CHECK_EQUAL(qv1.empty(), true); 

    qv1.reset(); 
    pos = 0; 
    while (!qv1.empty() ) { 
      benchbuff_disk canonical; 
      create_canonical(&canonical, pos); 
      BOOST_CHECK_EQUAL(qv1.front().x[5], canonical.x[5]);
      BOOST_CHECK_EQUAL(qv1.front().x[20], canonical.x[20]);
      BOOST_CHECK_EQUAL(qv1.front().x[50], canonical.x[50]);
      qv1.pop(); 
      pos++; 
    }


    db.close(DB_NOSYNC); 

    
  } catch(DbException &e) {
    // Error handling code goes here
  } catch(std::exception &e) {
    // Error handling code goes here
  }
  
}


BOOST_AUTO_TEST_SUITE_END(); 
