#include "networkdatacache.h"
#include <boost/format.hpp>

NetworkDataCache::NetworkDataCache(pISomaNetCodec_t pnc, 
				   pTimer_t ptime, bf::path scratchdir): 
  pNetCodec_(pnc), 
  pTimer_(ptime),
  scratchdir_(scratchdir / "netdatawave"),
  waveSignals_(MAXWAVE),
  rawSignals_(MAXWAVE)
{
  pNetCodec_->newDataSignal().connect(sigc::mem_fun(*this, 
						 &NetworkDataCache::appendNewData)); 
  
  // create the db for each data type. Note that we don't put data
  // in there until it has been expressly enabled. 
  // 
  
  u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE; // Open flags;
 
  boost::filesystem::remove_all(scratchdir_ );
  boost::filesystem::create_directory(scratchdir_); 
  
}


NetworkDataCache::~NetworkDataCache()
{
  std::cout << "network data cache destructor" << std::endl;
  //boost::filesystem::remove_all(scratchdir_ );

}

core::IQueueView<WaveBuffer_t>::ptr NetworkDataCache::getNetWaveSource(datasource_t n)
{
  dbmap_t::iterator pdb = waveCacheDBs_.find(n); 
  if (pdb == waveCacheDBs_.end()) {
    // open a new database
    Db * db = new Db(NULL, 0); 
    
    bf::path wavedb_path = scratchdir_ / boost::str(boost::format("wave%d.db") % n); 
      u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE; // Open flags;

    db->set_pagesize(1<<16); 
    db->set_flags(DB_INORDER); 
    db->set_re_len(sizeof(WaveBuffer_t)); 
    db->open(NULL,                // Transaction pointer
	     wavedb_path.string().c_str(),          // Database file name
	     NULL, 
	     DB_QUEUE,            // Database access method
	     oFlags,              // Open flags
	     0);                  // File mode (using defaults)
    
    std::pair<dbmap_t::iterator, bool> retval; 
    retval = waveCacheDBs_.insert(n, db); 
    pdb = retval.first;

    pNetCodec_->enableDataRX(n, WAVE); 
    
  } 

  return core::IQueueView<WaveBuffer_t>::ptr(
					     new BDBQueueView<WaveBuffer_t>(pdb->second)); 
  
}

core::IQueueView<WaveBuffer_t>::ptr NetworkDataCache::getNetRawSource(datasource_t n)
{

  dbmap_t::iterator pdb = rawCacheDBs_.find(n); 
  
  if (pdb == rawCacheDBs_.end()) {
    std::cout << "Creating new database" << std::endl; 
    // open a new database
    Db * db = new Db(NULL, 0); 
    
    bf::path rawdb_path = scratchdir_ / boost::str(boost::format("raw%d.db") % n); 
    u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE; // Open flags;
    
    db->set_pagesize(1<<16); 
    db->set_flags(DB_INORDER); 
    db->set_re_len(sizeof(WaveBuffer_t)); 
    db->open(NULL,                // Transaction pointer
	     rawdb_path.string().c_str(),          // Database file name
	     NULL, 
	     DB_QUEUE,            // Database access method
	     oFlags,              // Open flags
	     0);                  // File mode (using defaults)
    
    std::pair<dbmap_t::iterator, bool> retval; 
    retval = rawCacheDBs_.insert(n, db); 
    pdb = rawCacheDBs_.find(n);
    pNetCodec_->enableDataRX(n, RAW); 
    
  } 
  std::cout << "The ptr is " << pdb->second << std::endl; 
  core::IQueueView<WaveBuffer_t>::ptr p(new BDBQueueView<WaveBuffer_t>(pdb->second)); 
  return p; 

}

void NetworkDataCache::appendNewData(pDataPacket_t newData)
{
  if (newData->typ == WAVE) {
    dbmap_t::iterator pdb = waveCacheDBs_.find(newData->src); 
    if (pdb !=  waveCacheDBs_.end()) {

      int src = newData->src; 
      Wave_t  wp = rawToWave(newData); 
      WaveBuffer_t * wb = new WaveBuffer_t; 
      wb->samprate = wp.sampratenum / float(wp.samprateden); 
      wb->time = pTimer_->somaTimeToStreamTime(wp.time); 
      wb->data.reserve(WAVEBUF_LEN); 
      for(int i = 0; i < WAVEBUF_LEN; i++) {
	float val = float(wp.wave[i]) / 1e9; 
	float tsdelta =  1.0 / wb->samprate; 
	float tval = (wb->time + float(i) * tsdelta); 
	wb->data.push_back(val); 
      }

      db_recno_t rid; 
    
      Dbt key(&rid, sizeof(rid));
      Dbt data(wb, sizeof(WaveBuffer_t)); 
      
      int ret = pdb->second->put(NULL, &key, &data, DB_APPEND); // FIXME check return
      assert(ret == 0); 

      waveSignals_[src].emit(); 
    }
    
  } else if (newData->typ == RAW) {
    dbmap_t::iterator pdb = rawCacheDBs_.find(newData->src); 

    if (pdb != rawCacheDBs_.end() ) {
      int src = newData->src; 
      Raw_t  wp = rawToRaw(newData); 
      //if (wp.chansrc == 0) {  // FIXME!!
	WaveBuffer_t * wb = new WaveBuffer_t; 
	wb->samprate = 32000.0; 
	wb->time = pTimer_->somaTimeToStreamTime(wp.time); 
	wb->data.reserve(WAVEBUF_LEN); 
	for(int i = 0; i < RAWBUF_LEN; i++) {
	  wb->data.push_back(float(wp.data[i])/1e9); 
	}
	
	db_recno_t rid; 
    
	Dbt key(&rid, sizeof(rid));
	Dbt data(wb, sizeof(WaveBuffer_t)); 
      
	int ret = pdb->second->put(NULL, &key, &data, DB_APPEND); // FIXME check return

	assert(ret == 0); 
	rawSignals_[src].emit(); 
	//}
    }
    
  } 
  else {
    // FIXME: Log unknown data packet 
    
  }

}

