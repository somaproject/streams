#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <somanetwork/fakenetwork.h>
#include <somanetwork/network.h>
#include <string>
#include <sigc++/sigc++.h>
#include <somanetwork/logging.h>
#include <somadspio/logging.h>

namespace po = boost::program_options;

//std::string LOG_ROOT("soma.streams.streamsmain"); 

#include "logging.h"

using namespace std; 
namespace bf = boost::filesystem; 

#include "networkdatacache.h"
#include "streamsapp.h"
#include "somanetcodec.h"

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);
  
  Gtk::GL::init(argc, argv);
  
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("soma-ip", po::value<string>(), "The IP of the soma hardware")
    ("debug-timer", "debug: use internally-generated (non-network) timer")
    ("enable-network-log", po::value<string>()->default_value("warning"), "Enable soma network debugging at this level")
    ("enable-dspio-log", po::value<string>()->default_value("warning"), "Enable soma DSP IO debugging at this level")
    ("force-reference-time", po::value<long>()->default_value(0), "Force the reference timestamp")
    ("scratch-dir", po::value<string>()->default_value("/tmp"), "Scratch directory for data cache")
    ; 

//   desc.add(logging_desc()); 
    
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    
  
  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }
  
//   config_logging(vm, LOG_ROOT); 

//   // get logging objects
//   log4cpp::Category& logstreams = log4cpp::Category::getInstance(LOG_ROOT);
  
  std::string somaip; 
  if (!vm.count("soma-ip")) {
//     logstreams.fatal("soma-ip was not specified, no way to get data"); 
    return -1; 
  }

  somaip = vm["soma-ip"].as<string>(); 
//   logstreams.infoStream() << "soma hardware IP: " << somaip; 

  if (vm.count("enable-network-log")) {
    string logstr = vm["enable-network-log"].as<string>(); 
    if (logstr == "") {
      logstr = "warning"; 
    }
    boost::logging::level::type lt = log_level_parse(logstr); 
    somanetwork::init_logs(lt);  
  }

  if (vm.count("enable-dspio-log")) {
    string logstr = vm["enable-dspio-log"].as<string>(); 
    if (logstr == "") {
      logstr = "warning"; 
    }
    boost::logging::level::type lt = log_level_parse(logstr); 
    somadspio::init_logs(lt);
  }

  somatime_t expStartTime = 0; 
  
  if(vm.count("force-reference-time")) {
    expStartTime =  vm["force-reference-time"].as<long>(); 
  }

  pNetworkInterface_t pnetwork =  Network::createINet(somaip); 
  pISomaNetCodec_t psnc(new SomaNetCodec(pnetwork)); 
  
  
  pTimer_t ptimer; 
  if(vm.count("debug-timer")) {
    ptimer = Timer::createDummyTimer(expStartTime); 
  } else {
    ptimer = Timer::createNetworkTimer(psnc, expStartTime); 
  }

  bf::path scratchdir_base(vm["scratch-dir"].as<string>()); 
  
  pNetworkDataCache_t pndc(new NetworkDataCache(psnc, ptimer, 
						scratchdir_base / "netdata")); 

  pSourceState_t pSourceState(new SourceState); 
  pSourceState->timer = ptimer; 
  pSourceState->netdatacache = pndc; 
  pSourceState->somanetcodec = psnc; 



  StreamsApp sa(pSourceState, scratchdir_base); 

  pPipelineManager_t pm = sa.getPipelineManager();   
  for (int i = 0; i < 2; i++) {

    pStreamPipeline_t pl = 
      pm->createPipeline(boost::str(boost::format("pipeline%d") % i));
    pISource_t src = pl->createSource("NoiseWave", 
				      boost::str(boost::format("ns%d") % i)); 

    pIVis_t vis = pl->createVis("WaveVis", 
				boost::str(boost::format("waveVis%d") % i)); 
          
    core::pISourcePad_t ps1 = 
      src->getSourcePad("default");
    core::pISinkPad_t ps2 = 
      vis->getSinkPad("default"); 
    ps1->connect(ps2);
  }

  pnetwork->run(); 

  kit.run(sa);

  return 0;
}

