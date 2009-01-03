# 

AM_CXXFLAGS = $(GTKDEPS_CFLAGS)  $(BOOST_CPPFLAGS) $(SOMADEPS_CFLAGS)  -pthread \
		 -I@top_srcdir@/src/
AM_LDFLAGS = $(GTKDEPS_LIBS) $(BOOST_LDFLAGS) $(SOMADEPS_LIBS) $(BOOST_THREAD_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB)


h_sources =  waveidentity.h
cc_sources = waveidentity.cc

noinst_LTLIBRARIES = libstreamfilters.la

libstreamfilters_la_SOURCES = $(cc_sources) 
libstreamfilters_la_CXXFLAGS = $(AM_CXXFLAGS)

