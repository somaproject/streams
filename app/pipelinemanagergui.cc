#include "pipelinemanagergui.h"
#include "pipelinegui.h"
PipelineManagerGUI::PipelineManagerGUI(pPipelineManager_t pm, 
				       pSomaConfig_t somaconfig, 
				       Gtk::VBox & vBoxPipelines, 
				       PropertyPaneManager& ppm) :
  pm_(pm), 
  vBoxPipelines_(vBoxPipelines), 
  ppm_(ppm),
  pSomaConfig_(somaconfig)
{
  
  // connect to signals
  pm->pipelineCreatedSignal().connect(sigc::mem_fun(*this, 
						    &PipelineManagerGUI::newPipeline)); 
  

}

PipelineManagerGUI::~PipelineManagerGUI()
{


}

void PipelineManagerGUI::newPipeline(pStreamPipeline_t sp)
{
  //std::cout << "PipelineManagerGUI::newPipeline" << std::endl;
  PipelineGUI * pg = new PipelineGUI(sp,  pSomaConfig_, &ppm_); 

  //Gtk::Label * l = new Gtk::Label("Hello Too"); 
  vBoxPipelines_.pack_start(*pg);

//   vBoxPipelines_.pack_start(*pg); 
  pg->show(); 
  vBoxPipelines_.show_all();
  //std::cout << "Now there are " << vBoxPipelines_.children().size() << std::endl; 
}

