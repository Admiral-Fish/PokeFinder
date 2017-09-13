#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    brew update
    brew install boost
    brew install qt5
else
    sudo apt-get -qq update
    sudo apt-get install -y build-essential qtbase5-dev qt5-qmake qt5-default libboost-dev libboost-system-dev
fi
