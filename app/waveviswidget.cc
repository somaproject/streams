#include "waveviswidget.h"
#include <boost/format.hpp>

WaveVisWidget::WaveVisWidget(WaveVis * nw, pSomaConfig_t sc) :
  ElementStatus(sc),
  wavevis_(nw)
{
  add(vbox_);
  vbox_.pack_start(label_); 
  label_.set_text("Scale"); 
  show_all(); 

  wavevis_->scale.signal().connect(sigc::mem_fun(*this, 
						 &WaveVisWidget::scaleUpdate)); 
  scaleUpdate(0.0);

  set_tooltip_text("Click me!");
}


WaveVisWidget::~WaveVisWidget()
{
  

}

void WaveVisWidget::on_size_allocate(Gtk::Allocation & r)
{
  Gtk::EventBox::on_size_allocate(r); 
  wavevis_->setPixelHeight(r.get_height()); 
}

void WaveVisWidget::scaleUpdate(float scale)
{
  label_.set_text(boost::str(boost::format("scale : %f") % 
			     wavevis_->scale)); 
  
}
