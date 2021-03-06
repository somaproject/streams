#include <iostream>
#include <db_cxx.h>
#include <string.h>
#include <boost/timer.hpp>
#include "boost/filesystem.hpp"
#include <sys/time.h>
#include <time.h>
#include <boost/thread/thread.hpp>

typedef uint64_t timeid_t; 
const size_t benchbuf_n = 128; 

int compare_timeid(Db *db, const Dbt *a, const Dbt *b)
{
  // Returns:
  // < 0 if a < b
  // = 0 if a = b
  // > 0 if a > b
  timeid_t ad; 
  timeid_t bd; 
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

class Writer
{
public:
  Db * db_; 
  uint64_t iters_; 
  uint64_t pos_; 
  bool running_; 
  boost::thread * thread_; 

  Writer(Db * db, int counts) :
    db_(db), 
    iters_(counts), 
    pos_(0), 
    running_(false),
    thread_(NULL)
  { 
    

  }

  void run() {
    running_ = true; 

    thread_ = new boost::thread(boost::bind(&Writer::worker, this)); 

  }

  void worker() {
    timeid_t time = 0; 
    benchbuff bufdata; 
    while((time < iters_)) { 
      create_canonical(&bufdata, time); 
      
      Dbt key(&time, sizeof(time));
      Dbt data(&bufdata, sizeof(bufdata)); 
      Dbc * cursorp; 
      db_->cursor(NULL, &cursorp,  DB_WRITECURSOR  ); 
      int ret = cursorp->put(&key, &data, DB_KEYFIRST); 
      cursorp->close(); 

      //      std::cout << "putting key = " << time << std::endl;
      //int ret = db_->put(NULL, &key, &data, DB_NOOVERWRITE);
      if (ret != 0) {
	std::cout << "HOLY CRAP PUT ERROR?" << std::endl; 
      } else { 
	//std::cout << "Successful put" << std::endl;
      }
      time++; 
      pos_++; 

      // now signal for new data? 
    }
    running_ = false; 
  }

  void stop() {
    running_ = false; 
    if (thread_) 
      thread_->join(); 
    delete thread_; 
  }

}; 

class Reader
{
public:
  Db * db_; 
  bool running_;
  int iters_;
  boost::thread * thread_; 
  Reader(Db * db, int iters) : 
    db_(db),
    running_(false),
    iters_(iters),
    thread_(NULL)
  {

  }

  void run() 
  {
    running_ = true; 
    thread_ = new boost::thread(boost::bind(&Reader::worker, this)); 
    
  }
  
  void worker() 
  {
    Dbc *cursorp;
    int count = 0; 

    timeid_t searchval = 0; 
    Dbt search_key(&searchval, sizeof(searchval)); 
    Dbt found_data; 


    timeid_t lastgood = 0;

    bool backedup = false; 

//     std::cout << "Reader starting " << std::endl;
    int ret =0; 
    while (count == 0) { 	// first one
      db_->cursor(NULL, &cursorp, 0); 
      ret = cursorp->get(&search_key, &found_data, DB_FIRST);
      if (ret == 0) {
	lastgood = *((timeid_t*)search_key.get_data()); 
	cursorp->close(); 

	count = 1; 
      } else {
	cursorp->close(); 
	sleep(0.1); // should actually synchronize! 

      }
    }

    bool cursoropen = false; 
    while(count < iters_) {
      if (!cursoropen) { 
	db_->cursor(NULL, &cursorp, 0); 
	cursoropen = true; 
	ret = cursorp->get(&search_key, &found_data, DB_GET_BOTH);
      }
      ret = cursorp->get(&search_key, &found_data, DB_NEXT);
      if (ret == DB_NOTFOUND) { 
	cursorp->close(); 
	cursoropen = false; 
      } else {
	time_t readerid =  *((timeid_t*)search_key.get_data()); 

	//assert(readerid == count); 
	lastgood = readerid; 
	
	count += 1; 
      }

    }
    if (cursoropen) { 
      cursorp->close(); 
    }

  }
  
  void stop() 
  {
    running_ = false; 
    if (thread_) 
      thread_->join(); 
    delete thread_; 

  }
  

}; 

int main()
{

  size_t ITERS = 100000; 
  u_int32_t env_flags = DB_CREATE |     // If the environment does not
    // exist, create it.
    DB_INIT_MPOOL |
    DB_INIT_CDB |
    DB_THREAD ; // Initialize the in-memory cache.

  std::string envHome = "/tmp/benchtest"; 
  boost::filesystem::remove_all(envHome); 
  boost::filesystem::create_directories(envHome);
  
  DbEnv myEnv(0); 

  try {
    myEnv.open(envHome.c_str(), env_flags, 0);
  } catch(DbException &e) {
    std::cerr << "Error opening database environment: "
              << envHome << std::endl;
    std::cerr << e.what() << std::endl;
    exit( -1 );
  } catch(std::exception &e) {
    std::cerr << "Error opening database environment: "
              << envHome << std::endl;
    std::cerr << e.what() << std::endl;
    exit( -1 );
  } 


  Db db(&myEnv, 0);               // Instantiate the Db object
  u_int32_t oFlags = DB_CREATE |   DB_THREAD; // Open flags;
 
  std::string dbname("my_db.db"); 
  boost::filesystem::remove_all(dbname );
  
  try {
    // Open the database
    db.set_pagesize(1<<16); 
    db.set_bt_compare(compare_timeid);
    db.open(NULL,                // Transaction pointer
            dbname.c_str(),          // Database file name
            NULL,                // Optional logical database name
            DB_BTREE,            // Database access method
            oFlags,              // Open flags
            0);                  // File mode (using defaults)
    // DbException is not subclassed from std::exception, so
    // need to catch both of these.


    Writer w(&db, ITERS); 
    Reader r1(&db, ITERS); 
    Reader r2(&db, ITERS); 

    //    std::cout << "Starting timer" << std::endl; 
    //boost::timer timer; 
    timeval t1, t2; 
    gettimeofday(&t1, NULL); 
    w.run(); 
    r1.run(); 
    r2.run(); 
    while(!w.running_) {
      // spin while the read starts up
    }

    while(w.running_) { 
      sleep(1); 
    }
    w.stop(); 
    r1.stop();
    r2.stop();

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
