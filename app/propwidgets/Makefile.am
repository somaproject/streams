# 

AM_CXXFLAGS = $(GTKDEPS_CFLAGS)  $(BOOST_CPPFLAGS) $(SOMADEPS_CFLAGS) -I$(top_builddir)/src  -I$(top_builddir)ext/logging/src -pthread 
AM_LDFLAGS = $(GTKDEPS_LIBS) $(BOOST_LDFLAGS) $(SOMADEPS_LIBS) $(BOOST_THREAD_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB)


LIBADD = 

bin_PROGRAMS = test
test_SOURCES = test.cc spinbutton.cc 
#testLDADD = $(top_builddir)/src/libstream.la $

noinst_LTLIBRARIES = libpropwidgets.la

libpropwidgets_la_SOURCES = spinbutton.cc
libpropwidgets_la_CXXFLAGS = $(AM_CXXFLAGS)

$(top_builddir)/src/libstream.la:
	cd $(top_builddir)/src && $(MAKE) $(AM_MAKEFLAGS) libstream.la
.PHONY: $(top_builddir)/src/libstream.la

