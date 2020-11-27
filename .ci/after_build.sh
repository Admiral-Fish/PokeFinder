#!/bin/bash

case $OS in
  linux)
  {
    zip -r PokeFinder-linux.zip PokeFinder Core/Resources/i18n
    sha256sum PokeFinder-linux.zip > PokeFinder-linux.zip.sha256
  } ;;  
  macOS)
  {
	mv Core/Resources/i18n PokeFinder.app/Contents/MacOS/i18n
    macdeployqt PokeFinder.app -dmg -verbose=2
    zip -r PokeFinder-macOS.zip PokeFinder.dmg
    shasum -a 256 PokeFinder-macOS.zip > PokeFinder-macOS.zip.sha256
  } ;;
esac