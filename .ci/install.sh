#!/bin/bash

case $OS in
  linux)
  {
    sudo sudo apt-get update
    sudo apt-get install -y libgl1-mesa-dev
  } ;;
  macOS)
  {
    brew update
    brew install qt5
    brew link --force qt5
  } ;;
esac