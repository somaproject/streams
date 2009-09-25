#include "pipelineconfig.h"

#include "json_spirit.h"
#include <cassert>
#include <fstream>

using namespace std;
using namespace json_spirit;


const mValue& find_value( const mObject& obj, const string& name  )
{
    mObject::const_iterator i = obj.find( name );

    assert( i != obj.end() );
    assert( i->first == name );

    return i->second;
}


void load_pipeline_config(pPipelineManager_t, bf::path configfile)
{
  
  ifstream is(configfile.string() );

  mArray pipelines_array;
  read( is, value );
  
  for (int p = 0; p < pipelines_array; p++) {
    mObject po = pipeline_array[p].get_obj(); 
    
    std::string name = find_value(po, "name").get<string>(); 
    std::map<std::string, core::p
  }


}
