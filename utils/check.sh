#!/bin/bash
./autogen.sh
./configure
make 
make check
exit $?
