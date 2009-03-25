#include <boost/format.hpp>

#include "somaconfig.h"


SomaConfig::SomaConfig()
{


}


SomaConfig::~SomaConfig()
{


}


std::vector<somanetwork::datasource_t> SomaConfig::getAvailableSources()
{
  std::vector<somanetwork::datasource_t> srcs; 
  for (int i = 0; i < 64; i++) {
    srcs.push_back(i); 
  }

  return srcs; 


}


std::list<somanetwork::datatype_t> SomaConfig::getSourceDataTypes(somanetwork::datasource_t src)
{
  std::list<somanetwork::datatype_t> dt; 
  dt.push_back(somanetwork::TSPIKE); 
  dt.push_back(somanetwork::WAVE); 
  dt.push_back(somanetwork::RAW); 
  
}

std::string SomaConfig::getSourceName(somanetwork::datasource_t src)
{
  return boost::str(boost::format("source %d") % (int)src); 

}

