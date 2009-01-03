# 

AM_CXXFLAGS = $(GTKDEPS_CFLAGS)  $(BOOST_CPPFLAGS) $(SOMADEPS_CFLAGS)  -pthread \
		 -I@top_srcdir@/src/
AM_LDFLAGS = $(GTKDEPS_LIBS) $(BOOST_LDFLAGS) $(SOMADEPS_LIBS) $(BOOST_THREAD_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB)

noinst_PROGRAMS = streamstest

streamstest_SOURCES = streamstest.cc
streamstest_LDADD =  $(top_builddir)/src/libstream.la

