#!/bin/bash

case $OS in
  linux)
  {
    sudo apt update
    sudo apt install -y g++-11 libgl1-mesa-dev
  } ;;
esac