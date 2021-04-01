#!/bin/sh

if [ ! -e ./ltmain.sh ]
then
    libtoolize
fi

./autogen.sh
./configure
make
