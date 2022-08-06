#CONFIG -= qt
QT += core

TEMPLATE = lib
CONFIG += dll

CONFIG += c++17

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += DEBUG_LINE
DEFINES += _WINSOCKAPI_

gcc:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-function

SOURCES += \
    archive-api.cpp

HEADERS +=

INCLUDEPATH += $$(HOME)/include

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_DEBUG

DESTDIR = $$PWD/..

#message($$QMAKE_QMAKE)
contains(QMAKE_QMAKE, .*\-static\/.*) {
    message( "[STATIC BUILD]" )
    DEFINES += QT_STATIC_BUILD
    TARGET = $${TARGET}-$${QMAKE_HOST.arch}-static
} else {
    message( "[SHARED BUILD]" )
    TARGET = $${TARGET}-$${QMAKE_HOST.arch}
}

gcc:QMAKE_POST_LINK=strip $$DESTDIR/$(TARGET)

#include($$(HOME)/include/include.pri)
include($$(HOME)/qt/common/common.pri)
gcc {
    LIBS += -larchive -lz -lbz2 -llzma -liconv -lbcrypt -lexpat -lb2 -llz4 -lzstd
    LIBS +=-lssl -lcrypto
    LIBS += -lws2_32
    LIBS += -ladvapi32 -lole32 -loleaut32 -luser32 -luuid
}
