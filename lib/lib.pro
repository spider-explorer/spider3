#QT -= gui
CONFIG -= qt

TEMPLATE = lib
CONFIG += dll

CONFIG += c++17

SOURCES += \
    lib.cpp

HEADERS += \
    lib.h \
    utf8LogHandler.h

INCLUDEPATH += $$(HOME)/include

DESTDIR = $$PWD

#message($$QMAKE_QMAKE)
contains(QMAKE_QMAKE, .*\-static\/.*) {
    message( "[STATIC BUILD]" )
    DEFINES += QT_STATIC_BUILD
    TARGET = $${TARGET}-$${QMAKE_HOST.arch}-static
} else {
    message( "[SHARED BUILD]" )
    TARGET = $${TARGET}-$${QMAKE_HOST.arch}
}

#include($$(HOME)/qt/common/common.pri)
gcc {
    LIBS += -larchive -lz -lbz2 -llzma -liconv -lbcrypt -lexpat -lb2 -llz4 -lzstd
    LIBS += -lcrypto -lssl
    LIBS += -lws2_32
    LIBS += -ladvapi32 -lole32 -loleaut32 -luser32 -luuid
}
