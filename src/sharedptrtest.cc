#include <iostream>
#include <boost/shared_ptr.hpp>
#include <vector>

class SimpleBase; 
class SimpleA; 

typedef boost::shared_ptr<SimpleA> sharedSimpleAPtr_t; 
typedef boost::shared_ptr<SimpleBase> sharedSimpleBasePtr_t; 

class SimpleBase 
{
public:
  virtual void doSomething(void) = 0; 

}; 

class SimpleA : public SimpleBase
{
public:
  SimpleA(int val) {
    val_ = val; 
  }

  void doSomething(){
    std::cout << "doing something A" << std::endl; 
  }
  ~SimpleA() {
    std::cout << "Deleting SimpleA with val = " << val_ << std::endl; 
  }


private:
  int val_; 
  
};


int main(void)
{
  
  std::vector<sharedSimpleBasePtr_t> slist; 
  std::vector<sharedSimpleAPtr_t> slista; 

  
  for (int i = 0; i < 100; i++) {
    sharedSimpleBasePtr_t x(new SimpleA(i)); 
    slist.push_back(x); 
  }
  
  for (int i = 0; i < 100; i++) {
    sharedSimpleAPtr_t a1(boost::dynamic_pointer_cast<SimpleA>(slist[i])); 
    slista.push_back(a1); 
  }
  slist.clear(); 
  
  std::cout << "all smart pointers have been created" << std::endl; 

}
