#!/bin/sh
rm -rf autom4te.cache
libtoolize --force --copy &&  aclocal &&  autoheader &&  automake --add-missing --force-missing --foreign --copy &&  autoconf
