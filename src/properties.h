#ifndef PROPERTIES_H
#define PROPERTIES_H
#include <iostream>
#include <sigc++/sigc++.h>

template <typename ValueType> 
class PropertyMirror; 


template <typename ValueType>
class Property
{
public: 

  Property(ValueType v) :
  verifyUpdate_(false) {
    value_ = v; 
  }
  

  Property() : 
  verifyUpdate_(false) {
    
  }
  
  Property(ValueType v, sigc::slot<bool, ValueType> cb) :
    value_(v),
    verifyUpdate_(true), 
    allowUpdateSlot_(cb)
  {
    
  }
  
  virtual operator ValueType() 
  {
    return getter(); 
  }
  
  ValueType operator =(const ValueType &  value) {
    if (verifyUpdate_) {
      if (!allowUpdateSlot_(value)) {
	return value; 
      }
    }
    setter(value); 
    return getter(); 
  }

  sigc::signal<void, ValueType> & signal() {
    return signal_; 
  }

protected:
  ValueType value_; 
  virtual void setter(ValueType value) {
    value_ = value; 
    signal_.emit(value_); 
  }

  virtual ValueType getter()
  {
    return value_; 
  }

  sigc::signal<void, ValueType> signal_; 
  sigc::slot<bool, ValueType> allowUpdateSlot_; 
  bool verifyUpdate_; 

  friend class PropertyMirror<ValueType>;

}; 

template <typename ValueType>
class PropertyMirror : public Property<ValueType>
{
public:
  PropertyMirror() :
    pSrcProp_(NULL)
  {

  }

  PropertyMirror(Property<ValueType> * srcprop) :
    pSrcProp_(srcprop )
  {
    
    
  }
  void setSource(Property<ValueType> * srcprop) {
    pSrcProp_ = srcprop; 
  }

  virtual operator ValueType() 
  {
    return pSrcProp_->getter(); 
  }

  ValueType operator =(const ValueType &  value) {
    return *pSrcProp_ = value; 
  }

  sigc::signal<void, ValueType> & signal() {
    return pSrcProp_->signal_; 
  }
    
private:
  Property<ValueType> * pSrcProp_; 

}; 

/*
  This is a simpler property, akin to python. It is constructed
  with two functors and a signal to connect to 

*/
template <typename ValueType>
class PropertyProxy : public Property<ValueType>
{
public:
  PropertyProxy()
  {
  }

  PropertyProxy(sigc::slot<void, ValueType> setter, 
		sigc::slot<ValueType> getter, 
		sigc::signal<void, ValueType> & cbsignal) : 
  setterSlot_(setter),
  getterSlot_(getter)
  {

    cbconn_ = cbsignal.connect(sigc::mem_fun(*this, 
					     &PropertyProxy<ValueType>::updateCallBack)); 
    
  }
  
  ~PropertyProxy() 
  {
    cbconn_.disconnect(); 
  }

  void resetProxy(sigc::slot<void, ValueType> setter, 
		sigc::slot<ValueType> getter, 
		sigc::signal<void, ValueType> & cbsignal) {
    cbconn_.disconnect(); 
    cbconn_ = cbsignal.connect(sigc::mem_fun(*this, 
					      &PropertyProxy<ValueType>::updateCallBack)); 
    setterSlot_ = setter; 
    getterSlot_ = getter; 

  }
protected:
  sigc::slot<void, ValueType> setterSlot_; 
  sigc::slot<ValueType> getterSlot_; 
  
  //  sigc::signal<void, ValueType> cbsignal_; 

  sigc::connection cbconn_; 

  virtual void setter(ValueType value) {
    setterSlot_(value); 
  }

  virtual ValueType getter()
  {
    return getterSlot_(); 
  }
  
  void updateCallBack(ValueType value)
  {
    this->signal_.emit(value); 
  }
  
}; 

#endif // PROPERTIES_H 
