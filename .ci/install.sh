#!/bin/bash

case $OS in
  linux)
  {
    sudo apt update
    sudo apt install -y g++ libgl1-mesa-dev

    # Change default g++ from 7.5 to 9.3, gets access to <filesystem>
    # Should probably do this with update-alternatives but I am lazy
    sudo rm /usr/bin/g++
    sudo ln -s /usr/bin/g++-9 /usr/bin/g++
  } ;;
esac