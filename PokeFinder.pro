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
        languages/PokeFinder_ko.ts

SOURCES += \
        main.cpp \
        Forms/mainwindow.cpp \
        Core/Objects/EncounterSlot.cpp \
        Core/Objects/Frame.cpp \
        Core/Objects/FrameCompare.cpp \
        Core/Objects/Nature.cpp \
        Core/Objects/Power.cpp \
        Core/Objects/Utilities.cpp \
        Core/Gen3/NatureLock.cpp \
        Core/Gen3/GeneratorGen3.cpp \
        Core/Gen3/FrameGen3.cpp \
        Core/Gen3/SearcherGen3.cpp \
        Core/RNG/LCRNG.cpp \
        Core/RNG/LCRNG64.cpp \
        Core/RNG/MTRNG.cpp \
        Core/RNG/TinyMT.cpp \
        Core/RNG/RNGCache.cpp \
        Core/RNG/RNGEuclidean.cpp \
    Core/Gen3/ProfileGen3.cpp \
    Forms/ProfileManager/ProfileManagerGen3.cpp \
    Forms/ProfileManager/ProfileManagerGen3NewEdit.cpp \
    Util/Validators/IDValidator.cpp \
    Util/Validators/SeedValidator.cpp \
    Util/Validators/FrameValidator.cpp

HEADERS += \
        Forms/mainwindow.h \
        Core/Objects/Encounter.hpp \
        Core/Objects/Lead.hpp \
        Core/Objects/Method.hpp \
        Core/Objects/EncounterSlot.hpp \
        Core/Objects/Generator.hpp \
        Core/Objects/Frame.hpp \
        Core/Objects/FrameCompare.hpp \
        Core/Objects/Nature.hpp \
        Core/Objects/Power.hpp \
        Core/Objects/Searcher.hpp \
        Core/Objects/Utilities.hpp \
        Core/Gen3/NatureLock.hpp \
        Core/Gen3/GeneratorGen3.hpp \
        Core/Gen3/FrameGen3.hpp \
        Core/Gen3/SearcherGen3.hpp \
        Core/RNG/LCRNG.hpp \
        Core/RNG/LCRNG64.hpp \
        Core/RNG/MTRNG.hpp \
        Core/RNG/TinyMT.hpp \
        Core/RNG/RNGCache.hpp \
        Core/RNG/RNGEuclidean.hpp \
    Core/Objects/Profile.hpp \
    Core/Gen3/ProfileGen3.hpp \
    Forms/ProfileManager/ProfileManagerGen3.hpp \
    Forms/ProfileManager/ProfileManagerGen3NewEdit.hpp \
    Util/Validators/IDValidator.hpp \
    Util/Validators/SeedValidator.hpp \
    Util/Validators/FrameValidator.hpp

FORMS += \
        Forms/mainwindow.ui \
    Forms/ProfileManager/ProfileManagerGen3.ui \
    Forms/ProfileManager/ProfileManagerGen3NewEdit.ui
