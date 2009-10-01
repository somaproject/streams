#ifndef __DATASTORE_H__
#define __DATASTORE_H__

#include <boost/ptr_container/ptr_map.hpp>

#include <boost/intrusive/list.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#include <exception>
#include <list>

namespace datastore
{

using namespace boost::intrusive;

typedef uint64_t timeid_t; 

// forward declaration
template<typename T>
class TimeCursor; 

template<typename T>
class cursor_impl; 



enum CURSOR_RET {
  CURSOR_RET_GOOD = 0, 
  CURSOR_RET_NO_NEXT = -1  
}; 



template<typename T>
class ListNode : 
    public list_base_hook<>
{
public:
  ListNode(timeid_t t, const T val) :
    time(t),
    value(val) 
  {
    
  }

  const timeid_t time; 
  const T value; 
}; 

template<typename T> 
class MonotonicConstantSequence : boost::noncopyable
{
public:
  typedef ListNode<T> nodeclass_t; 
  typedef list<nodeclass_t > seq_t;
  
  seq_t sequence; 

  boost::shared_mutex mutex; 

  bool valid; 


  timeid_t getHeadTime() { 
    // NOT THREAD SAFE
    return sequence.front().time;
  }
  
  timeid_t getTailTime()
  {
    return sequence.back().time; 
  }

  typedef typename seq_t::iterator si_t; 

  si_t search_leq(timeid_t t)
  {
    // FIXME: Linear time! ouch!
    // FIXME : NOT THREAD SAFE
    /*
      return an iterator pointing to the 
      largest element  at a time less-than-or-equal-to 
      t.       
      
    */
    si_t i = sequence.begin(); 
    
    si_t lastiter = sequence.end(); 
    for(i; i != sequence.end(); ++i) {
      if (i->time <= t) {
	lastiter = i; 
      }
    }

    return lastiter; 
  }

}; 

typedef size_t cursorid_t; 

template<typename T>
class TimeSeriesDataBase
{
private:
  // active cursors? it would REALLY SUCK if we had to use this

  typedef MonotonicConstantSequence<T> sequence_t; 
  // now the set of "active" lists
  typedef std::map<timeid_t, sequence_t *> sequenceptrmap_t; 
  
  typedef typename sequence_t::nodeclass_t nc_t; 
  typedef std::list<nc_t * > nodeowner_t;
  nodeowner_t allnodes_;
 
  typename sequence_t::seq_t::iterator currentAppendPosition_; 

  sequenceptrmap_t sequences_; 
  boost::shared_mutex sequences_map_mutex_;

  sequence_t * currentSequence_; 

  typedef TimeCursor<T> timecursor_t; 
  typedef cursor_impl<T> timecursorimpl_t; 
  typedef boost::ptr_map<cursorid_t, timecursorimpl_t> cursormap_t;
  cursormap_t cursors_; 
  boost::shared_mutex cursors_mutex_; 
  cursorid_t cursorid_; 

public:

  TimeSeriesDataBase() : 
    currentSequence_(NULL),
    cursorid_(1)
  { 


  }

   void reset() {
     //invalidate all existing cursors
       boost::shared_lock<boost::shared_mutex> cursorlock(cursors_mutex_); 
       cursors_.clear(); 
    
       boost::unique_lock<boost::shared_mutex> seqlock(sequences_map_mutex_); 
       
       if (currentSequence_) {
	 delete currentSequence_;
       }
       
    typedef typename sequenceptrmap_t::value_type t; 

    BOOST_FOREACH(t & s, sequences_) {
      delete s.second; 
    }


    BOOST_FOREACH(nc_t * p, allnodes_){
      delete p; 
      
    }



  }
  
  ~TimeSeriesDataBase()
  {
    reset(); 
  }

  size_t size() {
    // return total number of elements
    return allnodes_.size(); 
  }

  size_t seqsize() {
    size_t val = 0; 
    if (currentSequence_ != NULL) {
      val += 1; 
    }
    
    val +=  sequences_.size(); 
    return val; 

  }
  
