#include <math.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "voltage.h"



Voltage::Voltage(double V) :
  value_(0)
{
  double result = round(V*1e9); 
  value_ = result; 
}

Voltage::Voltage(int V) :
  value_(V)
{


}

Voltage::Voltage(std::string str):
  value_(0)
{
  // This is where the bulk of the effort comes in; we want to turn
  // 1e6 V and 100e-6 mV and 10e-9 uV and 123.4567 nV all into 
  // the correct values; and we throw an exception if we can't
  // parse the format

  std::stringstream ssi(str), ssd(str); 
  int64_t vint; 
  double vdouble = std::numeric_limits<double>::quiet_NaN(); 

  std::string sint, sdouble; 

  
  ssi >> vint; 
  ssi >> sint; 

  ssd >> vdouble; 
  ssd >> sdouble; 

  
  // first we try integer
  if (sint == "V" ) { 
    value_ = vint * 1000000000; 
  } else if (sint == "mV") {
    value_ = vint * 1000000; 
    
  } else if (sint == "uV") {
    value_ = vint * 1000; 
  } else if (sint == "nV") {
    value_ = vint; 

  } else {
    // now we try the floats 

    if (sdouble == "V" ) { 
      value_ = vdouble * 1000000000; 
    } else if (sdouble == "mV") {
      value_ = vdouble * 1000000; 
    } else if (sdouble == "uV") {
      value_ = vdouble * 1000; 
    } else if (sdouble == "" and !isnan(vdouble)) {
      // assume Volts
      value_ = vdouble * 1000000000; 
    } else {

      throw ConversionError(); 
    }

  }

}

int32_t Voltage::to_nv()
{
  return value_ & 0xFFFFFFFF; 
}

double Voltage::to_volts()
{
  double result = double(value_)/1e9; 

  return result;
}

std::string Voltage::str(int floatpoints)
{
  // Return the canonical string representation. 
  
  boost::format format; 
  
  if (value_ < 1000) {
    // return in NV 
    format = boost::format("%d nV"); 
    format % value_; 

  } else if (value_ < 1000000) {
    // return in uV
    format = boost::format("%0.2f uV"); 
    format %  boost::io::group(std::setprecision(floatpoints),
			       ((double)value_ /1e3) ); 
  } else if (value_ < 1000000000) {
    // return in mV 
    format = boost::format("%0.2f mV"); 
    format %  boost::io::group(std::setprecision(floatpoints),
			       ((double)value_ /1e6) ); 

  } else {
    // return in V
    format = boost::format("%0.2f V"); 
    format %  boost::io::group(std::setprecision(floatpoints),
			       ((double)value_ /1e9) ); 
  }

  return boost::str(format); 

}
