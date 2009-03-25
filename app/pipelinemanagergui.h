#ifndef PIPELINEMANAGERGUI_H
#define PIPELINEMANAGERGUI_H
#include <gtkmm.h>
#include <pipelinemanager.h>
#include "propertypanemanager.h"
#include "somaconfig.h"

/* 
   Connect to the new-pipeline signal
   and for that pipeline, modify its visitors to be the appropriate
   tied-in gui components for that pipeline

*/

class PipelineManagerGUI 
{
public:
  PipelineManagerGUI(pPipelineManager_t pm, 
		     pSomaConfig_t somaconfig, 
		     Gtk::VBox & vBoxPipelines, 
		     PropertyPaneManager & ppm); 
  ~PipelineManagerGUI();
  

private:
  Gtk::VBox &  vBoxPipelines_; 
  pPipelineManager_t pm_; 
  PropertyPaneManager & ppm_;
  void newPipeline(pStreamPipeline_t sp); 
  
  pSomaConfig_t pSomaConfig_;
};

#endif // PIPELINEMANAGERGUI_H