  bool newSequence(timeid_t t, const T & val ) {
    /*
      Append the element as the start of a new sequence
      
      return: true if we created a new sequence
              false if timeID was the tail of some other sequence

      throws an error if the timeID is contained _within_ some
      existing sequence


    */ 
    
    boost::upgrade_lock<boost::shared_mutex> lock(sequences_map_mutex_);
    
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    std::cout << "newSequence called with t=" << t << std::endl;
    if(currentSequence_) {

      sequences_.insert(std::make_pair(currentSequence_->getHeadTime(), currentSequence_));
      currentSequence_ = NULL; 
    }
    
    if (!sequences_.empty() ) {
      // does this time exist in any other list? That is, are we starting
      // in the middle of a list? 
      
      
      typename sequenceptrmap_t::iterator range =  find_time_in_range(t); 
      if (range == sequences_.end()) 
	{
	  std::cout << "This is a brand-new sequence starting with  " << t << std::endl;
	  MonotonicConstantSequence<T> * seq = new MonotonicConstantSequence<T>; 
	  seq->valid = true; 
	  
	  ListNode<T> * headnode = new ListNode<T>(t, val); 
	  
	  seq->sequence.push_back(*headnode); 
	  currentSequence_ = seq; 
	  currentAppendPosition_ = seq->sequence.begin(); 
	  allnodes_.push_back(headnode); 
	  return false; 
	} else { 
	
	typename sequence_t::seq_t::iterator i  = range->second->search_leq(t);
	std::cout << "searched leq " << std::endl;
	if (i == range->second->sequence.end())
	  throw std::runtime_error("Violation of monotonic updates"); // FIXME better error message. 
	
	if (i->time != t){
	  throw std::runtime_error("violation of monotonci updates"); // FIXME better error message
	}
	std::cout << "instead of creating new sequence, selecting sequence that began at " << range->first
		  << std::endl;
	
	currentSequence_ = range->second; 
	sequences_.erase(range); 
	currentAppendPosition_ = i; 
	std::cout << "setting currentAppendPosition_ = " << i->time << std::endl; 
	return true; 
      }
    } else { 
      // This is the first sequence in this DB
      std::cout << "This is the first sequence in the DB" << std::endl;
      MonotonicConstantSequence<T> * seq = new MonotonicConstantSequence<T>; 
      seq->valid = true; 
      
      ListNode<T> * headnode = new ListNode<T>(t, val); 
      
      seq->sequence.push_back(*headnode); 
      currentSequence_ = seq; 
      currentAppendPosition_ = seq->sequence.begin(); 

      allnodes_.push_back(headnode); 

      return false;
    }
  }

  bool append(timeid_t t, const T & val) {
    /* 
       Always append to the current (head) sequence; 
       returns true if this element has been added before
    */

    std::cout << "append called with t=" << t << std::endl;
    std::cout << "Sequences start is " << sequences_.begin()->first
	      << std::endl;

    boost::upgrade_lock<boost::shared_mutex> seqmaplock(sequences_map_mutex_);
    boost::upgrade_to_unique_lock<boost::shared_mutex> seqmapuniqueLock(seqmaplock);


    if(currentSequence_ == NULL) {
      throw std::runtime_error("Appending without a valid sequence");    
    }


    // if we're appending the same time, we're good
    if(currentAppendPosition_->time == t) {
      return true; 
    }

    if (currentAppendPosition_->time > t) { 
      throw std::runtime_error("Violation of montonicity"); 
    }

    // check and see if we would violate continuity in other
    // lists
    typename sequenceptrmap_t::iterator closest_list = sequences_.lower_bound(t); 
    
    if (closest_list != sequences_.end()) {
      std::cout << "Checking to merge lists" << std::endl;
      if (t == closest_list->first) {
	// MERGE THE LISTS
	currentAppendPosition_ = closest_list->second->sequence.begin(); 
	join_current_to(closest_list) ;
	
	return true; 
	
      }
    }
    
    typename sequenceptrmap_t::iterator range =  find_time_in_range(t); 
    if (range != sequences_.end()) 
      {
	assert(range->first != t); 
	throw std::runtime_error("Violation of monotono"); 	
      }


    // if we're appending the next time, it's okay too, and
    // increment pointer
    typename sequence_t::seq_t::iterator nextptr = currentAppendPosition_; 
    nextptr++; 
    
    if (nextptr != currentSequence_->sequence.end()) {
      if (nextptr->time != t) {
	throw std::runtime_error("Violation of monotono"); 
      } else {
	currentAppendPosition_ = nextptr; 
	return true; 
      }
    }
    
    ListNode<T> * tailnode = new ListNode<T>(t, val); 
    
    boost::shared_lock<boost::shared_mutex> seqlock(currentSequence_->mutex);
    currentSequence_->sequence.push_back(*tailnode); 
    
    allnodes_.push_back(tailnode); 
    currentAppendPosition_ = currentSequence_->sequence.iterator_to(*tailnode); 
    
  }
  
//   // reader interface
//   TimeCursor<T>  createTimeCursor(timeid_t leq) {
//     /* return a time cursor pointing to some 
//        time equal to or after the requested time. 

//        I.e. if we request time 1000, we could return
//        a cursor pointing to time 1000 or time 1100 but not
//        one pointing to 900

//        if we're past the end, we return a NULL CURSOR

//     */
//     cursorid_t cid; 

//     return TimeCursor(cid, this); 
    

//   }
  
//   void deleteCursor(TimeCursor<T> * tc) { 

//     boost::scoped_lock lock(cursorsmutex_); 
//     cursors_.erase(tc); 
//   }

  
  void join_current_to(typename sequenceptrmap_t::iterator tgt)
  {
    /*
      join the current list to a later list, tgt
      
    */ 
    currentSequence_->mutex.lock(); 
    sequence_t * seq = tgt->second; 
    seq->mutex.lock(); 
    currentSequence_->sequence.splice(currentSequence_->sequence.end(),
				      seq->sequence); 
    
    // delete all the cursors FIXME
    sequences_.erase(tgt); 

    seq->mutex.unlock(); 

    delete seq; 


    currentSequence_->mutex.unlock(); 

  }
  
