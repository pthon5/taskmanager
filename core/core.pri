QT       += core sql
CONFIG   += c++11

INCLUDEPATH += $$PWD/include

SOURCES += $$PWD/src/statscore.cpp \
    $$PWD/src/hardwareinfo.cpp \
    $$PWD/src/linuxstatscore.cpp \
    $$PWD/src/macstatscore.cpp \
    $$PWD/src/genericstatscore.cpp \
    $$PWD/src/tempcore.cpp \


HEADERS  += $$PWD/include/statscore.h \
    $$PWD/src/hardwareinfo.h \
    $$PWD/src/linuxstatscore.h \
    $$PWD/src/macstatscore.h \
    $$PWD/src/genericstatscore.h \
    $$PWD/src/tempcore.h \
