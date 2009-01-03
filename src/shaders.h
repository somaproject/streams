#ifndef SHADERS_H
#define SHADERS_H
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <vector>
#include <list>

//
// options are GL_VERTEX_SHADER and GL_FRAGMENT_SHADER

GLuint loadGPUShader(const std::string & shaderSrc, GLuint shaderType); 
GLuint loadGPUShaderFromFile(const std::string & filename, GLuint shaderType); 

GLuint  createGPUProgram(const std::list<GLuint> &  shaders); 

GLuint useGPUProgram(GLuint gpuProg); 

#endif // SHADERS_H
