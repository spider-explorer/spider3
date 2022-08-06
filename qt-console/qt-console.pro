QT += core

CONFIG += c++17
CONFIG += console
CONFIG += force_debug_info

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += _WINSOCKAPI_

LIBS += -L$$[QT_INSTALL_PREFIX]/lib

gcc:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-function
msvc:QMAKE_LFLAGS += /LTCG:INCREMENTAL

SOURCES += main.cpp

INCLUDEPATH += $$PWD
INCLUDEPATH += $$(HOME)/include

DESTDIR = $$PWD/..

TARGET = $${TARGET}-$${QMAKE_HOST.arch}

#message($$QMAKE_QMAKE)
contains(QMAKE_QMAKE, .*static.*) {
    message( "[STATIC BUILD]" )
    DEFINES += QT_STATIC_BUILD
    TARGET = $${TARGET}-static
} else {
    message( "[SHARED BUILD]" )
}

#gcc:QMAKE_POST_LINK=strip $$DESTDIR/$(TARGET)

RESOURCES += \
    qt-console.qrc

#include($$(HOME)/qt/common/common.pri)

SUBDIRS += \
    template-all.pro