  typedef typename sequenceptrmap_t::iterator seqptrmapiter_t; 
  seqptrmapiter_t find_time_in_range(timeid_t t)
  {
    /*
      FIXME: linear
    */ 
    seqptrmapiter_t i = sequences_.begin(); 
    for(i; i != sequences_.end(); ++i) {
      if ((t >= i->first) and (t <= i->second->getTailTime())) {
	return i;
      }
    }
    return sequences_.end(); 
  }

  timecursor_t createCursor(timeid_t inittime) { 
    // can take place in a client thread, so we must lock 
    // access to the underlying sequence 
    std::cout << "init time = " << inittime << " " << 
      sequences_.begin()->first << std::endl;
    boost::shared_lock<boost::shared_mutex> lock(sequences_map_mutex_); 
    seqptrmapiter_t iter =  sequences_.upper_bound(inittime); 
    
    sequence_t * seq= NULL; 
    
    if (inittime <= currentSequence_->getHeadTime()) {
      seq = currentSequence_; 
    }

    if (iter != sequences_.end()) { 
      assert(inittime <= iter->first) ; // is this really doing what we think it's doing
      if (seq != NULL) { 
	if (iter->first < seq->getHeadTime()) {
	  seq = iter->second;
	}
      } else { 
	seq = iter->second; 
      }
    }

    if(seq == NULL) {
      timecursor_t tc(0, NULL); 
      std::cout << "Returnning null cursor" << std::endl;
      return tc; 
    }

    timecursorimpl_t * tic = new timecursorimpl_t(); 

    tic->requestTime_ = inittime; 
    tic->startTime_ = seq->getHeadTime(); 
    tic->valid_ = true;

    tic->tgtsequence = seq; 
    {
      // FIXME : do we need this mutex? 
      boost::shared_lock<boost::shared_mutex> seqlock(tic->tgtsequence->mutex);
      assert(tic->tgtsequence->sequence.size() > 0);
      tic->currentReadPosition_ = tic->tgtsequence->sequence.begin(); 
    }

    boost::shared_lock<boost::shared_mutex> cursorslock(cursors_mutex_); 

    cursors_.insert(cursorid_, tic); 

    timecursor_t tc(cursorid_, this); 
    cursorid_++;
    
    return tc;     
  }

  void closeCursor(cursorid_t id) {
    boost::upgrade_lock<boost::shared_mutex> lock(cursors_mutex_);

    typename cursormap_t::iterator ci = cursors_.find(id); 
    if (ci == cursors_.end() ) {
      throw std::runtime_error("Cursor does not exist; perhaps closed?");
    }
    
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
    cursors_.erase(ci); 
  }

  timeid_t getCursorRequestTime(cursorid_t id) {
    std::cout << "getCursorRequestTime" << std::endl;
    boost::shared_lock<boost::shared_mutex> lock(cursors_mutex_);
    std::cout << "lock acquired" << std::endl;
    typename cursormap_t::iterator ci = cursors_.find(id); 
    if (ci == cursors_.end() ) {
      throw std::runtime_error("Cursor does not exist; perhaps closed?");
    }
    assert(ci->second != NULL); 
    std::cout << "Attempting the dereference" << std::endl;
    return ci->second->requestTime_; 


  }
  
