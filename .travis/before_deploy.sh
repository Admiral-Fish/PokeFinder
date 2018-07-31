#!/bin/bash

rm Languages/*.ts
mkdir PokeFinder.app/Contents/MacOS/Languages
cp Languages/*.qm PokeFinder.app/Contents/MacOS/Languages
/usr/local/Cellar/qt/5.11.1/bin/macdeployqt PokeFinder.app -dmg -verbose=2
zip -r PokeFinder-macOS.zip PokeFinder.dmg
