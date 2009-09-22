#include <iostream>
#include <db_cxx.h>
#include <string.h>
#include <boost/timer.hpp>
#include "boost/filesystem.hpp"
#include <sys/time.h>
#include <time.h>

const size_t benchbuf_n = 128; 

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

  size_t ITERS = 100; 

  Db db(NULL, 0);               // Instantiate the Db object
  u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE; // Open flags;
 
  std::string dbname("QUEUE.db"); 
  boost::filesystem::remove_all(dbname );
  
  try {
    // Open the database
    db.set_pagesize(1<<16); 
    db.set_flags(DB_INORDER); 
    db.set_re_len(sizeof(benchbuff)); 
    db.open(NULL,                // Transaction pointer
            dbname.c_str(),          // Database file name
            NULL,                // Optional logical database name
            DB_QUEUE,            // Database access method
            oFlags,              // Open flags
            0);                  // File mode (using defaults)
    // DbException is not subclassed from std::exception, so
    // need to catch both of these.

    Dbc *cursorp;
    db.cursor(NULL, &cursorp, 0); 
    Dbt found_data; 
    
    Dbt search_key; 
    std::cout << "Initial cursor get" << std::endl; 
    int ret = cursorp->get(&search_key, &found_data, DB_CURRENT);

    assert(ret == DB_NOTFOUND); 

    //boost::timer timer; 
    std::cout << "now adding data" << std::endl; 
    
    for (int i = 0; i < ITERS; i++) { 
      db_recno_t rid; 

      benchbuff bufdata; 
      create_canonical(&bufdata, i); 
      
      Dbt key(&rid, sizeof(rid));
      Dbt data(&bufdata, sizeof(bufdata)); 
      
      int ret = db.put(NULL, &key, &data,  DB_APPEND);
    }
    
    
    ret = cursorp->get(&search_key, &found_data, DB_CURRENT);
    std::cout << "key: " << *((db_recno_t*)search_key.get_data()) << std::endl; 
        

    db.close(DB_NOSYNC); 

    
    
  } catch(DbException &e) {
    // Error handling code goes here
  } catch(std::exception &e) {
    // Error handling code goes here
  }
  
}
