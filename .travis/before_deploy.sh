#!/bin/bash

if [ $TRAVIS_OS_NAME = linux ]; then
    zip PokeFinder-linux.zip PokeFinder
else
    /usr/local/Cellar/qt/5.9.1/bin/macdeployqt PokeFinder.app -dmg -verbose=2
    mv PokeFinder.dmg PokeFinder-macOS.dmg
fi
