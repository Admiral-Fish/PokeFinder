#!/bin/bash

case $OS in
  linux)
  {
    source /opt/qt5*/bin/qt5*-env.sh
    qmake PokeFinder.pro
    make -j $(nproc)
  } ;;  
  macOS)
  {
    qmake PokeFinder.pro
    make -j $(sysctl -n hw.physicalcpu)
  } ;;
esac