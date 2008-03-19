# 

AM_CXXFLAGS = $(GTKDEPS_CFLAGS)  $(BOOST_CPPFLAGS) $(SOMADEPS_CFLAGS)  -pthread 
AM_LDFLAGS = $(GTKDEPS_LIBS) $(BOOST_LDFLAGS) $(SOMADEPS_LIBS) $(BOOST_THREAD_LIB)  $(BOOST_PROGRAM_OPTIONS_LIB)


h_sources = glconfig.h glspikes.h \
	queueview.2.h queueview.h \
	shaders.h sourcestatus.h \
	streamcontrol.h streamsapp.h \
	streams.h streamsource.h \
	streamvis.h triggerwin.h \
	visproperty.h visstatus.h \
	wave.h wavestreamrenderer.h \
	wavestreamsource.h wavestreamsourcestatus.h \
	wavestreamtrigger.h wavestreamtriggerrenderer.h \
	wavestreamviscontrol.h wavestreamvis.h \
	wavestreamvisstatus.h wavewin.h \
	streamsourcebase.h streamsource.h \
	fakewave.h



cc_sources = glconfig.cc \
	shaders.cc streamcontrol.cc  \
	streamsapp.cc streamsmain.cc \
	triggerwin.cc visproperty.cc \
	wavestreamrenderer.cc wavestreamsource.cc \
	wavestreamsourcestatus.cc wavestreamtrigger.cc wavestreamtriggerrenderer.cc \
	wavestreamvis.cc wavestreamvisstatus.cc wavewin.cc \
	fakewave.cc

source_sources = fakewave.cc


bin_PROGRAMS = streams teststreams


#streams_SOURCES = $(cc_sources)

teststreams_SOURCES = teststreams.cc teststreamsapp.cc \
	$(source_sources) shaders.cc wavewin.cc triggerwin.cc \
	wavevis.cc timer.cc sourcecontrol.cc viscontrol.cc \
	glconfig.cc wavevisrenderer.cc wavevistrigger.cc \
	wavevistriggerrenderer.cc
