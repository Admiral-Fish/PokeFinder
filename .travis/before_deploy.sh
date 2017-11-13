#!/bin/bash

if [ $TRAVIS_OS_NAME = linux ]; then
    zip -r PokeFinder-linux.zip PokeFinder languages
else
    zip -r PokeFinder-macOS.zip PokeFinder.app languages
fi
