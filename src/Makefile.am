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
	wave.h wavevisrenderer.h \
	wavevissource.h wavestreamsourcestatus.h \
	wavevistrigger.h wavevistriggerrenderer.h \
	wavevisviscontrol.h wavevisvis.h \
	wavevisvisstatus.h wavewin.h \
	streamsourcebase.h streamsource.h \
	fakewave.h


cc_sources = glconfig.cc timer.cc viscontrol.cc \
	shaders.cc streamsapp.cc streamsmain.cc \
	triggerwin.cc wavewin.cc \
	sourcecontrolmonitor.cc viscontrolmonitor.cc \
	wavevisrenderer.cc wavevis.cc \
	wavevistrigger.cc wavevistriggerrenderer.cc \
	spectvis.cc spectvisrenderer.cc spectvistrigger.cc spectvistriggerrenderer.cc \
	wavevisstatus.cc sourcecontrol.cc  $(source_sources)

source_sources = fakewave.cc


bin_PROGRAMS = streams teststreams


streams_SOURCES = $(cc_sources)

teststreams_SOURCES = teststreams.cc teststreamsapp.cc \
	$(source_sources) shaders.cc wavewin.cc triggerwin.cc  timer.cc \
	wavevis.cc wavevisrenderer.cc wavevistrigger.cc wavevistriggerrenderer.cc \
	spectvis.cc spectvisrenderer.cc spectvistrigger.cc spectvistriggerrenderer.cc \
	glconfig.cc sourcecontrol.cc viscontrol.cc 
