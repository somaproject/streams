#include <iostream>
#include <sigc++/sigc++.h>


template <typename ValueType>
class Property
{
public: 

  Property() {

  }
  
  operator ValueType() 
  {
    return getter(); 
  }
  
  ValueType operator =(const ValueType &  value) {
    setter(value); 
    return getter(); 
  }

  sigc::signal<void, ValueType> & signal() {
    return signal_; 
  }

private:
  ValueType value_; 
  void setter(ValueType value) {
    value_ = value; 
    signal_.emit(value_); 
  }

  ValueType getter()
  {
    return value_; 
  }

  sigc::signal<void, ValueType> signal_; 
  
}; 
