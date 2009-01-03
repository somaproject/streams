#include "shaders.h"
#include <iostream>
#include <fstream>

using namespace std; 

GLuint loadGPUShader(const std::string & shaderSrc, GLuint shaderType)
{
  GLuint shdr = glCreateShader(shaderType); 
  
  char * program[1];
  program[0] = new char[shaderSrc.size() + 10];
  strcpy(program[0], shaderSrc.c_str()); 


  glShaderSource(shdr, 1, (const GLchar**) program, NULL); 
  glCompileShader(shdr); 
  GLint param ; 
  glGetShaderiv(shdr, GL_COMPILE_STATUS, &param); 
  if (param == GL_TRUE) {
    //cout << "Successfully compiled vertex shader" << endl; 
  } else {
    
    cout << "Error compiling vertex shader" << endl; 
    char logmessage[1000];
    int loglen; 
    glGetShaderInfoLog(shdr, 1000, &loglen, logmessage); 
    cerr << logmessage << endl; 
    exit(1);
  }
  return shdr; 
}



GLuint loadGPUShaderFromFile(const std::string & filename, GLuint shaderType)
{
  ifstream shaderFile; 
  shaderFile.open(filename.c_str()); 

  string shaderSrc = ""; 
  string line = ""; 
  while (shaderFile and not shaderFile.eof()) {
    
    getline(shaderFile, line); 
    shaderSrc += line + '\n'; 
  };
  shaderFile.close();

  return loadGPUShader(shaderSrc, shaderType); 
}

GLuint createGPUProgram(const std::list<GLuint> & shaders)
{
  
  GLuint gpuProg = glCreateProgram(); 
  for (std::list<GLuint>::const_iterator i = shaders.begin(); 
       i != shaders.end(); i++) 
    {
      glAttachShader(gpuProg, *i); 
    }
  
  glLinkProgram(gpuProg);


  int progLogLen;
  char progLog[1000]; 
  
  GLint param ; 
  glGetProgramiv(gpuProg, GL_LINK_STATUS, &param); 
  
  if (param != GL_TRUE) {
    glGetProgramInfoLog(gpuProg, 10000, &progLogLen, progLog); 
    cout << progLog << endl; 
    exit(1); 
  }

  return gpuProg; 

}

GLuint useGPUProgram(GLuint gpuProg)
{
  glUseProgram(gpuProg); 

}
