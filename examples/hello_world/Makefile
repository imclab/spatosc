DEPS=spatosc-0.3 liblo
CXXFLAGS=`pkg-config --cflags $(DEPS)` -g -Wall -Werror -Wfatal-errors -Wextra
LIBS=`pkg-config --libs $(DEPS)`
OBJS=hello.o

all: hello

%.o : %.cpp %.h
	$(CC) -c $(CXXFLAGS) $< -o $@

hello: $(OBJS)
	g++ -Wall -g $(LIBS) $^ -o $@

.PHONY: clean

clean:
	rm -f hello *.o

