#!/bin/bash

case $OS in
  linux)
  {
    cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_PREFIX_PATH=/home/appveyor/Qt/5.14.2/gcc_64 -DTEST=ON ../ 
  } ;;  
  macOS)
  {
	  PATH=$PATH:$HOME/Qt/5.15/clang_64/bin cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DTEST=ON ../
  } ;;
esac