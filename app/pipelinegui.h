#ifndef PIPELINEGUI_H
#define PIPELINEGUI_H
#include <gtkmm.h>
#include "viscreator.h"
#include "sourcecreator.h"
#include "triggercreator.h"
#include "filtercreator.h"
#include "streampipeline.h"
#include "propertypanemanager.h" 

class PipelineGUI : public Gtk::VBox
{
public:
  PipelineGUI(pStreamPipeline_t sp, PropertyPaneManager * ppm);
  ~PipelineGUI(); 
  
  void addNewSource(Widget *  w); 
  void addNewVis(Widget *  w); 
  void show();
  PropertyPaneManager * getPropertyPaneManager(); 
  
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
}; 


#endif // PIPELINEGUI_H
