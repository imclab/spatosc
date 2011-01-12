DEPS=clutter-1.0 spatosc openal freealut liblo
CFLAGS=`pkg-config --cflags $(DEPS)`
LIBS=`pkg-config --libs $(DEPS)`

all: openalspat

main.o: main.cpp
	g++ -Wall -g $(CFLAGS) main.cpp -c -o main.o

oscreceiver.o: oscreceiver.cpp
	g++ -Wall -g $(CFLAGS) oscreceiver.cpp -c -o oscreceiver.o

openalspat: main.o oscreceiver.o
	g++ -Wall -g $(LIBS) main.o oscreceiver.o -o openalspat

.PHONY: clean

clean:
	rm -f openalspat *.o

