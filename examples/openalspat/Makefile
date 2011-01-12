all: openalspat

openalspat: main.cpp
	g++ -Wall -g `pkg-config --cflags --libs clutter-1.0 spatosc openal freealut liblo` main.cpp -o openalspat
clean:
	rm -f openalspat *.o

