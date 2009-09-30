#ifndef __JSON_UTIL_H__
#define __JSON_UTIL_H__

#include "json_spirit.h"

namespace json_util { 

using namespace json_spirit; 

const mValue& find_value( const mObject& obj, const std::string& name  ); 
bool opt_find_value( const mObject& src, const std::string& name, mValue & out  ); 



}



#endif 