  timeid_t getCursorStartTime(cursorid_t id) 
  {
    boost::shared_lock<boost::shared_mutex> lock(cursors_mutex_);

    typename cursormap_t::iterator ci = cursors_.find(id); 
    if (ci == cursors_.end() ) {
      throw std::runtime_error("Cursor does not exist; perhaps closed?");
    }
    return ci->second->startTime_; 

  }

  timeid_t getCursorCurrentValue(cursorid_t id, T * val)
  {
    boost::shared_lock<boost::shared_mutex> lock(cursors_mutex_);
    std::cout << "about to search through cursors" << std::endl;

    typename cursormap_t::iterator ci = cursors_.find(id); 
    if (ci == cursors_.end() ) {
      throw std::runtime_error("Cursor does not exist; perhaps closed?");
    }
    
    boost::shared_lock<boost::shared_mutex> seqmaplock(sequences_map_mutex_);
    std::cout << "getting current value" << std::endl;
    timeid_t time = ci->second->currentReadPosition_->time; 
    *val = ci->second->currentReadPosition_->value; 
    return time; 
  }

  CURSOR_RET getCursorNext(cursorid_t id)
  {
    std::cout << "Get cursor next" << std::endl; 
    boost::shared_lock<boost::shared_mutex> lock(cursors_mutex_);
    
    std::cout << "About to search " << &cursors_  << std::endl;
    typename cursormap_t::iterator ci =    cursors_.find(id); 

    std::cout << "Searched..." << std::endl; 
    if (ci == cursors_.end() ) {
      throw std::runtime_error("Cursor does not exist; perhaps closed?");
    }
    std::cout << "HERE " << (int)cursors_.size() << " and more" << std::endl;
    std::cout << "reading thing" << ci->first <<  std::endl; 
    std::cout << "the seq is " << ci->second->tgtsequence << std::endl; 
    boost::shared_lock<boost::shared_mutex> seqlock(ci->second->tgtsequence->mutex); 
    ci->second->currentReadPosition_++ ; 

    std::cout << "incremented pointer" << std::endl; 

    if(ci->second->currentReadPosition_ == ci->second->tgtsequence->sequence.end()) {
      ci->second->currentReadPosition_-- ; // backup
      return CURSOR_RET_NO_NEXT; 
    }
    return CURSOR_RET_GOOD; 
  }

}; 

template<typename T>
class cursor_impl
{
public:
  /* Cursor implementation class */ 
  typedef MonotonicConstantSequence<T> sequence_t; 

  sequence_t * tgtsequence; 
  typename sequence_t::seq_t::iterator currentReadPosition_; 
  timeid_t requestTime_; 
  timeid_t startTime_; 
  
  bool valid_; 
  
};


template<typename T>
class TimeCursor
{
  /* 
     TimeCursor is really just a loose proxy around the underlying
     set of APIs on the base object

   */
public:
  ~TimeCursor()
  {
    
  }

  timeid_t requestTime()
  {
    return parentDB_->getCursorRequestTime(cursorid_); 
  }
  
  timeid_t startTime() 
  {
    return parentDB_->getCursorStartTime(cursorid_); 
  }

  timeid_t getCurrentValue(T * val)
  {
    return parentDB_->getCursorCurrentValue(cursorid_, val); 
  }
  
  CURSOR_RET next() {
    std::cout << "cursor next, parentdb = " << parentDB_ << std::endl; 
    return parentDB_->getCursorNext(cursorid_); 

  }

  TimeCursor()  :
    cursorid_(0), 
    parentDB_(NULL)
  {


  }

  TimeCursor(const TimeCursor<T> & copyFromMe)
  {
    parentDB_ = copyFromMe.parentDB_;
    cursorid_ = copyFromMe.cursorid_; 
    
  }

  bool valid() {
    return cursorid_ != 0; 
  }

private:
  TimeSeriesDataBase<T> * parentDB_; 
  cursorid_t cursorid_; 


  TimeCursor(cursorid_t cid, TimeSeriesDataBase<T> *  parentDB) :
    cursorid_(cid), 
    parentDB_(parentDB)
  {
    std::cout << "private constructor, parentDB_ = " << parentDB_  << std::endl;

  }
  
  friend class TimeSeriesDataBase<T>; 
  
};


}


#endif
