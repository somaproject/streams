#include <iostream>


#include "properties.h" 

class TestClass {
public: 
  Property<double> doubleProp; 
  Property<std::string> stringProp;
  
  double testDoubleProp(); 
  std::string testStringProp(); 

};

double TestClass::testDoubleProp()
{
  return doubleProp; 
}

std::string TestClass::testStringProp()
{
  return stringProp; 
}

void propDoubleCallback(double val) {
  std::cout << " callback Double was " << val << std::endl; 
}

int main(void){
  Property<int> myprop; 
  myprop = 7; 
  
  std::cout << myprop << std::endl; 
  TestClass tc; 
  tc.doubleProp = 18.5; 
  tc.doubleProp.signal().connect(&propDoubleCallback); 
  tc.doubleProp = 1.21e9; 
  
  std::cout << tc.testDoubleProp() << std::endl; 
  
}
