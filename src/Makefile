
CXX =ccache g++
CXXFLAGS = -O9 `pkg-config gtkmm-2.4 gdkmm-2.4 gtkglextmm-1.2 sigc++-2.0 libglademm-2.4 --cflags` -g -msse3

LDFLAGS = `pkg-config --libs gtkmm-2.4 gdkmm-2.4 sigc++-2.0 gtkglextmm-1.2 libglademm-2.4` -g -lboost_thread -lboost_date_time -lboost_program_options -lboost_regex 
# -lboost_unit_test_framework


all:  streamsmain

streamcontroltest: streamcontrol.o

streamsource.o: streamsource.cc

bench: bench.o 

streamsmain: streamsmain.o streamsapp.o wavewin.o glconfig.o shaders.o wavestreamsource.o wavestreamvis.o wavestreamrenderer.o wavestreamtrigger.o triggerwin.o wavestreamtriggerrenderer.o wavestreamvisstatus.o wavestreamsourcestatus.o streamcontrol.o visproperty.o

clean:
	rm *.o
