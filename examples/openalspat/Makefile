DEPS=clutter-1.0 spatosc-0.1 openal freealut liblo
CXXFLAGS=`pkg-config --cflags $(DEPS)` -Wall -Werror -Wfatal-errors
LIBS=`pkg-config --libs $(DEPS)`

all: openalspat

%.o : %.cpp
	$(CC) -c $(CXXFLAGS) $< -o $@

openalspat: main.o gui.o audio_scene.o application.o
	g++ -Wall -g $(LIBS) $^ -o $@

.PHONY: clean

clean:
	rm -f openalspat *.o

