#ifndef __SOMACONFIG_H__
#define __SOMACONFIG_H__

/*
  This is a helper class designed to answer system-wide soma questions
  like :  
      1. what channels are available with what data sources? 
      2. what are their names?
      3. What are the available filters on the DSPs

  For the time being I am imagining this as a proxy for the
  soma config server. 

 */ 

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <somanetwork/datapacket.h>
#include <list>

class SomaConfig : boost::noncopyable
{
public:
  SomaConfig();
  ~SomaConfig(); 
  
  std::vector<somanetwork::datasource_t> getAvailableSources(); 
  std::list<somanetwork::datatype_t> getSourceDataTypes(somanetwork::datasource_t ); 
  std::string getSourceName(somanetwork::datasource_t); 

}; 


typedef boost::shared_ptr<SomaConfig> pSomaConfig_t; 


#endif // __SOMACONFIG_H__

