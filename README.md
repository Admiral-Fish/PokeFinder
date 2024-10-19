# PokéFinder

Join the PokéFinder Discord server to talk about development and contribute.

[![PokéFinder](https://discordapp.com/assets/07dca80a102d4149e9736d4b162cff6f.ico)](https://discord.gg/XmgQF9X)

This will be a RNG Tool for all main Pokémon games generations 3-8. It currently supports generations 3/4 and parts of generation 5/8.

# Download

[Latest Official Release](https://github.com/Admiral-Fish/PokeFinder/releases/latest)

[Latest Nightly Build](https://github.com/Admiral-Fish/PokeFinder/actions)

# Features
Gen 3
- Egg
- GameCube
- IDs
- Static
- Wild

Gen 4
- Egg
- Event
- IDs
- Static
- Wild

Gen 5
- Dream Radar
- Egg
- Event
- Hidden Grotto
- IDs

Gen 8
- Egg
- Event
- IDs
- Raid
- Static
- Underground
- Wild

# Supported Platforms

Windows
- Windows 10
- Windows 11

MacOS
- MacOS Ventura
- MacOS Sonoma
- MacOS Sequoia

Linux
- Ubuntu 22.04
- Ubuntu 24.04

Qt
- 6.8 or newer

# Installing

Windows
- Install the [Microsoft Visual C++ Redistributable](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)
- Download the win zip folder from the [releases page](https://github.com/Admiral-Fish/PokeFinder/releases/latest)
- Extract PokéFinder from the zip folder

MacOS
- Download the macos zip folder from the [releases page](https://github.com/Admiral-Fish/PokeFinder/releases/latest)
- Extract PokéFinder from the zip folder

Linux
- Install Qt 6
  - [Qt website](https://www.qt.io/download)
  - sudo apt install qt6-base-dev
- Download the linux zip folder from the [releases page](https://github.com/Admiral-Fish/PokeFinder/releases/latest)
- Extract PokéFinder from the zip folder

# Building

Windows
- Install the dependencies
  - [Qt 6](https://www.qt.io/download)
  - [Build tools for Visual Studio](https://visualstudio.microsoft.com/downloads/)
- Build
  - git submodule update
  - mkdir build
  - cd build
  - cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=RELEASE ../
  - cmake --build .
- Bundle
  - mk PokeFinder-windows
  - move release\PokeFinder.exe PokeFinder-windows\PokeFinder.exe 
  - windeployqt --release --no-translations --no-angle --no-plugins --no-opengl-sw PokeFinder.exe
  - xcopy /I "QTPath"\plugins\platforms\qwindows.dll PokeFinder-windows\platforms\
  - xcopy /I "QTPath"\plugins\styles\qwindowsvistastyle.dll PokeFinder-windows\styles\

MacOS
- Install the dependencies
  - Qt 6 ([brew](https://formulae.brew.sh/formula/qt) or the [Qt website](https://www.qt.io/download))
- Build
  - git submodule update
  - mkdir build
  - cd build
  - PATH="PATH=$PATH:$HOME/Qt/6.8/macos/bin" cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE ../
    - Replace Qt path/version as necessary
  - cmake --build .
- Bundle
  - macdeployqt PokeFinder.app -dmg -verbose=2

Linux
- Install the dependencies
  - Qt 6
    - [Qt website](https://www.qt.io/download)
    - sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools
  - sudo apt install build-essential libgl1-mesa-dev
- Build
  - git submodule update
  - mkdir build
  - cd build
  - cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_PREFIX_PATH=$HOME/Qt/6.8/gcc_64 ../
    - Replace Qt path/version as necessary
  - cmake --build .

# Credits (in no particular order)
- Bill Young, Mike Suleski, and Andrew Ringer for [RNG Reporter](https://github.com/Slashmolder/RNGReporter)
- chiizu for [PPRNG](https://github.com/chiizu/PPRNG)
- wwwwwwzx for [3DSRNG Tool](https://github.com/wwwwwwzx/3DSRNGTool)
- The PokemonRNG team for various contributions and research [zaksabeast](https://github.com/zaksabeast), [EzPzstreamz](https://github.com/SteveCookTU), [Shiny_Sylveon](https://github.com/ShinySylveon04), [Vlad](https://github.com/RichardPaulAstley), [Real96](https://github.com/Real96)
- Other great people for various help and research ([OmegaDonut](https://github.com/OmegaDonut), [Bond697](https://github.com/Bond697), [Kaphotics](https://github.com/kwsch), [SciresM](https://github.com/SciresM), Zari, amab, Marin, [Lean](https://github.com/Leanny), etc)
- Sans for initial GUI design
