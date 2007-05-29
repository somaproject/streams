uniform int axes; 

varying vec4 passworld;
void main()
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; 
  passworld =   gl_Vertex; 
}
