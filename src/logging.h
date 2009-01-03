#ifndef LOGGING_H
#define LOGGING_H



#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace std; 


int config_logging(po::variables_map vm, std::string LOGROOT); 

po::options_description logging_desc(); 

log4cpp::Priority::Value intToPriority(int custlevel); 

int  priorityToInt(log4cpp::Priority::Value v); 

#endif // LOGGING_H
