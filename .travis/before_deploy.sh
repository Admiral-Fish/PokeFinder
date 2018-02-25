#!/bin/bash
rm Languages/*.ts
if [ $TRAVIS_OS_NAME = linux ]; then
    zip -r PokeFinder-linux.zip PokeFinder Languages
else
	/usr/local/Cellar/qt/5.10.1/bin/macdeployqt PokeFinder.app -verbose=2
	mkdir PokeFinder.app/Contents/MacOS/Languages
	cp Languages/*.qm PokeFinder.app/Contents/MacOS/Languages
	zip -r PokeFinder-macOS.zip PokeFinder.app
fi
