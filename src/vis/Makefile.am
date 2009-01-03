# 

AM_CXXFLAGS = $(GTKDEPS_CFLAGS)  $(BOOST_CPPFLAGS) $(SOMADEPS_CFLAGS)  -pthread \
		 -I@top_srcdir@/src/
AM_LDFLAGS = $(GTKDEPS_LIBS) $(BOOST_LDFLAGS) $(SOMADEPS_LIBS) $(BOOST_THREAD_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB)


h_sources =  sinevis.h 
cc_sources = sinevis.cc wavevis.cc wavevisrenderer.cc visfactory.cc

noinst_LTLIBRARIES = libstreamvis.la

libstreamvis_la_SOURCES = $(cc_sources) 
libstreamvis_la_CXXFLAGS = $(AM_CXXFLAGS)

