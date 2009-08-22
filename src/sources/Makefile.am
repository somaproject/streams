# 

AM_CXXFLAGS = $(GTKDEPS_CFLAGS)  $(BOOST_CPPFLAGS) $(SOMADEPS_CFLAGS)  -pthread \
		 -I@top_srcdir@/src/
AM_LDFLAGS = $(GTKDEPS_LIBS) $(BOOST_LDFLAGS) $(SOMADEPS_LIBS) $(BOOST_THREAD_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB)


h_sources =  noisewave.h
cc_sources = noisewave.cc sourcefactory.cc netdatawave.cc netdataraw.cc


noinst_LTLIBRARIES = libstreamsources.la

libstreamsources_la_SOURCES = $(cc_sources) 
libstreamsources_la_CXXFLAGS = $(AM_CXXFLAGS)

