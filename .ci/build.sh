#!/bin/bash

case $OS in
  linux)
  {
    $HOME/Qt/5.12/gcc_64/bin/qmake PokeFinder.pro
    make -j $(nproc)
  } ;;  
  macOS)
  {
    qmake PokeFinder.pro
    make -j $(sysctl -n hw.physicalcpu)
  } ;;
esac