DEPS=spatosc-0.1 liblo ode
CFLAGS=`pkg-config --cflags $(DEPS)`
LIBS=`pkg-config --libs $(DEPS)`

all: spat-ode

main.o: main.cpp
	g++ -Wall -g $(CFLAGS) main.cpp -c -o main.o

spat-ode: main.o
	g++ -Wall -g $(LIBS) main.o -o spat-ode

.PHONY: clean

clean:
	rm -f spat-ode *.o

