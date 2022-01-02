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
    PATH=$PATH:$HOME/Qt/6.1/macos/bin macdeployqt PokeFinder.app -verbose=3

    # Remove libraries we don't need
    rm -r PokeFinder.app/Contents/Frameworks/QtDBus.framework
    rm -r PokeFinder.app/Contents/Frameworks/QtOpenGL.framework
    rm -r PokeFinder.app/Contents/Frameworks/QtQml.framework
    rm -r PokeFinder.app/Contents/Frameworks/QtQmlModels.framework
    rm -r PokeFinder.app/Contents/Frameworks/QtQuick.framework
    rm -r PokeFinder.app/Contents/Frameworks/QtSvg.framework
    rm -r PokeFinder.app/Contents/Frameworks/QtVirtualKeyboard.framework

    # Remove plugins we don't need
    rm -r PokeFinder.app/Contents/PlugIns/iconengines
    rm -r PokeFinder.app/Contents/PlugIns/imageformats
    rm -r PokeFinder.app/Contents/PlugIns/platforminputcontexts
    rm -r PokeFinder.app/Contents/PlugIns/virtualkeyboard

    tar czf PokeFinder-macOS.tar.gz PokeFinder.app
    shasum -a 256 PokeFinder-macOS.tar.gz > PokeFinder-macOS.tar.gz.sha256
  } ;;
esac