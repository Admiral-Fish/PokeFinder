#!/bin/bash

case $OS in
  linux)
  {
    mv build/Source/Form/PokeFinder .
    tar czf PokeFinder-linux.tar.gz PokeFinder
    sha256sum PokeFinder-linux.tar.gz > PokeFinder-linux.tar.gz.sha256
  } ;;
  macOS)
  {
    mv build/Source/Form/PokeFinder.app .
    PATH=$PATH:$HOME/Qt/$QTVER/macos/bin macdeployqt PokeFinder.app -no-plugins -verbose=3

    # Fix rpath
    install_name_tool -rpath /Users/appveyor/Qt/$QTVER/macos/lib @executable_path/../Frameworks PokeFinder.app/Contents/MacOS/PokeFinder

    # Create plugins folder
    mkdir PokeFinder.app/Contents/PlugIns
    mkdir PokeFinder.app/Contents/PlugIns/platforms
    mkdir PokeFinder.app/Contents/PlugIns/styles

    # Copy over plugins we need
    cp $HOME/Qt/$QTVER/macos/plugins/platforms/libqcocoa.dylib PokeFinder.app/Contents/PlugIns/platforms
    cp $HOME/Qt/$QTVER/macos/plugins/styles/libqmacstyle.dylib PokeFinder.app/Contents/PlugIns/styles

    tar czf PokeFinder-macOS.tar.gz PokeFinder.app
    shasum -a 256 PokeFinder-macOS.tar.gz > PokeFinder-macOS.tar.gz.sha256
  } ;;
esac
