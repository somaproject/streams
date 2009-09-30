#include "jsonutil.h"



namespace json_util { 

const mValue& find_value( const mObject& obj, const std::string& name  )
{
    mObject::const_iterator i = obj.find( name );

    assert( i != obj.end() );
    assert( i->first == name );

    return i->second;
}

bool opt_find_value( const mObject& src, const std::string& name, mValue & out  )
{

  
  mObject::const_iterator i = src.find( name );
  if (i == src.end()) 
    return false; 
  
  assert( i->first == name );

  out =  i->second;
  return true; 

}

}
