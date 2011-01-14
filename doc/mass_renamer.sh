#!/bin/bash

# %s/xxx/vPlugin/g
# %s/yyy/Translator/g

git mv src/xxx.cpp src/yyy.cpp
git mv include/xxx.h include/yyy.h
sed -i "s/xxx.cpp/yyy.cpp/g" src/Makefile.am
for f in src/*cpp include/*h; do sed -i "s/xxx.h/yyy.h/g" $f; done
