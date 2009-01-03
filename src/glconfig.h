#ifndef GLCONFIG_H
#define GLCONFIG_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <assert.h>

#include <stdlib.h>
#include <gtkmm.h>

#define GL_GLEXT_PROTOTYPES


#include <gtkglmm.h>
#include "logging.h"

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

//////////////////////////////////////////////////////////////////////////////
//
// OpenGL frame buffer configuration utilities.
//
//////////////////////////////////////////////////////////////////////////////

struct GLConfigUtil
{

  static void examine_gl_attrib(const Glib::RefPtr<const Gdk::GL::Config>& glconfig);
};

#endif // GLCONFIG_H
