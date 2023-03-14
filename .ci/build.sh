#!/bin/bash

case $OS in
  linux)
  {
    cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_PREFIX_PATH=$HOME/Qt/$QTVER/gcc_64 -DTEST=ON ../
  } ;;
  macOS)
  {
    PATH=$PATH:$HOME/Qt/$QTVER/macos/bin cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DTEST=ON ../
  } ;;
esac