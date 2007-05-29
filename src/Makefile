
CXX =g++
CXXFLAGS = -O2 `pkg-config gtkmm-2.4 gdkmm-2.4 gtkglextmm-1.2 --cflags` -g

LDFLAGS = `pkg-config --libs gtkmm-2.4 gdkmm-2.4 gtkglextmm-1.2` -g -lboost_thread -lboost_date_time -lboost_program_options -lboost_regex

all: ratetimelinevis


ratetimelinevis: ratetimelinevis.o ratetimeline.o glconfig.o shaders.o 

clean:
	rm *.o test
