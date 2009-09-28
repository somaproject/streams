#include "spectviswidget.h"
#include <boost/format.hpp>

SpectVisWidget::SpectVisWidget(SpectVis * nw, pSomaConfig_t sc) :
  ElementStatus(sc),
  spectvis_(nw)
{
  add(vbox_);
  vbox_.pack_start(label_); 
  label_.set_text("Scale"); 
  show_all(); 

  spectvis_->scale.signal().connect(sigc::mem_fun(*this, 
						 &SpectVisWidget::scaleUpdate)); 
  scaleUpdate(0.0);

  set_tooltip_text("Click me!");
}


SpectVisWidget::~SpectVisWidget()
{
  

}

void SpectVisWidget::on_size_allocate(Gtk::Allocation & r)
{
  Gtk::EventBox::on_size_allocate(r); 
  spectvis_->setPixelHeight(r.get_height()); 
}

void SpectVisWidget::scaleUpdate(float scale)
{
  label_.set_text(boost::str(boost::format("scale : %f") % 
			     spectvis_->scale)); 
  
}
