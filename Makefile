VERSION=0.1
CPPVERSION ?= -std=c++14
CXXFLAGS=-Wall $(CPPVERSION) -I. -MMD -MP 
CFLAGS=-Wall -O3 -MMD -MP
LDFLAGS=$(CXX2011FLAGS)  

PROGRAMS=doorbell-web

all: $(PROGRAMS)

-include *.d

doorbell-web: doorbell-web.o
	g++ $< -o $@ -lboost_system -pthread

doorbell-gpio: doorbell-gpio.o
	g++ $< -o $@ -lwiringPi

clean:
	rm -f *.o *~ $(PROGRAMS) *.d
	
	