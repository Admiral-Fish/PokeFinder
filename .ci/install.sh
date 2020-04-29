#!/bin/bash

case $OS in
  linux)
  {
    sudo add-apt-repository ppa:beineri/opt-qt-5.14.2-bionic -y && sudo apt-get update
    sudo apt-get install qt514base qt514svg qt514tools libgl1-mesa-dev -y
  } ;;
  macOS)
  {
    brew update
    brew install qt5
    brew link --force qt5
  } ;;
esac