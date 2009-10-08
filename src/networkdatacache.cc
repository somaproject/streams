#include "networkdatacache.h"
#include <boost/format.hpp>

NetworkDataCache::NetworkDataCache(pISomaNetCodec_t pnc, 
				   pTimer_t ptime, bf::path scratchdir): 
  dbEnv_(0), 
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
  
  dbEnv_.set_cachesize(0, 1024 * 1024 * 50, 1); 

  u_int32_t env_flags = DB_CREATE |     // If the environment does not
    DB_INIT_MPOOL |
    DB_INIT_CDB |
    DB_THREAD ; // Initialize the in-memory cache.
  
  boost::filesystem::remove_all(scratchdir_ );
  boost::filesystem::create_directories(scratchdir_); 

  try {
    dbEnv_.open(scratchdir_.string().c_str(), env_flags, 0);
  } catch(DbException &e) {
    std::cerr << "Error opening database environment: "
              << scratchdir_ << std::endl;
    std::cerr << e.what() << std::endl;
    exit( -1 );
  } catch(std::exception &e) {
    std::cerr << "Error opening database environment: "
              << scratchdir_ << std::endl;
    std::cerr << e.what() << std::endl;
    exit( -1 );
  } 

  
}


NetworkDataCache::~NetworkDataCache()
{
  //boost::filesystem::remove_all(scratchdir_ );

}

core::IQueueView<WaveBuffer_t>::ptr NetworkDataCache::getNetWaveSource(datasource_t n)
{
  dbmap_t::iterator pdb = waveCacheDBs_.find(n); 
  if (pdb == waveCacheDBs_.end()) {
    // open a new database
    Db * db = new Db(&dbEnv_, 0); 
    
    bf::path wavedb_path(boost::str(boost::format("wave%d.db") % n)); 
      u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE; // Open flags;

    db->set_pagesize(1<<16); 
    db->set_flags(DB_INORDER); 
    db->set_re_len(sizeof(WaveBufferDisk_t<WAVEBUF_LEN>)); 
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
					     new BDBQueueView<WaveBuffer_t, WaveBufferDisk_t<WAVEBUF_LEN> >(pdb->second)); 
  
}

core::IQueueView<WaveBuffer_t>::ptr NetworkDataCache::getNetRawSource(datasource_t n)
{

  dbmap_t::iterator pdb = rawCacheDBs_.find(n); 
  
  if (pdb == rawCacheDBs_.end()) {
    // open a new database
    Db * db = new Db(&dbEnv_, 0); 
    
    bf::path rawdb_path(boost::str(boost::format("raw%d.db") % n)); 
    u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE; // Open flags;
    
    db->set_pagesize(1<<16); 
    db->set_flags(DB_INORDER); 
    db->set_re_len(sizeof(WaveBufferDisk_t<RAWBUF_LEN>)); 
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

    core::IQueueView<WaveBuffer_t>::ptr p(new BDBQueueView<WaveBuffer_t, WaveBufferDisk_t<RAWBUF_LEN> >(pdb->second)); 
  return p; 

}

void NetworkDataCache::appendNewData(pDataPacket_t newData)
{
  if (newData->typ == WAVE) {
    dbmap_t::iterator pdb = waveCacheDBs_.find(newData->src); 
    if (pdb !=  waveCacheDBs_.end()) {

      int src = newData->src; 
      Wave_t  wp = rawToWave(newData); 
      WaveBufferDisk_t<WAVEBUF_LEN>  wb; 
      wb.samprate = wp.sampratenum / float(wp.samprateden); 
      wb.time = pTimer_->somaTimeToStreamTime(wp.time); 
      for(int i = 0; i < WAVEBUF_LEN; i++) {
	float val = float(wp.wave[i]) / 1e9; 
	float tsdelta =  1.0 / wb.samprate; 
	float tval = (wb.time + float(i) * tsdelta); 
	wb.data[i] = val; 
      }

      db_recno_t rid; 
    
      Dbt key(&rid, sizeof(rid));
      Dbt data(&wb, sizeof(WaveBufferDisk_t<WAVEBUF_LEN> )); 
      
//       Dbc * cursorp; 
//       pdb->second->cursor(NULL, &cursorp,  DB_WRITECURSOR  ); 

      int ret = pdb->second->put(NULL,  &key, &data, DB_APPEND); // FIXME check return
      assert(ret == 0); 
      //      cursorp->close(); 

      waveSignals_[src].emit(); 
    }
    
  } else if (newData->typ == RAW) {
    dbmap_t::iterator pdb = rawCacheDBs_.find(newData->src); 

    if (pdb != rawCacheDBs_.end() ) {
      int src = newData->src; 
      Raw_t  wp = rawToRaw(newData); 
      WaveBufferDisk_t<RAWBUF_LEN> wb; 
      wb.samprate = 32000.0; 
      wb.time = pTimer_->somaTimeToStreamTime(wp.time); 

      for(int i = 0; i < RAWBUF_LEN; i++) {
	wb.data[i] = (float(wp.data[i])/1e9); 
      }
      
      db_recno_t rid; 
      
      Dbt key(&rid, sizeof(rid));
      Dbt data(&wb, sizeof(WaveBufferDisk_t<RAWBUF_LEN>)); 

//       Dbc * cursorp; 
//       pdb->second->cursor(NULL, &cursorp,  DB_WRITECURSOR  ); 
      
      int ret = pdb->second->put(NULL, &key, &data, DB_APPEND); // FIXME check return
      //      cursorp->close(); 
      assert(ret == 0); 
      rawSignals_[src].emit(); 
      //}
    }
    
  } 
  else {
    // FIXME: Log unknown data packet 
    
  }

}

