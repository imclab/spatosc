#!/bin/bash
CFLAGS = `python-config --cflags` `pkg-config --cflags spatosc-0.3` -I.. -enable-lib64 -fPIC -Wall
LIBS = `python-config --libs` `pkg-config --libs spatosc-0.3` --enable-lib64 -fPIC -shared --start-group -lutil -lpthread --end-group -lboost_python-py26

all: spatosc.so

spatosc.o: python-spatosc.cpp
	g++ ${CFLAGS} -c -o python-spatosc.o python-spatosc.cpp

spatosc.so: spatosc.o
	g++ ${LIBS} -o spatosc.so  python-spatosc.o

install: spatosc.so
	install tempi.so /usr/local/lib/python2.6/dist-packages
	ldconfig

clean:
	rm -f *.so *.o

run: spatosc.so
	PYTHONPATH=${PWD} python example.py
