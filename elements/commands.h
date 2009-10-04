#ifndef __ELEMENTS_COMMANDS_H__
#define __ELEMENTS_COMMANDS_H__

namespace elements
{
  enum MESSAGES { 
    RESET,
    NEWDATA,
    NEWSEQUENCE,
    CONNECT, 
    DISCONNECT}; 
  
  class commandqueue
  {
    /*
      AT THE MOMENT A VERY NAIVE IMPLEMENTATION
      
    */ 
  public:
    void send(MESSAGES m); 
    bool empty(); 
    MESSAGE get(); 

  private:
    boost::mutex mutex_; 
    std::list<MESSAGES> queue_; 

  }; 
  
}


#endif 
