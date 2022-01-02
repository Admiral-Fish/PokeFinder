#!/bin/bash

case $OS in
  linux)
  {
    mv build/Source/Forms/PokeFinder .
    tar czf PokeFinder-linux.tar.gz PokeFinder
    sha256sum PokeFinder-linux.tar.gz > PokeFinder-linux.tar.gz.sha256
  } ;;  
  macOS)
  {
    mv build/Source/Forms/PokeFinder.app .
    PATH=$PATH:$HOME/Qt/6.1/macos/bin macdeployqt PokeFinder.app -no-plugins -verbose=3

    # Copy over plugins we need
    cp $HOME/Qt/6.1/macos/plugins/platforms/libqcococa.dylib PokeFinder.app/Contents/PlugIns/platforms
    cp $HOME/Qt/6.1/macos/plugins/styles/libqmacstyle.dylib PokeFinder.app/Contents/PlugIns/styles

    tar czf PokeFinder-macOS.tar.gz PokeFinder.app
    shasum -a 256 PokeFinder-macOS.tar.gz > PokeFinder-macOS.tar.gz.sha256
  } ;;
esac