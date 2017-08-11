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
    INCLUDEPATH += C:/boost/include/boost-1_64
}

macx {
    _BOOST_PATH = /usr/local/Cellar/boost/1.63.0
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
        Core/Objects/LCRNG.cpp \
        Core/Objects/MTRNG.cpp \
        Core/Objects/NatureLock.cpp \
        Core/Objects/Utilities.cpp \
        Core/Objects/SFMT.cpp \
        Core/Objects/TinyMT.cpp \
        Core/Gen3/StationaryGen3.cpp

HEADERS += \
        Forms/mainwindow.h \
        Core/Objects/Encounter.hpp \
        Core/Objects/EncounterSlot.hpp \
        Core/Objects/LCRNG.hpp \
        Core/Objects/Lead.hpp \
        Core/Objects/Method.hpp \
        Core/Objects/MTRNG.hpp \
        Core/Objects/NatureLock.hpp \
        Core/Objects/Utilities.hpp \
        Core/Objects/SFMT.hpp \
        Core/Objects/TinyMT.hpp \
        Core/Gen3/StationaryGen3.hpp

FORMS += \
        Forms/mainwindow.ui
