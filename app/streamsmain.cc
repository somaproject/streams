#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <somanetwork/fakenetwork.h>
#include <somanetwork/network.h>

#include <sigc++/sigc++.h>

namespace po = boost::program_options;

std::string LOG_ROOT("soma.streams.streamsmain"); 

#include "logging.h"


#include "networkdatacache.h"
#include "streamsapp.h"

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
    ; 

  desc.add(logging_desc()); 
    
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    
  
  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }
  
  config_logging(vm, LOG_ROOT); 

  // get logging objects
  log4cpp::Category& logstreams = log4cpp::Category::getInstance(LOG_ROOT);
  
  std::string somaip; 
  if (!vm.count("soma-ip")) {
    logstreams.fatal("soma-ip was not specified, no way to get data"); 
    return -1; 
  }

  somaip = vm["soma-ip"].as<string>(); 
  logstreams.infoStream() << "soma hardware IP: " << somaip; 


  pNetworkInterface_t pnetwork(new Network(somaip)); 
  pSomaNetCodec_t psnc(new SomaNetCodec(pnetwork)); 
  
  
  pTimer_t ptimer; 
  if(vm.count("debug-timer")) {
    ptimer = Timer::createDummyTimer(); 
  } else {
    ptimer = Timer::createNetworkTimer(psnc); 
  }
  
  pNetworkDataCache_t pndc(new NetworkDataCache(psnc, ptimer)); 

  pSourceState_t pSourceState(new SourceState); 
  pSourceState->timer = ptimer; 
  pSourceState->netdatacache = pndc; 
  pSourceState->somanetcodec = psnc; 

  StreamsApp sa(pSourceState); 

  pPipelineManager_t pm = sa.getPipelineManager();   
  for (int i = 0; i < 10; i++) {

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
  kit.run(sa);

  return 0;
}
