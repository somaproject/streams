#ifndef PROPERTY_COMBOBOX_H
#define PROPERTY_COMBOBOX_H

#include <gtkmm.h>
#include <elements/property.h>
#include <set>
#include <list>
#include "propwidget.h"

namespace PropertyWidgets
{
  template<class valuetype_t> 
  class ComboBox : public Gtk::ComboBox
  {
  public:
    typedef  std::vector<std::pair<std::string, valuetype_t> > possiblevalues_t; 

    ComboBox(); 
    ~ComboBox(); 
    enum State {NORMAL, PENDING, CONFLICTED}; 

  public:
    typedef elements::Property<valuetype_t> *  pProperty_t; 
    void addProperty(pProperty_t comboProperty); 
    void delProperty(pProperty_t comboProperty); 
    void setPossibleValues(const possiblevalues_t & ); 

  private:
    typedef std::set<pProperty_t >  propset_t ; 
    typedef std::map<pProperty_t, size_t>  notifymap_t; 
    propset_t propertySet_; 
    notifymap_t notifyMap_; 
    void refreshProperty(); 

    State state_; 
    valuetype_t value_; 
    void setState(State); 
    bool inSetState_; 
    void on_my_value_changed(); 

    //Tree model columns:
    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:
      
      ModelColumns()
      { 
	add(colID);
 	add(colVal); 
 	add(colName);
      } 
      Gtk::TreeModelColumn<int> colID;
      Gtk::TreeModelColumn<valuetype_t> colVal; 
      Gtk::TreeModelColumn<Glib::ustring> colName;
    };
    
    ModelColumns modelColumns_; 
    Glib::RefPtr<Gtk::ListStore> refTreeModel_; 
    
    possiblevalues_t possibleValues_; 

    pWidgetPropertyNotify_t notify_; 

  }; 

  template <class valuetype_t> 
  ComboBox<valuetype_t>::ComboBox() :
    Gtk::ComboBox(), 
    state_(NORMAL),
    notify_(new WidgetPropertyNotify)

  {
    refTreeModel_ = Gtk::ListStore::create(modelColumns_);
    
    set_model(refTreeModel_);
    signal_changed().connect(sigc::mem_fun(*this, 
					   &ComboBox::on_my_value_changed)); 
    pack_start(modelColumns_.colName); 
    notify_->signal().connect(sigc::mem_fun(*this, 
					    &ComboBox::refreshProperty)); 
    
    
  }

  template <class valuetype_t> 
  ComboBox<valuetype_t>::~ComboBox() {
    for(typename propset_t::iterator pi = propertySet_.begin(); pi != propertySet_.end(); pi++)
      { 
	delProperty(*pi); 
      }
    
  }

  template <class valuetype_t> 
  void ComboBox<valuetype_t>::addProperty(pProperty_t comboProperty) 
  {

    if (propertySet_.find(comboProperty) == propertySet_.end()) {
      
      propertySet_.insert(comboProperty); 

      size_t rethandle = comboProperty->add_watcher(notify_); 
      notifyMap_[comboProperty] = rethandle; 

      if (! possibleValues_.empty()) {
	refreshProperty(); 
      }
    }
    
    
    
  }
  
  template<class valuetype_t> 
  void ComboBox<valuetype_t>::setPossibleValues(const possiblevalues_t & vals)
  {

  possibleValues_ = vals; 
  // add the values to the tree store; 
  refTreeModel_->clear(); 
  
  int pos = 0; 
  for(typename possiblevalues_t::const_iterator i = vals.begin(); i != vals.end(); i++)
    {
      
      Gtk::TreeModel::Row row = *(refTreeModel_->append());
      row[modelColumns_.colID] = pos;
      
      row[modelColumns_.colName] = i->first; 
      row[modelColumns_.colVal] = i->second; 
      
      pos += 1; 
    }
  
  }
  
  template <class valuetype_t> 
  void ComboBox<valuetype_t>::delProperty(pProperty_t comboProperty)
  {
    if (propertySet_.find(comboProperty) != propertySet_.end()) {
      propertySet_.erase(comboProperty); 

    comboProperty->remove_watcher(notifyMap_[comboProperty]); 
    notifyMap_.erase(comboProperty); 

    }
    
  }
  
  template <class valuetype_t> 
  void ComboBox<valuetype_t>::refreshProperty() {
    /* 
       For each property, compute current value, and update
       
       Note: This is an asymptotic hate crime. Every time we get 
       any change, we iterate through. We're banking on: 
       - The actual number of elements will be very small (~10 worst case)
       - This is a fast operation
       
    */
    valuetype_t newvalue; 
    bool seen = false; 
    bool conflict = false; 
    for (typename propset_t::iterator  pi = propertySet_.begin(); 
	 pi != propertySet_.end(); pi++) {
      if (!seen) {
	newvalue = *( *pi); 
	seen = true; 
      } else {
	if (newvalue != *(*pi)) {
	  conflict = true; 
	} 
      }
    }
    
    if (conflict) {
      setState(CONFLICTED); 
    } else {
      value_ = newvalue; 
      setState(NORMAL); 
    }
    
  }
  
  template <class valuetype_t> 
  void ComboBox<valuetype_t>::setState(State st) {
    inSetState_ = true; 
    std::string statestring; 
    if (st == NORMAL) statestring = "NORMAL"; 
    if (st == PENDING) statestring = "PENDING"; 
    if (st == CONFLICTED) statestring = "CONFLICTED"; 
    

    if (st == NORMAL) {
      Gtk::TreeModel::Children c =  refTreeModel_->children(); 
      for(Gtk::TreeModel::Children::iterator ci = c.begin(); ci != c.end(); ci++) {
	Gtk::TreeModel::Row row = **ci;
	if (value_ == row[modelColumns_.colVal]) {
	  set_active(*ci); 
	  break; 
	} 
      }
      set_sensitive(true); 
    } else if (st == PENDING) { 
      Gtk::TreeModel::Children c =  refTreeModel_->children(); 
      for(Gtk::TreeModel::Children::iterator ci = c.begin(); ci != c.end(); ci++) {
	Gtk::TreeModel::Row row = **ci;
	if (value_ == row[modelColumns_.colVal]) {
	  set_active(*ci); 
	  break;
	}  

      }
      set_sensitive(false); 
      
    } else if (st == CONFLICTED) {
    
      set_sensitive(true); 
      set_active(-1); 
    }
    state_ = st; 

    inSetState_ = false; 
  }
  
  template <class valuetype_t> 
  void ComboBox<valuetype_t>::on_my_value_changed()
  {
    if (inSetState_) return;  // on_my_value_changed fires whenver the value changes, which can cause spurious triggerings. 



    Gtk::TreeModel::iterator iter = get_active();
    if(iter)
      {
	Gtk::TreeModel::Row row = *iter;
	if(row)
	  {
	    //       //Get the data for the selected row, using our knowledge of the tree
	    //       //model:
	    int id = row[modelColumns_.colID];
	    Glib::ustring name = row[modelColumns_.colName]; 
	    //	    std::cout << " ID=" << id << ", name=" << name << std::endl;
	    setState(PENDING); 
	    for (typename propset_t::iterator  pi = propertySet_.begin(); 
	     pi != propertySet_.end(); pi++) {
	      std::cout << "Attempting to set ComboBox" 
			<< row[modelColumns_.colVal] << std::endl; 
	      *(*pi) = row[modelColumns_.colVal]; 
	    }  
	  }
      } else { 
	//	std::cout << "invalid iter" << std::endl;
      }
    
    
  }
  
}

#endif // PROPERTY_COMBOBOX_H
