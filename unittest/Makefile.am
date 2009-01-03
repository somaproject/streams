# 


AM_LDFLAGS = $(GTKDEPS_LIBS) $(BOOST_LDFLAGS) $(SOMADEPS_LIBS) \
	$(BOOST_THREAD_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB) \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIB)


AM_CXXFLAGS = $(GTKDEPS_CFLAGS) -I@top_srcdir@/src/  $(BOOST_CPPFLAGS) $(SOMADEPS_CFLAGS)  -pthread 

check_PROGRAMS = runner

test_sources = queueview_test.cc sourcepad_test.cc SinkPad.cc \
	sourcesink_test.cc Element.cc StreamPipeline.cc \
	PipelineManager.cc SomaNetCodec.cc NetworkDataCache.cc


# Element.cc StreamPipeline.cc
#	sourcesink_test.cc Element.cc StreamPipeline.cc


runner_SOURCES =  runner.cc  $(test_sources)
runner_LDADD = $(top_builddir)/src/libstream.la



$(top_builddir)/src/libstream.la:
	cd $(top_builddir)/src && $(MAKE) $(AM_MAKEFLAGS) libstream.la
.PHONY: $(top_builddir)/src/libstream.la

#http://www.gnu.org/software/autoconf/manual/autoconf-2.57/html_chapter/autoconf_17.html#SEC179  FAQ entry for how to handle the data directory


