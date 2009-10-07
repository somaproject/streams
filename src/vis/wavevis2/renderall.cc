#include "renderall.h"
#include "types.h"

namespace wavevis2 {

int renderall_compare_timeid(Db *db, const Dbt *a, const Dbt *b)
{
  // Returns:
  // < 0 if a < b
  // = 0 if a = b
  // > 0 if a > b
  timeid_t ad; 
  timeid_t bd; 
  memcpy(&ad, a->get_data(), sizeof(a));
  memcpy(&bd, b->get_data(), sizeof(b));
  
  if (ad < bd) return -1; 
  if (ad > bd) return 1;
  return 0; 
}


RenderAll::RenderAll(bf::path scratch) :
  dbEnv_(0)
{


  u_int32_t env_flags = DB_CREATE |     // If the environment does not
    DB_INIT_MPOOL |
    DB_INIT_CDB |
    DB_THREAD ; // Initialize the in-memory cache.
  
  bf::path envHome = scratch / "renderall"; 
  boost::filesystem::remove_all(envHome); 
  boost::filesystem::create_directories(envHome);
  
  try {
    dbEnv_.open(envHome.string().c_str(), env_flags, 0);
  } catch(DbException &e) {
    std::cerr << "Error opening database environment: "
              << envHome << std::endl;
    std::cerr << e.what() << std::endl;
    exit( -1 );
  } catch(std::exception &e) {
    std::cerr << "Error opening database environment: "
              << envHome << std::endl;
    std::cerr << e.what() << std::endl;
    exit( -1 );
  } 

  db_ = new Db(&dbEnv_, 0);               // Instantiate the Db object
  u_int32_t oFlags = DB_CREATE |   DB_THREAD; // Open flags;
  
  std::string dbname("my_db.db"); 
  boost::filesystem::remove_all(dbname );
  
  db_->set_pagesize(1<<16); 
  db_->set_bt_compare(renderall_compare_timeid);
  db_->open(NULL,                // Transaction pointer
	  dbname.c_str(),          // Database file name
	  NULL,                // Optional logical database name
	  DB_BTREE,            // Database access method
	  oFlags,              // Open flags
	  0);                  // File mode (using defaults)

  
}


void RenderAll::newSample(WaveBuffer_t & wb) {
  GLPointBuffer_t * pb = new GLPointBuffer_t; 
	
  pb->size = 0; 
  double bufstarttime = (double)wb.time / 1e9; 
  
  double period = 1/wb.samprate; 
  // do the conversion
  for(int i = 0; i < wb.data.size(); i++) { 
    pb->data[pb->size].t = period * i; 
    pb->data[pb->size].x = wb.data[i]; 
    pb->size++; 
  }
  
  Dbt key(&wb.time, sizeof(wb.time));
  Dbt data(pb, sizeof(GLPointBuffer_t)); 
  Dbc * cursorp; 
  db_->cursor(NULL, &cursorp,  DB_WRITECURSOR  ); 
  int ret = cursorp->put(&key, &data, DB_KEYFIRST); 
  if (ret != 0) { 
    std::cout << "ERror putting " << std::endl; 
  }
  cursorp->close(); 
  
  delete pb; 

}

void RenderAll::renderStream(timeid_t t1, timeid_t t2, int pixels)
{
  /* Must be thread-safe!
   */
   

  Dbc *cursorp;
  
  timeid_t searchval = t1; 
  Dbt search_key(&searchval, sizeof(searchval)); 
  Dbt found_data; 
  
  
  db_->cursor(NULL, &cursorp, 0); 
  int ret = cursorp->get(&search_key, &found_data, DB_SET_RANGE);

  while ((ret == 0) and (*((timeid_t*)search_key.get_data()) < t2)) {
    timeid_t buftime = *((timeid_t*)search_key.get_data()) ; 
    
    GLPointBuffer_t * bufptr; 
    bufptr = (GLPointBuffer_t * )found_data.get_data(); 
    renderGLPointBuffer(buftime - t1, bufptr); 
    ret = cursorp->get(&search_key, &found_data, DB_NEXT);
  } 
  
  cursorp->close(); 
  

}

void RenderAll::renderGLPointBuffer(timeid_t origintime, 
				    GLPointBuffer_t * bufptr)
{
  double originf = double(origintime) / 1e9; 
  glPushMatrix();
  glTranslatef(originf, 0, 0); 
  glVertexPointer(2, GL_FLOAT, sizeof(GLWavePoint_t), &(bufptr->data[0]) ); 
  glDrawArrays(GL_LINE_STRIP, 0, bufptr->size); 
  glPopMatrix(); 
}


void RenderAll::reset() {
  /* 
     Instead of truncating, we delete all the records; ug, slow. 
  */ 
  timeid_t tid= 0; 
  Dbt key(&tid, sizeof(tid));
  GLPointBuffer_t pb; 
  Dbt data(&pb, sizeof(GLPointBuffer_t)); 
  Dbc * cursorp; 
  db_->cursor(NULL, &cursorp,  DB_WRITECURSOR  ); 
  int ret; 
  while ((ret = cursorp->get(&key, &data, 
			     DB_SET)) == 0) {
    cursorp->del(0);
  }
  cursorp->close(); 


}

RenderAll::~RenderAll()
{

  delete db_; 

}



}
