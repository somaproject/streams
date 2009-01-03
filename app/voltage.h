#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <string>
#include <boost/format.hpp>
#include <stdexcept>

/* 
A class to handle voltage-like types, including conversion to and 
from strings. 

Internally we store everything as 64-bit integers representing nanovolts. 

*/ 

class ConversionError : public  std::runtime_error {
 public:
  ConversionError() : std::runtime_error("Conversion Error") {}

}; 

class Voltage
{
 public:
  Voltage(double V); 
  Voltage(int V); 
  Voltage(std::string str); 
  
  std::string str(int floatpoints = 2); 
  int32_t to_nv(); // returns the voltage as a float in nv; 
  double to_volts(); 

 private:
  int64_t value_; 

}; 



#endif // VOLTAGE_H
