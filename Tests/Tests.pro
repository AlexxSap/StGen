QT -= gui
QT += testlib

CONFIG += c++14 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += ../Core

SOURCES += \
        main.cpp \
    TSelect.cpp

HEADERS += \
    TSelect.h

include(../Core/Core.pri)
