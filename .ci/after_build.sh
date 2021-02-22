#!/bin/bash

case $OS in
  linux)
  {
    zip -r PokeFinder-linux.zip Source/Forms/PokeFinder
    sha256sum PokeFinder-linux.zip > PokeFinder-linux.zip.sha256
  } ;;  
  macOS)
  {
    macdeployqt Source/Forms/PokeFinder.app -dmg -verbose=2
    zip -r PokeFinder-macOS.zip Source/Forms/PokeFinder.dmg
    shasum -a 256 PokeFinder-macOS.zip > PokeFinder-macOS.zip.sha256
  } ;;
esac