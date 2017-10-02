#!/bin/bash
BASEDIR=$(pwd)


echo "Compiling" $BASEDIR/SFrame
cd $BASEDIR/SFrame
cd ..
make

for f in `ls -d */`
do 
    echo "Compiling" $BASEDIR/$f
    cd $BASEDIR/$f
    make
    cd ..
done