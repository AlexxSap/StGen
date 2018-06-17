QT -= gui
QT += testlib sql

CONFIG += c++14 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += ../Core

SOURCES += \
        main.cpp \
    TSelect.cpp \
    TCommon.cpp

HEADERS += \
    TSelect.h \
    TCommon.h

include(../Core/Core.pri)
