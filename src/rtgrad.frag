uniform float rate; 
uniform float cutoff; 

varying vec4 passworld;

void main()
{		
	//  float rate = 1000.0; 
	//  float cutoff = 8000.0; 
  vec4 test = {1.0, 1.0, 1.0, 0.0}; 	
  if (passworld.x < cutoff)
  {	
    test.w = 1.0 - (cutoff - passworld.x)/rate; 
  }

  gl_FragColor = test; 
}	

