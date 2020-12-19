#!/bin/bash

case $OS in
  linux)
  {
    cmake .
    make -j $(nproc)
  } ;;  
  macOS)
  {
    cmake .
    make -j $(sysctl -n hw.physicalcpu)
  } ;;
esac