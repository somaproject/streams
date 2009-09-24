#ifndef __BDB_QUEUEVIEW_H__
#define __BDB_QUEUEVIEW_H__

#include <db_cxx.h>
#include <core/iqueueview.h>

/*

  A BDB-like queue view to an underlying BDB database
  opened in 

*/


template<typename T> 
class BDBQueueView : public core::IQueueView<T>
{
  
public:
  BDBQueueView(Db * db) 
  {
//     db->cursor(NULL, &cursor_, 0); 
//     Dbt found_data; 
//     Dbt search_key; 

//     int ret = cursor_->get(&search_key, &found_data, DB_FIRST); 

//     if (ret == DB_NOTFOUND) {
//       empty_= true; 
//     } else { 
//       empty_ = false; 
//       memcpy(&data_, found_data.get_data(), sizeof(T)); 
//     }
  }
  
  ~BDBQueueView()   {
    std::cout << "Bdb queue view destroying" << std::endl;
    //cursor_->close(); 

  }


  
  bool empty() {
    if (empty_ == false) {
      return false; 
    } 

    // empty is currently true
    Dbt found_data; 
    Dbt search_key; 
    int ret = cursor_->get(&search_key, &found_data, DB_NEXT); 
    if (ret == DB_NOTFOUND) {
      empty_= true; 
      return true; 
    } else { 
      empty_ = false; 
      memcpy(&data_, found_data.get_data(), sizeof(T)); 
      return false; 
    }
    
    
  }
 
  T &  front()  {
//     if (empty()) {
//       throw std::runtime_error("QueueView is empty");
//     }
    return data_; 
  }

  void pop() {

    Dbt found_data; 
    Dbt search_key; 

    int ret = cursor_->get(&search_key, &found_data, DB_NEXT);

    if (ret == DB_NOTFOUND) {
      empty_ = true; 
    } else { 
      empty_ = false; 
      memcpy(&data_, found_data.get_data(), sizeof(T)); 
    }
    
  }
    
  void reset() {
    Dbt found_data; 
    Dbt search_key; 
    int ret = cursor_->get(&search_key, &found_data, DB_FIRST); 

    if (ret == DB_NOTFOUND) {
      empty_ = true; 
    } else { 
      empty_ = false; 
      memcpy(&data_, found_data.get_data(), sizeof(T)); 
    }
    

  }
private:
  Dbc * cursor_; 
  T data_; 
  bool empty_; 

};



#endif 
