INCLUDEPATH += $$PWD

PRECOMPILED_HEADER = $$PWD/stable.h

SOURCES += \
    $$PWD/StGen.cpp \
    $$PWD/Common.cpp \
    $$PWD/DataBaseInterface.cpp \
    $$PWD/QueryResult.cpp \
    $$PWD/Query.cpp \
    $$PWD/StGenGlobal.cpp

HEADERS += \
    $$PWD/StGen.h \
    $$PWD/Common.h \
    $$PWD/DataBaseInterface.h \
    $$PWD/QueryResult.h \
    $$PWD/Query.h \
    $$PWD/StGenGlobal.h
