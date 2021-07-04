#!/bin/bash

case $OS in
  linux)
  {
    sudo sudo apt-get update
    sudo apt-get install -y libgl1-mesa-dev
  } ;;
esac