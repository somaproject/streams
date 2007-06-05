
CXX =g++
CXXFLAGS = -O2 `pkg-config gtkmm-2.4 gdkmm-2.4 gtkglextmm-1.2 --cflags` -g -msse3

LDFLAGS = `pkg-config --libs gtkmm-2.4 gdkmm-2.4 gtkglextmm-1.2` -g -lboost_thread -lboost_date_time -lboost_program_options -lboost_regex -lboost_unit_test_framework 

all:  testrunner

streamsource.o: streamsource.cc

bench: bench.o 

wavestest: wavestest.o wavewin.o glconfig.o shaders.o wave.o rendercontrol.o

testrunner : testrunner.cc queueview_test.o

clean:
	rm *.o
