#!/bin/bash

case $OS in
  linux)
  {
    cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE ../ -DCMAKE_PREFIX_PATH=/home/appveyor/Qt/5.14.2/gcc_64
  } ;;  
  macOS)
  {
	cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE ../ -DCMAKE_PREFIX_PATH=/usr/local/Cellar/qt/5*
  } ;;
esac