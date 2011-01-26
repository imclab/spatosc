#!/bin/bash

# export CC=i586-mingw32msvc-gcc
# export CXX=i586-mingw32msvc-c++
# export LD=i586-mingw32msvc-ld
# export AR=i586-mingw32msvc-ar
# export AS=i586-mingw32msvc-as
# export NM=i586-mingw32msvc-nm
# export STRIP=i586-mingw32msvc-strip
# export RANLIB=i586-mingw32msvc-ranlib
# export DLLTOOL=i586-mingw32msvc-dlltool
# export OBJDUMP=i586-mingw32msvc-objdump
# export RESCOMP=i586-mingw32msvc-windres

# ./configure --build=i686-pc-linux-gnu --host=i586-mingw32msvc --prefix=/usr/i586-mingw32msvc

make distclean 
./configure --host=i586-mingw32msvc --prefix=/usr/i586-mingw32msvc
make


