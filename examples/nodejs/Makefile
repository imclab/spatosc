all: hello.node

CFLAGS = -g  \
    -fPIC  \
    -DPIC  \
    -D_LARGEFILE_SOURCE  \
    -D_FILE_OFFSET_BITS=64  \
    -D_GNU_SOURCE  \
    -I/usr/include/nodejs \
    -DEV_MULTIPLICITY=0 

LDFLAGS = -lv8 -shared

hello.node: hello.o
	g++  ${LDFLAGS} hello.o -o hello.node

hello.o: hello.cpp
	g++  ${CFLAGS} -c -o hello.o hello.cpp

run: hello.node
	node ./test.js

clean:
	rm -f *.o *.node

