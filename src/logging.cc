#include "logging.h"

int config_logging(po::variables_map vm, std::string LOGROOT) {
  /*
    Process the variables map and configure logging. I feel like we should
    abstract this away somehow, but I'm not really too concerned right now

  */

  // configure logging
  std::string propfile; 
  if (vm.count("log-properties")) {
    log4cpp::PropertyConfigurator::configure(vm["log-properties"].as<string>()); 
    
  } 
  
  log4cpp::Category& logrecorder = log4cpp::Category::getInstance(LOGROOT);
  log4cpp::Category& logdbus = log4cpp::Category::getInstance(LOGROOT + ".dbus");
  log4cpp::Priority::Value loglevel = log4cpp::Priority::WARN; 

  int custlevel; 
  if (vm.count("log-level")) {
    custlevel = vm["log-level"].as<int>(); 
    loglevel = intToPriority(custlevel); 
  } 
  
  log4cpp::Appender * logappender =  new log4cpp::OstreamAppender("appender", &std::cout); 
  log4cpp::Layout* loglayout = new log4cpp::BasicLayout();
  logappender->setLayout(loglayout); 
  log4cpp::Category::getRoot().setPriority(loglevel); 
  log4cpp::Category& root = log4cpp::Category::getRoot();
  
  root.addAppender(logappender); 

}

po::options_description logging_desc() 
{

  po::options_description loggingdesc("Logging options");
  loggingdesc.add_options()
    ("log-properties", po::value<string>(), "A custom log configuration file")
    ("log-level", po::value<int>()->default_value(400), 
     "The minimum log level to log to standard out; 0=lowest (log everything), 9 =highest (log nothing)"); 

  return loggingdesc; 

}

log4cpp::Priority::Value intToPriority(int custlevel) {

  log4cpp::Priority::Value  loglevel; 


    switch(custlevel) {
    case 0:  
      return  log4cpp::Priority::DEBUG;  
      break; 
    case 1:  
      return log4cpp::Priority::INFO;  
      break; 
    case 2:  
      return log4cpp::Priority::NOTICE;  
      break; 
    case 3:  
      return log4cpp::Priority::WARN;  
      break; 
    case 4:  
      return log4cpp::Priority::ERROR;  
      break; 
    case 5:  
      return log4cpp::Priority::CRIT;  
      break; 
    case 6:  
      return log4cpp::Priority::ALERT;  
      break; 
    default:
      break; 
    }
    
    return log4cpp::Priority::FATAL;  
}


int  priorityToInt(log4cpp::Priority::Value v) {

  switch(v) {
  case log4cpp::Priority::DEBUG: 
    return 0; 
    break; 
  case log4cpp::Priority::INFO:
    return 1; 
    break; 
  case log4cpp::Priority::NOTICE:
    return 2; 
    break; 
  case log4cpp::Priority::WARN:
    return 3; 
    break; 
  case log4cpp::Priority::ERROR:
    return 4; 
    break; 
  case log4cpp::Priority::CRIT:
    return 5; 
    break; 
  case log4cpp::Priority::ALERT:
    return 6; 
    break; 
  default: 
    return -1; 
  }

}
