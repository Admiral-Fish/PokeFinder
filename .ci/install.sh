#!/bin/bash

case $OS in
  linux)
  {
    sudo apt update
    sudo apt install -y g++-11 libgl1-mesa-dev

    python3 -m pip install requests

    # Change default g++ from 9 to 11, gets access to C++20 features
    # Should probably do this with update-alternatives but I am lazy
    sudo rm /usr/bin/g++
    sudo ln -s /usr/bin/g++-11 /usr/bin/g++
  } ;;
  macOS)
  {
    pip3.11 install requests --user
  } ;;
esac