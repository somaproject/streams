#ifndef __BDB_QUEUEVIEW_H__
#define __BDB_QUEUEVIEW_H__

#include <db_cxx.h>
#include <core/iqueueview.h>

/*

  A BDB-like queue view to an underlying BDB database
  opened in queue mode, which stores bdbT objects. 
  These objects must have a bdbT.copy(&foo) object to 
  copy themselves into the underlying T object. 

  Originally we kept a cursor around, but that will totally
  screw threading. Since we're connected to a Queue-based BDB, 
  we know the recnos are monotonic starting at one, and can always just
  ask for the next. 

*/


template<typename T, typename bdbT> 
class BDBQueueView : public core::IQueueView<T>
{


  class Cursor
  {
    // RAII cursor wrapper
  public:
    Cursor(Db * db) {
      db->cursor(NULL, &cursor, 0); 
    } 
    
    ~Cursor()
    {
      cursor->close(); 
    }

    Dbc * cursor; 
  }; 

public:
  BDBQueueView(Db * db) :
    db_(db), 
    rid_(0) // RID is always "what's in data_"
  {

    Dbt found_data; 
    Dbt search_key; 
    
    Cursor c(db_); 
    int ret = c.cursor->get(&search_key, &found_data, DB_FIRST); 

    if (ret == DB_NOTFOUND) {
      empty_= true; 
    } else { 
      empty_ = false; 
      memcpy(&data_, found_data.get_data(), sizeof(T)); 
      rid_ = *((db_recno_t*)search_key.get_data()); 
    }

  }
  
  ~BDBQueueView()   {
    //cursor->close(); 

  }


  
  bool empty() {
    if (empty_ == false) {
      return false; 
    } 

    Dbt found_data; 
    db_recno_t next = rid_ + 1; 
    Dbt search_key(&next, sizeof(next)); 
    
    Cursor c(db_);
    int ret = c.cursor->get(&search_key, &found_data, DB_SET); 
    if (ret == DB_NOTFOUND) {
      empty_= true; 
      return true; 
    } else { 
      empty_ = false; 
      memcpy(&buffer_, found_data.get_data(), sizeof(bdbT)); 
      buffer_.copy(&data_); 
      rid_ = *((db_recno_t*)search_key.get_data()); 
      return false; 
    }
    
    
  }
 
  T &  front()  {
    if (empty()) {
      throw std::runtime_error("QueueView is empty");
    }
    return data_; 
  }

  void pop() {

    Dbt found_data; 

    db_recno_t next = rid_ + 1; 
    Dbt search_key(&next, sizeof(next)); 

    Cursor c(db_);

    int ret = c.cursor->get(&search_key, &found_data, DB_GET_BOTH);

    if (ret == DB_NOTFOUND) {
      empty_ = true; 
    } else { 
      empty_ = false; 
      memcpy(&data_, found_data.get_data(), sizeof(T)); 
      rid_ = *((db_recno_t*)search_key.get_data()); 
    }
    
  }
    
  void reset() {
    Dbt found_data; 
    Dbt search_key; 

    Cursor c(db_);

    int ret = c.cursor->get(&search_key, &found_data, DB_FIRST); 

    if (ret == DB_NOTFOUND) {
      empty_ = true; 
    } else { 
      empty_ = false; 
      memcpy(&data_, found_data.get_data(), sizeof(T)); 
      rid_ = *((db_recno_t*)search_key.get_data()); 
    }
    

  }
private:
  Db * db_; 
  T data_; 
  bdbT buffer_; 
  bool empty_; 
  db_recno_t rid_; 
  
};



#endif 
