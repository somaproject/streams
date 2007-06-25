#ifndef SOURCESTATUS_H
#define SOURCESTATUS_H

class SourceStatus: public Gtk::HBox
{
 public: 
  virtual streamSourcePtr_t getSourcePtr() = 0; 

}; 


#endif // SOURCESTATUS_H

