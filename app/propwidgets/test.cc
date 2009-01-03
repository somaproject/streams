#include <iostream>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm.h>

#include <properties.h>
#include "spinbutton.h"
#include "combobox.h"

class HelloWorld : public Gtk::Window
{

public:
  HelloWorld();
  virtual ~HelloWorld();

protected:
  //Signal handlers:
  virtual void on_button_clicked();
  virtual void setProp2();
  virtual void setComboInt(); 
  virtual void setComboInt1(); 

  //Member widgets:Gtk::Adjustment
  // SPIN BUTTON TEST
  Property<float> prop1_; 
  Property<float> prop2_; 
  Gtk::Adjustment adjustment_; 
  PropertyWidgets::SpinButton m_button; 
  Gtk::Button statusButton_; 
  Gtk::Button setButton_; 
  Gtk::Button setComboIntButton_; 
  Gtk::Button setComboInt1Button_; 
  Gtk::VBox box_; 
  bool on_my_output(); 

  // COMBO BOX TEST

  PropertyWidgets::ComboBox<int> comboBox_; 
  Property<int> comboIntProp1_; 
  Property<int> comboIntProp2_; 

};

HelloWorld::HelloWorld()
  : prop1_(0.0), 
    prop2_(0.0), 
    adjustment_(1.0, 1.0, 102.0, 1.0, 5.0, 0.0),
    statusButton_("status"), 
    setButton_("set prop2"), 
    setComboIntButton_("set both combo int"), 
    setComboInt1Button_("set combo int 1"), 
    m_button(adjustment_),   // creates a new button with label "Hello World".
    comboIntProp1_(1),
    comboIntProp2_(1)
{
  // Sets the border width of the window.
  set_border_width(10);

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  statusButton_.signal_clicked().connect(sigc::mem_fun(*this,
						       &HelloWorld::on_button_clicked));
  setButton_.signal_clicked().connect(sigc::mem_fun(*this,
						       &HelloWorld::setProp2));
  setComboIntButton_.signal_clicked().connect(sigc::mem_fun(*this,
						       &HelloWorld::setComboInt));

  setComboInt1Button_.signal_clicked().connect(sigc::mem_fun(*this,
						       &HelloWorld::setComboInt1));
  //m_button.signal_output().connect(sigc::mem_fun(*this, &HelloWorld::on_my_output)); 
  // This packs the button into the Window (a container).

  add(box_);
  box_.pack_start(m_button); 
  box_.pack_start(statusButton_); 
  box_.pack_start(setButton_); 
  box_.pack_start(setComboIntButton_); 
  box_.pack_start(setComboInt1Button_); 
  m_button.addProperty(&prop1_);
  m_button.addProperty(&prop2_);

  // Combo box
  PropertyWidgets::ComboBox<int>::possiblevalues_t vals; 
  vals.push_back(std::make_pair("Hello1", 1)); 
  vals.push_back(std::make_pair("Hello2", 2)); 
  vals.push_back(std::make_pair("Hello3", 3)); 
  vals.push_back(std::make_pair("Hello4", 4)); 
  comboBox_.setPossibleValues(vals); 
  comboBox_.addProperty(&comboIntProp1_); 
  comboBox_.addProperty(&comboIntProp2_); 

  box_.pack_start(comboBox_); 

  // The final step is to display this newly created widget...
  show_all(); 
}

bool HelloWorld::on_my_output()
{
  
  //std::cout << "on_my_output" << std::endl; 
  //m_button.set_text(""); 

  return false; 


}
HelloWorld::~HelloWorld()
{
}

void HelloWorld::on_button_clicked()
{
  std::cout << "Hello World" << std::endl;
  std::cout << "prop1 = " << prop1_ << std::endl;
  std::cout << "prop2 = " << prop2_ << std::endl;

  std::cout << "comboIntProp1 = " << comboIntProp1_ << std::endl; 
  std::cout << "comboIntProp2 = " << comboIntProp2_ << std::endl; 
}


void HelloWorld::setProp2()
{
  prop2_ = 100.0; 

}

void HelloWorld::setComboInt()
{
  comboIntProp1_ = 3; 
  comboIntProp2_ = 3; 

}

void HelloWorld::setComboInt1()
{
  comboIntProp1_ = 1; 
}

int main (int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  HelloWorld helloworld;
  //Shows the window and returns when it is closed.
  Gtk::Main::run(helloworld);

  return 0;
}

