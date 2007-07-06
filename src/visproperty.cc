#include "wavestreamvis.h"

#include "visproperty.h"

VisProperty::VisProperty(std::set<streamVisPtr_t> * svp) :
  pSelSet_(svp), 
  hScaleAdj_(0.0, 0.0, 10.0)
{

  pGladeXml_ = Gnome::Glade::Xml::create("wavevisproperty.glade");
  Gtk::VBox * win = 0; 
  
  pGladeXml_->get_widget("mainVBox", win); 
  win->reparent(*this);
  
  // connect signals
  Gtk::ColorButton * cb = 0; 
  pGladeXml_->get_widget("colorButton", cb); 
  cb->signal_color_set().connect(sigc::mem_fun(*this, 
					       &VisProperty::colorSet)); 
  Gtk::HScale * hs; 
  pGladeXml_->get_widget("heightScale", hs); 
  hs->set_adjustment(hScaleAdj_); 
  hScaleAdj_.signal_value_changed().connect(sigc::mem_fun(*this, 
						   &VisProperty::hScaleAdjValueChanged)); 
  

  
}
 
void VisProperty::updateSet()
{
  // for the time being we assume there's only wavevis
  Gdk::Color c; 
  // populate 
  std::set<streamVisPtr_t>::iterator i;
  for(i = pSelSet_->begin(); i!= pSelSet_->end(); ++i)
    {
      waveStreamVisPtr_t p = boost::dynamic_pointer_cast<WaveStreamVis> (*i); 
      c = p->getColor(); 
      std::cout << "setting color " << std::endl; 
    }
  Gtk::ColorButton * cb = 0; 
  pGladeXml_->get_widget("colorButton", cb); 
  
  cb->set_color(c); 
  
}
  
void VisProperty::colorSet()
{
  Gtk::ColorButton * cb = 0; 
  pGladeXml_->get_widget("colorButton", cb); 

  std::cout << "Color set via signal" << std::endl ; 
  std::set<streamVisPtr_t>::iterator i;

  for(i = pSelSet_->begin(); i!= pSelSet_->end(); ++i)
    {

      waveStreamVisPtr_t p = boost::dynamic_pointer_cast<WaveStreamVis> (*i); 
      p->setColor(cb->get_color()); 
    }

}

void VisProperty::hScaleAdjValueChanged()
{

  Gtk::Label * hsl; 
  pGladeXml_->get_widget("heightScaleLabel", hsl); 
  // this is where we do our specific setting
  double val = hScaleAdj_.get_value(); 
  float hscale = 1.0; 
  if ( val < 1.0) {
    hscale = 100e-6; 

  } else if (val < 2.0) {
    hscale = 200e-6; 
 
  } else if (val < 3.0) {
    hscale = 500e-6; 

  } else if (val < 4.0) {
    hscale = 1000e-6; 

  } else if (val < 5.0) {
    hscale = 2e-3; 

  } else if (val < 6.0) {
    hscale = 5e-3; 

  } else if (val < 7.0) {
    hscale = 10e-3; 

  } else if (val < 8.0) {
    hscale = 20e-3; 

  } else if (val < 9.0) {
    hscale = 50e-3; 

  } else {
    hscale = 100e-3; 
  }

  hsl->set_text(voltsToString(hscale).c_str()); 

  std::set<streamVisPtr_t>::iterator i;
  for(i = pSelSet_->begin(); i!= pSelSet_->end(); ++i)
    {

      waveStreamVisPtr_t p = boost::dynamic_pointer_cast<WaveStreamVis> (*i); 
      p->setVerticalScale(hscale); 
    }


}
