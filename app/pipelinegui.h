#ifndef PIPELINEGUI_H
#define PIPELINEGUI_H
#include <gtkmm.h>
#include "viscreator.h"
#include "sourcecreator.h"
#include "triggercreator.h"
#include "filtercreator.h"
#include "streampipeline.h"
#include "propertypanemanager.h" 
#include "somaconfig.h"

class PipelineGUI : public Gtk::VBox
{
public:
  PipelineGUI(pStreamPipeline_t sp, pSomaConfig_t sc, 
	      PropertyPaneManager * ppm);
  ~PipelineGUI(); 
  
  void addNewSource(Widget *  w); 
  void addNewVis(Widget *  w); 
  void show();
  PropertyPaneManager * getPropertyPaneManager(); 
  pSomaConfig_t getSomaConfig(); 
  
private:
  pStreamPipeline_t streamPipeline_; 
  pVisCreator_t pVisCreator_; 
  pTriggerCreator_t pTriggerCreator_; 
  pSourceCreator_t pSourceCreator_; 
  pFilterCreator_t pFilterCreator_; 

  PropertyPaneManager * ppm_; 

  Gtk::HBox mainBox_; 
  Gtk::Frame  sourceBin_; 
  Gtk::VBox visBox_; 
  Gtk::Label testLabel_; 
  pSomaConfig_t pSomaConfig_; 
}; 


#endif // PIPELINEGUI_H
