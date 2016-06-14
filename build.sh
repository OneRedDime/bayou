#!/bin/sh

./autogen.sh
./configure --prefix=/home/master/development/bayou
make
make install
mkdir -p bin
cd bin

if [ ! -e ./assets ]
then
    ln -s ../assets
fi
