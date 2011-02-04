DEPS=clutter-1.0 spatosc-0.1 openal freealut liblo
CXXFLAGS=`pkg-config --cflags $(DEPS)` -Wall -Werror -Wfatal-errors -Wextra -O2
LIBS=`pkg-config --libs $(DEPS)`
OBJS=main.o gui.o audio_scene.o application.o

ifeq ($(shell uname -s),Darwin)
  CXXFLAGS += -DDARWIN
endif


all: openalspat

%.o : %.cpp %.h
	$(CC) -c $(CXXFLAGS) $< -o $@

openalspat: $(OBJS)
	g++ -Wall -g $(LIBS) $^ -o $@

.PHONY: clean

clean:
	rm -f openalspat *.o

