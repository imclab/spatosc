DEPS=clutter-1.0 spatosc-0.1 openal freealut liblo
CXXFLAGS=`pkg-config --cflags $(DEPS)` -Wall -Werror -Wfatal-errors
LIBS=`pkg-config --libs $(DEPS)`
OBJS=main.o gui.o audio_scene.o application.o

all: openalspat

%.o : %.cpp %.h
	$(CC) -c $(CXXFLAGS) $< -o $@

openalspat: $(OBJS)
	g++ -Wall -g $(LIBS) $^ -o $@

.PHONY: clean

clean:
	rm -f openalspat *.o

