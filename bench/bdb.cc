#include <iostream>
#include <db_cxx.h>
#include <string.h>
#include <boost/timer.hpp>
#include "boost/filesystem.hpp"
#include <sys/time.h>
#include <time.h>

const size_t benchbuf_n = 128; 
int compare_double(Db *db, const Dbt *a, const Dbt *b)
{
  // Returns:
  // < 0 if a < b
  // = 0 if a = b
  // > 0 if a > b
  double ad; 
  double bd; 
  memcpy(&ad, a->get_data(), sizeof(a));
  memcpy(&bd, b->get_data(), sizeof(b));
  
  if (ad < bd) return -1; 
  if (ad > bd) return 1;
  return 0; 
}



struct benchbuff
{
  float x[benchbuf_n]; 
  float t[benchbuf_n]; 
}; 

void create_canonical(benchbuff * bb, size_t index)
{
  for(int i = 0; i < benchbuf_n; i++) {
    bb->x[i] = index + i; 
    bb->t[i] = index * 100 + i; 
  }
}

int main()
{

  size_t ITERS = 100000; 

  Db db(NULL, 0);               // Instantiate the Db object
  u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE; // Open flags;
 
  std::string dbname("my_db.db"); 
  boost::filesystem::remove_all(dbname );
  
  try {
    // Open the database
    db.set_pagesize(1<<16); 
    db.set_bt_compare(compare_double);
    db.open(NULL,                // Transaction pointer
            dbname.c_str(),          // Database file name
            NULL,                // Optional logical database name
            DB_BTREE,            // Database access method
            oFlags,              // Open flags
            0);                  // File mode (using defaults)
    // DbException is not subclassed from std::exception, so
    // need to catch both of these.

    std::cout << "Starting timer" << std::endl; 
    //boost::timer timer; 
    timeval t1, t2; 
    gettimeofday(&t1, NULL); 
    
    for (int i = 0; i < ITERS; i++) { 
      double starttime =  - double(i) / 10.; // non-integer values
      
      benchbuff bufdata; 
      create_canonical(&bufdata, i); 
      
      Dbt key(&starttime, sizeof(starttime));
      Dbt data(&bufdata, sizeof(bufdata)); 
      
      int ret = db.put(NULL, &key, &data, DB_NOOVERWRITE);
      if (ret == DB_KEYEXIST) {
	db.err(ret, "Put failed because key %f already exists", starttime);
      }
    }
    
    Dbc *cursorp;
    db.cursor(NULL, &cursorp, 0); 
    double searchval = -20.0; 
    Dbt search_key(&searchval, sizeof(searchval)); 
    Dbt found_data; 

    int ret = cursorp->get(&search_key, &found_data, DB_SET_RANGE);
    int maxfind = 10; 
    while (ret != DB_NOTFOUND) {
      std::cout << "key: " << *((double*)search_key.get_data()) << std::endl; 
      ret = cursorp->get(&search_key, &found_data, DB_NEXT);
      maxfind--; 
      if (maxfind ==0) {
	break; 
      }
    }
    


    db.close(DB_NOSYNC); 
    gettimeofday(&t2, NULL); 

    //double elapsed = timer.elapsed(); 
    double elapsed = (t2.tv_sec - t1.tv_sec); 

    std::cout << "puts took " << elapsed << " seconds" << std::endl; 
    std::cout << ITERS / elapsed << " records/sec" << std::endl; 
    std::cout << (ITERS*sizeof(benchbuff)) /1e6 / elapsed << " MB/sec" << std::endl; 
    
    
  } catch(DbException &e) {
    // Error handling code goes here
  } catch(std::exception &e) {
    // Error handling code goes here
  }
  
}
