## Makefile.am -- Process this file with automake to produce Makefile.in
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

AM_CXXFLAGS = $(BOOST_CPPFLAGS) -pthread 
AM_LDFLAGS = $(BOOST_LDFLAGS) $(BOOST_THREAD_LIB) $(BOOST_PROGRAM_OPTIONS_LIB)

