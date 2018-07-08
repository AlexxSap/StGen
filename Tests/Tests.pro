QT -= gui
QT += testlib sql

CONFIG += c++14 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += ../Core

SOURCES += \
        main.cpp \
    Utils.cpp \
    TSelect.cpp \
    TCommon.cpp \
    TBase.cpp \
    TCreate.cpp \
    TInsert.cpp \
    TJoin.cpp

HEADERS += \
    Utils.h \
    TSelect.h \
    TCommon.h \
    TBase.h \
    TCreate.h \
    TInsert.h \
    TJoin.h

include(../Core/Core.pri)
