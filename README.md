# PokéFinder

Join the PokéFinder Discord server to talk about development and contribute.

[![PokéFinder](https://discordapp.com/assets/07dca80a102d4149e9736d4b162cff6f.ico)](https://discord.gg/XmgQF9X)

This will be a RNG Tool for all main Pokémon games generations 3-7. It currently supports generation 3 and 4.

# Features
Gen 3
- Stationary
- Wild
- Egg
- IDs
- Gales/Colo/Channel

Gen 4
- Stationary
- Wild
- Egg
- IDs

# Installing

Windows
- Install the [Microsoft Visual C++ Redistributable](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)
- Download the win build(64bit or 32bit) zip folder from the [releases page](https://github.com/Admiral-Fish/PokeFinder/releases/latest)
- Extract PokéFinder from the zip folder

MacOS
- Download the macos zip folder from the [releases page](https://github.com/Admiral-Fish/PokeFinder/releases/latest)
- Extract PokéFinder from the zip folder

Linux
- Download the linux zip folder from the [releases page](https://github.com/Admiral-Fish/PokeFinder/releases/latest)
- Extract PokéFinder from the zip folder

# Building

Windows
- Install the dependencies
  - Download and install [Qt](https://www.qt.io/download)
  - [Build tools for Visual Studio](https://visualstudio.microsoft.com/downloads/)
- Build
  - git submodule update
  - qmake PokeFinder.pro
  - jom -j %NUMBER_OF_PROCESSORS%
- Bundle
  - mk PokeFinder-windows
  - move release\PokeFinder.exe PokeFinder-windows\PokeFinder.exe 
  - windeployqt --release --no-translations --no-angle --no-plugins --no-opengl-sw PokeFinder.exe
  - xcopy /I "QTPath"\plugins\platforms\qwindows.dll PokeFinder-windows\platforms\
  - xcopy /I "QTPath"\plugins\styles\qwindowsvistastyle.dll PokeFinder-windows\styles\

MacOS
- Install the dependencies
   - brew update
   - brew install qt5
   - brew link --force qt5
- Build
  - git submodule update
  - qmake PokeFinder.pro
  - make -j $(sysctl -n hw.physicalcpu)
- Bundle
  - macdeployqt PokeFinder.app -dmg -verbose=2

Linux
- Install the dependencies
  - sudo apt-get update -qq
  - sudo apt-get install -y build-essential qtbase5-dev qt5-qmake qt5-default
- Build
  - git submodule update
  - qmake PokeFinder.pro
  - make -j $(nproc)

# Credits
- Bill Young, Mike Suleski, and Andrew Ringer for [RNG Reporter](https://github.com/Slashmolder/RNGReporter)
- chiizu for [PPRNG](https://github.com/chiizu/PPRNG)
- wwwwwwzx for [3DSRNG Tool](https://github.com/wwwwwwzx/3DSRNGTool)
- Pokémon researchers(OmegaDonut, Bond697, Kaphotics, Zari, amab, etc)
- Sans for initial GUI design
