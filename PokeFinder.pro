#-------------------------------------------------
#
# Project created by QtCreator 2017-07-28T23:02:16
#
#-------------------------------------------------

QT       += core gui
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


SOURCES += \
        main.cpp \
        Forms/mainwindow.cpp \
        Core/Objects/EncounterSlot.cpp \
        Core/Objects/Utilities.cpp \
        Core/Objects/Generator.cpp \
        Core/Objects/Frame.cpp \
        Core/Objects/FrameCompare.cpp \
        Core/Objects/Searcher.cpp \
        Core/Gen3/NatureLock.cpp \
        Core/Gen3/GeneratorGen3.cpp \
        Core/Gen3/FrameGen3.cpp \
        Core/Gen3/SearcherGen3.cpp \
        Core/RNG/LCRNG.cpp \
        Core/RNG/MTRNG.cpp \
        Core/RNG/SFMT.cpp \
        Core/RNG/TinyMT.cpp \
        Core/RNG/RNGCache.cpp \
        Core/RNG/RNGEuclidean.cpp

HEADERS += \
        Forms/mainwindow.h \
        Core/Objects/Encounter.hpp \
        Core/Objects/Lead.hpp \
        Core/Objects/Method.hpp \
        Core/Objects/EncounterSlot.hpp \
        Core/Objects/Utilities.hpp \
        Core/Objects/Generator.hpp \
        Core/Objects/Frame.hpp \
        Core/Objects/FrameCompare.hpp \
        Core/Objects/Searcher.hpp \
        Core/Gen3/NatureLock.hpp \
        Core/Gen3/GeneratorGen3.hpp \
        Core/Gen3/FrameGen3.hpp \
        Core/Gen3/SearcherGen3.hpp \
        Core/RNG/LCRNG.hpp \
        Core/RNG/MTRNG.hpp \
        Core/RNG/SFMT.hpp \
        Core/RNG/TinyMT.hpp \
        Core/RNG/RNGCache.hpp \
        Core/RNG/RNGEuclidean.hpp

FORMS += \
        Forms/mainwindow.ui
