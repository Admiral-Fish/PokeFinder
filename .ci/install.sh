#!/bin/bash

case $OS in
  linux)
  {
    sudo apt update
    sudo apt install -y libgl1-mesa-dev
  } ;;
esac