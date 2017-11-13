#-------------------------------------------------
#
# Project created by QtCreator 2017-07-28T23:02:16
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokeFinder
TEMPLATE = app
VERSION = 1.1.1
QMAKE_TARGET_DESCRIPTION = PokeFinder
QMAKE_TARGET_COPYRIGHT = Admiral_Fish

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Paths to boost libraries.

linux {
    LIBS += \
       -lboost_system\
}

win32 {
    INCLUDEPATH += C:\Libraries\boost_1_63_0
    INCLUDEPATH += C:\Libraries\boost_1_64_0
    INCLUDEPATH += C:\boost\include\boost-1_64
}

macx {
    _BOOST_PATH = /usr/local/Cellar/boost/1.65.1
        INCLUDEPATH += "$${_BOOST_PATH}/include/"
        LIBS += -L$${_BOOST_PATH}/lib
}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += \
    languages/PokeFinder_en.ts \
    languages/PokeFinder_fr.ts \
    languages/PokeFinder_es.ts \
    languages/PokeFinder_de.ts \
    languages/PokeFinder_it.ts \
    languages/PokeFinder_ja.ts \
    languages/PokeFinder_ko.ts \
    languages/PokeFinder_zh_Hans_CN.ts

SOURCES += \
    Forms/ProfileManager/ProfileManagerGen3.cpp \
    Forms/ProfileManager/ProfileManagerGen3NewEdit.cpp \
    Forms/mainwindow.cpp \
    libPokeFinder/Gen3/FrameGen3.cpp \
    libPokeFinder/Gen3/GeneratorGen3.cpp \
    libPokeFinder/Gen3/NatureLock.cpp \
    libPokeFinder/Gen3/ProfileGen3.cpp \
    libPokeFinder/Gen3/SearcherGen3.cpp \
    libPokeFinder/Objects/EncounterSlot.cpp \
    libPokeFinder/Objects/Frame.cpp \
    libPokeFinder/Objects/FrameCompare.cpp \
    libPokeFinder/Objects/Nature.cpp \
    libPokeFinder/Objects/Power.cpp \
    libPokeFinder/Objects/Utilities.cpp \
    libPokeFinder/RNG/LCRNG.cpp \
    libPokeFinder/RNG/LCRNG64.cpp \
    libPokeFinder/RNG/MTRNG.cpp \
    libPokeFinder/RNG/RNGCache.cpp \
    libPokeFinder/RNG/RNGEuclidean.cpp \
    libPokeFinder/RNG/TinyMT.cpp \
    Util/Validators/FrameValidator.cpp \
    Util/Validators/IDValidator.cpp \
    Util/Validators/SeedValidator.cpp \
    main.cpp \
    libPokeFinder/RNG/SFMT.cpp

HEADERS += \
    Forms/ProfileManager/ProfileManagerGen3.hpp \
    Forms/ProfileManager/ProfileManagerGen3NewEdit.hpp \
    Forms/mainwindow.h \
    libPokeFinder/Gen3/FrameGen3.hpp \
    libPokeFinder/Gen3/GeneratorGen3.hpp \
    libPokeFinder/Gen3/NatureLock.hpp \
    libPokeFinder/Gen3/ProfileGen3.hpp \
    libPokeFinder/Gen3/SearcherGen3.hpp \
    libPokeFinder/Objects/Encounter.hpp \
    libPokeFinder/Objects/EncounterSlot.hpp \
    libPokeFinder/Objects/Frame.hpp \
    libPokeFinder/Objects/FrameCompare.hpp \
    libPokeFinder/Objects/Generator.hpp \
    libPokeFinder/Objects/Lead.hpp \
    libPokeFinder/Objects/Method.hpp \
    libPokeFinder/Objects/Nature.hpp \
    libPokeFinder/Objects/Power.hpp \
    libPokeFinder/Objects/Profile.hpp \
    libPokeFinder/Objects/Searcher.hpp \
    libPokeFinder/Objects/Utilities.hpp \
    libPokeFinder/RNG/LCRNG.hpp \
    libPokeFinder/RNG/LCRNG64.hpp \
    libPokeFinder/RNG/MTRNG.hpp \
    libPokeFinder/RNG/RNGCache.hpp \
    libPokeFinder/RNG/RNGEuclidean.hpp \
    libPokeFinder/RNG/TinyMT.hpp \
    Util/Validators/FrameValidator.hpp \
    Util/Validators/IDValidator.hpp \
    Util/Validators/SeedValidator.hpp \
    libPokeFinder/RNG/IRNG.hpp \
    libPokeFinder/RNG/SFMT.hpp \
    libPokeFinder/RNG/IRNG64.hpp

FORMS += \
    Forms/ProfileManager/ProfileManagerGen3.ui \
    Forms/ProfileManager/ProfileManagerGen3NewEdit.ui \
    Forms/mainwindow.ui
