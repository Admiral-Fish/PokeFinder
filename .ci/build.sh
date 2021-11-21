#!/bin/bash

case $OS in
  linux)
  {
    cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_PREFIX_PATH=$HOME/Qt/6.1/gcc_64 -DTEST=ON ../
    make -j $(nproc)
  } ;;  
  macOS)
  {
    PATH=$PATH:$HOME/Qt/6.1/macos/bin cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DTEST=ON ../
    make -j $(sysctl -n hw.physicalcpu)
  } ;;
esac