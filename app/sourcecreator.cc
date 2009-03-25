#include "sourcecreator.h"
#include "noisewavewidget.h"
#include "pipelinegui.h"
#include "netdatawavewidget.h"


SourceCreator::SourceCreator(PipelineGUI & pg) :
  pipelineGUI_(pg)
{


}

SourceCreator::~SourceCreator()
{


}


void SourceCreator::visit(SourceBase * sb)
{


}

void SourceCreator::visit(NoiseWave * nw)
{
  // create gui wrapper around NoiseWave
  NoiseWaveWidget *  ng(new NoiseWaveWidget(nw, pipelineGUI_.getSomaConfig())); 
  ng->setPropertyPaneManager(pipelineGUI_.getPropertyPaneManager()); 

  // tell pipeline gui that there's a new source, and let it
  // position appropriatelyh
  pipelineGUI_.addNewSource(ng); 
}

void SourceCreator::visit(NetDataWave * nd)
{

  NetDataWaveWidget * ndw(new NetDataWaveWidget(nd, pipelineGUI_.getSomaConfig())); 
  ndw->setPropertyPaneManager(pipelineGUI_.getPropertyPaneManager()); 

  pipelineGUI_.addNewSource(ndw); 
  

}
