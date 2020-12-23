#!/bin/bash

case $OS in
  linux)
  {
    cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE ../ -DCMAKE_PREFIX_PATH=/home/appveyor/Qt/5.14.2/gcc_64
  } ;;  
  macOS)
  {
	PATH="$(brew --prefix qt5)/bin:$PATH" cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE ../
  } ;;
esac