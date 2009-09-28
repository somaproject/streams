#include "viscreator.h"
#include "waveviswidget.h"
#include "spectviswidget.h"
#include "pipelinegui.h"

VisCreator::VisCreator(PipelineGUI  & pg) :
  pipelineGUI_(pg)
{
  
  
}

VisCreator::~VisCreator()
{



}

void VisCreator::visit(VisBase * )
{
  std::cout << "VisCreator: Visiting for VisBase" << std::endl;




}


void VisCreator::visit(SineVis* sv )
{

}


void VisCreator::visit(WaveVis * pwv)
{
  
  WaveVisWidget * wv = new WaveVisWidget(pwv, pipelineGUI_.getSomaConfig()); 
  wv->setPropertyPaneManager(pipelineGUI_.getPropertyPaneManager()); 
  
  // tell pipeline gui that there's a new source, and let it
  // position appropriately
  pipelineGUI_.addNewVis(wv); 
  

}

void VisCreator::visit(SpectVis * psv)
{
  
  SpectVisWidget * sv = new SpectVisWidget(psv, pipelineGUI_.getSomaConfig()); 
  sv->setPropertyPaneManager(pipelineGUI_.getPropertyPaneManager()); 
  
  // tell pipeline gui that there's a new source, and let it
  // position appropriately
  pipelineGUI_.addNewVis(sv); 
  

}
