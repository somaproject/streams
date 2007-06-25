#include "visproperty.h"

VisProperty::VisProperty(std::set<streamVisPtr_t> * svp) :
  pSelSet_(svp)
{

  pGladeXml_ = Gnome::Glade::Xml::create("wavevisproperty.glade");
  Gtk::VBox * win = 0; 
  
  pGladeXml_->get_widget("mainVBox", win); 
  win->reparent(*this);
}
 
void VisProperty::updateSet()
{
  // for the time being we assume there's only wavevis
  
  // populate 
  std::set<streamVisPtr_t>::iterator i;
  for(i = pSelSet_->begin(); i!= pSelSet_->end(); ++i)
    {
      
      
    }
  
}
