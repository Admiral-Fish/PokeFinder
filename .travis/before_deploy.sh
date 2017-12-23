#!/bin/bash
rm Languages/*.ts
if [ $TRAVIS_OS_NAME = linux ]; then
    zip -r PokeFinder-linux.zip PokeFinder Languages
else
    /usr/local/Cellar/qt/5.10.0/bin/macdeployqt PokeFinder.app -dmg -verbose=2
    zip -r PokeFinder-macOS.zip PokeFinder.dmg Languages
fi
