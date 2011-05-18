DEPS=spatosc-0.2 liblo
CXXFLAGS=`pkg-config --cflags $(DEPS)` -g -Wall -Werror -Wfatal-errors -Wextra
LIBS=`pkg-config --libs $(DEPS)`
OBJS=using-the-wrapper.o

all: run

%.o : %.cpp %.h
	$(CC) -c $(CXXFLAGS) $< -o $@

run: $(OBJS)
	g++ -Wall -g $(LIBS) $^ -o $@

.PHONY: clean

clean:
	rm -f hello *.o

