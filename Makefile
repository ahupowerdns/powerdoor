VERSION=0.1
CXXFLAGS=-Wall -std=c++14 -I. -MMD -MP 
CFLAGS=-Wall -O3 -MMD -MP
LDFLAGS=$(CXX2011FLAGS)  

PROGRAMS=doorbell-web

all: $(PROGRAMS)

-include *.d

doorbell-web: doorbell-web.o
	g++ $< -o $@ -lboost_system -pthread

clean:
	rm -f *.o *~ $(PROGRAMS) *.d
	
	