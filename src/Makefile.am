# 

AM_CXXFLAGS = $(GTKDEPS_CFLAGS)  $(BOOST_CPPFLAGS) $(SOMADEPS_CFLAGS) -Ilogging/src -Iext-common/ -pthread 
AM_LDFLAGS = $(GTKDEPS_LIBS) $(BOOST_LDFLAGS) $(SOMADEPS_LIBS) $(BOOST_THREAD_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB)



SUBDIRS= logging/src vis sources 
logging_cc_sources = 
cc_sources =  glconfig.cc timer.cc testviscontrol.cc \
	viscontrol.cc streamrenderwin.cc sourcebase.cc streampipeline.cc \
	somanetcodec.cc  networkdatacache.cc pipelinemanager.cc

ext_sources = ext-common/cairogltext/glstring.cc ext-common/shaderutil/shaders.cc

noinst_LTLIBRARIES = libstream.la

LIBADD = 

libstream_la_SOURCES = $(cc_sources) $(source_sources) $(ext_sources)
libstream_la_CXXFALGS = $(AM_CXXFLAGS)
libstream_la_LIBADD=  vis/libstreamvis.la sources/libstreamsources.la logging/src/liblogging.la


