#include <iostream>

#include <iostream>
#include <string.h>
#include <boost/timer.hpp>
#include "boost/filesystem.hpp"
#include <sys/time.h>
#include <time.h>
#include <boost/thread/thread.hpp>



#include "timedb.h"

const size_t benchbuf_n = 128; 


using namespace datastore; 

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

typedef TimeSeriesDataBase<benchbuff> db_t; 

class Writer
{
public:
  db_t & db_; 
  uint64_t iters_; 
  uint64_t pos_; 
  bool running_; 
  boost::thread * thread_; 

  Writer(db_t & db, int counts) :
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
      if (time == 0) { 
	db_.newSequence(time, bufdata); 
      } else {
	db_.append(time, bufdata); 
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
  db_t & db_; 
  bool running_;
  int iters_;
  boost::thread * thread_; 
  Reader(db_t & db, int iters) : 
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

    TimeCursor<benchbuff> tc = db_.createCursor(0); 
    while(not tc.valid() ) {
      tc = db_.createCursor(0); 
//       std::cout << "waiting for valid cursor" << std::endl;
    }
    int count = 0; 
    benchbuff x; 
    while (count < iters_) {
//       std::cout << " count = " << count << std::endl;
      timeid_t t = tc.getCurrentValue(&x); 
      count++; 
      while((tc.next() == CURSOR_RET_NO_NEXT) and (count < iters_) ) {
	// spin!
      }
    }
    
    running_ = false; 
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
  db_t db; 

  size_t ITERS = 1000000; 

  Writer w(db, ITERS); 
  Reader r1(db, ITERS); 
  Reader r2(db, ITERS); 
  
  timeval t1, t2; 
  gettimeofday(&t1, NULL); 
  w.run(); 
  r1.run(); 
  r2.run(); 

  w.stop(); 
  r1.stop();
  r2.stop();
  
  gettimeofday(&t2, NULL); 
  
  //double elapsed = timer.elapsed(); 
  double elapsed = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec)/1e6; 

  std::cout << "puts took " << elapsed << " seconds" << std::endl; 
  std::cout << ITERS / elapsed << " records/sec" << std::endl; 
  std::cout << (ITERS*sizeof(benchbuff)) /1e6 / elapsed << " MB/sec" << std::endl; 
  
}
