#!/bin/bash
rm Languages/*.ts
if [ $TRAVIS_OS_NAME = linux ]; then
	zip -r PokeFinder-linux.zip PokeFinder Languages
else
	mkdir PokeFinder.app/Contents/MacOS/Languages
	cp Languages/*.qm PokeFinder.app/Contents/MacOS/Languages
	/usr/local/Cellar/qt/5.11.0/bin/macdeployqt PokeFinder.app -dmg -verbose=2
	zip -r PokeFinder-macOS.zip PokeFinder.dmg
fi
