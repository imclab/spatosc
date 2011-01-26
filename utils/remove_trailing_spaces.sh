#!/bin/bash
# removes all trailing whitespaces from source files in spatosc
sed -i 's/[ \t]*$//' include/*.h src/*cpp tests/*.cpp
