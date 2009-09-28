#include "pipelineconfig.h"

#include "json_spirit.h"
#include <cassert>
#include <sstream>

using namespace json_spirit; 


const mValue& find_value( const mObject& obj, const std::string& name  )
{
    mObject::const_iterator i = obj.find( name );

    assert( i != obj.end() );
    assert( i->first == name );

    return i->second;
}


void load_pipeline_config(pPipelineManager_t pm, std::istream & is)
{
  
  mValue val; 
  read( is, val );
  const mArray & pipelines_array = val.get_array(); 
  
  std::cout << "There are " << pipelines_array.size() << " pipelines" << std::endl;
  for (int p = 0; p < pipelines_array.size(); p++) {

    std::cout << "pipeline " << p << std::endl;
    mObject po = pipelines_array[p].get_obj(); 

    mValue name_val = find_value(po, "name"); 
    std::string name = name_val.get_str(); 
    
    mObject elt_obj = find_value(po, "elements").get_obj(); 
    pStreamPipeline_t pipeline = pm->createPipeline(name); 
    
    // create the elements
    std::map<std::string, core::pIElement_t> elements; 

    mObject::const_iterator ielt; 
    for (ielt = elt_obj.begin(); ielt != elt_obj.end(); ++ielt) {
      std::string eltname = ielt->first;
      mObject elt = ielt->second.get_obj(); 
      std::string type = find_value(elt, "type").get_str(); 
      std::string clas = find_value(elt, "class").get_str(); 
      
      mObject::const_iterator i = elt.find("config" );
      mValue config; 
      if (i != elt.end()) {
	config = i->second.get_obj(); 
      }
      
      if (type == "source") { 
	elements[eltname] = pipeline->createSource(clas, eltname); 
      } else if (type == "vis") {
	elements[eltname] = pipeline->createVis(clas, eltname); 
      } else {
	throw std::runtime_error("unknown element type"); 
      }
   }
    
    mValue conn_obj = find_value(po, "connections"); 
    const mArray & conns = conn_obj.get_array(); 
    for (int ci = 0; ci < conns.size(); ++ci) {
      mArray c = conns[ci].get_array(); 
      std::string srcelt = c[0].get_str(); 
      std::string srcport = c[1].get_str(); 
      std::string destelt = c[2].get_str(); 
      std::string destport = c[3].get_str(); 

      core::pISourcePad_t ps_s = elements[srcelt]->getSourcePad(srcport); 
      core::pISinkPad_t ps_d = elements[destelt]->getSinkPad(destport); 
      ps_s->connect(ps_d); 

    }
    std::cout << "end " << std::endl;
    //int x = conn_obj.get_int(); 
    
    
  }


}
