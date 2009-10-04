



class TestElement()
{

  /*
    THIS IS ALL FINE, but WHAT ABOUT the ISSUE OF GLOBAL STATE? 

    1. state can always be read in a thread-safe manner, although it may change out from 
       under you

    2. attempts to change state are serialized into the main loop 
  
    3. state change notifications occur via... ? what, a pipe? 


  
  */ 

  // There's a guarantee that these will never be called simultaneously, a
  // although if we're paranoid we can always critical-region them. 

  
  void idle(process) {
    

  }

  void process(link, signal, done)
  {
    // this is where we change signals and push shit downstream
    
  }
  
  
}; 
