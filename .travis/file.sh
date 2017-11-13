#!/bin/bash

if [ $TRAVIS_OS_NAME = linux ]; then
    echo PokeFinder-linux.zip
else
    echo PokeFinder-macOS.zip
fi
