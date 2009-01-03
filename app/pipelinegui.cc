#include "pipelinegui.h"

PipelineGUI::PipelineGUI(pStreamPipeline_t sp, PropertyPaneManager * ppm) :
  Gtk::VBox(),
  streamPipeline_(sp), 
  pVisCreator_(new VisCreator(*this)), 
  pSourceCreator_(new SourceCreator(*this)), 
  pFilterCreator_(new FilterCreator(*this)), 
  pTriggerCreator_(new TriggerCreator(*this)), 
  testLabel_(sp->getName() + "Silly"), 
  ppm_(ppm)
{

  // create the new visitors for this element and save them
  sp->resetVisitors(pSourceCreator_, pFilterCreator_, 
		    pTriggerCreator_,  pVisCreator_); 
  
  pack_start(mainBox_); 


  mainBox_.pack_start(sourceBin_); 
  mainBox_.pack_start(visBox_); 
}


PipelineGUI::~PipelineGUI()
{



}

PropertyPaneManager * PipelineGUI::getPropertyPaneManager() 
{
  return ppm_; 
  
}
void PipelineGUI::addNewSource(Gtk::Widget* w)
{
  std::cout << "Added new source" << std::endl;
  manage(w); 
  sourceBin_.add(*w); 
  w->show(); 
  show_all(); 
}

void PipelineGUI::addNewVis(Gtk::Widget * w)
{
  visBox_.pack_start(*w); 
  w->show(); 

  show_all();
}

void PipelineGUI::show()
{

  show_all();

}
