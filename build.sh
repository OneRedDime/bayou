#!/bin/sh

./autogen.sh
./configure --prefix=/home/master/development/bayou
make
make install
mkdir -p bin
cd bin
ln -s ../assets
