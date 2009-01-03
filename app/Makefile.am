# 

AM_CXXFLAGS = $(GTKDEPS_CFLAGS)  $(BOOST_CPPFLAGS) $(SOMADEPS_CFLAGS) -I$(top_builddir)/src  -I$(top_builddir)/src/ext-common/  -I$(top_builddir)/src/logging/src -pthread 
AM_LDFLAGS = $(GTKDEPS_LIBS) $(BOOST_LDFLAGS) $(SOMADEPS_LIBS) $(BOOST_THREAD_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB)

SUBDIRS = propwidgets

logging_cc_sources = 
gui_cc_sources = streamsapp.cc pipelinemanagergui.cc pipelinegui.cc \
	  filtercreator.c triggercreator.cc sourcecreator.cc viscreator.cc \
	propertypanemanager.cc  elementstatus.cc
widgets_cc_sources = noisewavewidget.cc waveviswidget.cc netdatawavewidget.cc
props_cc_sources = noisewaveproperty.cc wavevisproperty.cc netdatawaveproperty.cc

LIBADD = 

bin_PROGRAMS = soma-streams
soma_streams_SOURCES = streamsmain.cc $(gui_cc_sources) $(widgets_cc_sources) \
	$(props_cc_sources)
soma_streams_LDADD = $(top_builddir)/src/libstream.la  $(top_builddir)/app/propwidgets/libpropwidgets.la


$(top_builddir)/src/libstream.la:
	cd $(top_builddir)/src && $(MAKE) $(AM_MAKEFLAGS) libstream.la
.PHONY: $(top_builddir)/src/libstream.la

