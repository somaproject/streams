
#include "spectvis.h"
#include <iostream>
#include <boost/format.hpp>

const std::string SpectVis::TYPENAME = "SpectVis"; 

SpectVis::SpectVis(std::string name, bf::path scratch) :
  VisBase(name), 
  pSinkPad_(createSinkPad<WaveBuffer_t>("default")), 

  yheight_(100),
  color_("red"),
  verticalScale_(1.0),
  scratchdir_(scratch / name),
  pixelHeight_(100),
  spectblockdb_(new Db(NULL, 0))
{

  u_int32_t oFlags = DB_CREATE |  DB_TRUNCATE |  DB_THREAD ; // Open flags;
  bf::create_directories(scratchdir_);
  bf::path dbpath = scratchdir_ / "spectblock.db"; 
  std::cout << "Attempting to create " << dbpath << std::endl; 
  spectblockdb_->set_pagesize(1<<16); 
  spectblockdb_->set_bt_compare(spectvis_compare_double);
  int ret = spectblockdb_->open(NULL,                // Transaction pointer
				dbpath.string().c_str(), 
				NULL,                // Optional logical database name
				DB_BTREE,            // Database access method
				oFlags,              // Open flags
				0);                  // File mode (using defaults)

  streamRenderer_ = new SpectVisRenderer(spectblockdb_); 
  pSinkPad_->newDataSignal().connect(sigc::mem_fun(*this, 
						   &SpectVis::newData)); 
  
}

void SpectVis::renderStream(streamtime_t t1, streamtime_t t2, int pixels)
{
  // i really hate how this modifies the gloabl GL state
  streamRenderer_->renderStream(t1, t2, pixels); 
  
}

SpectVis::~SpectVis()
{
  
  delete streamRenderer_; 
}


void SpectVis::newData()
{
  // right now this is just a stupid no-op


  char * buffer = (char*)malloc(SpectRenderBlock::maxbytes()); 
  // our inputs are buffers of data, our filtered
  // outputs are GLwavePoints
  while (not pSinkPad_->getpQueueView()->empty())
    {
      // we're taking in WaveBuffer_t pointers

      WaveBuffer_t & wb = pSinkPad_->getpQueueView()->front(); 

      SpectRenderBlock sbb(wb.data.size(), 1); 

      sbb.starttime = wb.time; 
      double starttime = sbb.starttime; 
      sbb.endtime = wb.time + wb.data.size() * 1.0 / wb.samprate;

      sbb.lowfreq = 0;
      sbb.highfreq = 100; 
      
      int size = sbb.marshall_to_buffer(buffer); 
      Dbt key(&(starttime), sizeof(starttime));
      Dbt data(buffer, size); 
      int ret = spectblockdb_->put(NULL, &key, &data, DB_NOOVERWRITE);
      if (ret != 0) {
	spectblockdb_->err(ret, "Put failed because key %f already exists", sbb.starttime);
      }
      
      pSinkPad_->getpQueueView()->pop();       
    }
  
  free(buffer); 
}

void SpectVis::invalidateData()
{


}

// float SpectVis::getYOffset(){
//   return yoffset_; 
// }

// void SpectVis::setYOffset(float x)
// {
//   yoffset_ = x; 
// }

// void SpectVis::setYHeight(float x)
// {
//   yheight_ = x; 
//   streamRenderer_.setScale(yheight_ / verticalScale_, yheight_); 
// }

// void SpectVis::setVerticalScale(float volts){

//   verticalScale_ = volts; 
  
//   streamRenderer_.setScale(yheight_ / verticalScale_ , yheight_); 
//   verticalScaleSignal_.emit(volts); 

// }

// float SpectVis::getVerticalScale()
// {
//   return verticalScale_; 
// }

// void SpectVis::setColor(Gdk::Color c)
// {
  
//   color_ = c; 
//   streamRenderer_.setColor(c); 
//   colorSignal_.emit(c); 
// }

// Gdk::Color SpectVis::getColor()
// {
//   return color_; 
// }

// GLWavePoint_t SpectVis::filterNextPoint(GLWavePoint_t wp)
// {
  
//   wp.t -= 0.1; // delay

//   return wp; 



// }




void SpectVis::renderTrigger(streamtime_t deltapre, streamtime_t deltapost, int pixels)
{


}



void SpectVis::setPixelHeight(int x)
{

  pixelHeight_ = x; 

}

int SpectVis::getPixelHeight()
{
  return pixelHeight_; 

}

void SpectVis::setScale(float)
{


}
