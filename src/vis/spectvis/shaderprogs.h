#ifndef __SPECTVIS_SHADERPROG_H__
#define __SPECTVIS_SHADERPROG_H__


namespace spectvis { 

static const char* glstring_vertex = "void main()"
"{"
"gl_TexCoord[0] = gl_TextureMatrix[0]*  gl_MultiTexCoord0;"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"}"; 

static const std::string glstring_fragment = ""
  "uniform float alpha;"
  "uniform float maxval;"
  "uniform sampler2DRect tex;"
  "void main()"
  "{"
  "vec4 input = texture2DRect(tex, gl_TexCoord[0].st);"
  "float l = input[0];"
  "float lnorm = l / maxval;"
  "vec4 c;"
  "c.r = lnorm;"
  "c.g = lnorm;"
  "c.b = lnorm;"
  "c.a = 1.0;"
  "gl_FragColor =c ;"
  "}"; 

}
#endif
