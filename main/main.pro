QT += core gui widgets network
equals(QT_MAJOR_VERSION, 6):QT += core5compat

CONFIG += c++17
CONFIG += force_debug_info

TEMPLATE  = lib
#CONFIG += staticlib
CONFIG += dll

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += _WINSOCKAPI_
DEFINES += DEBUG_LINE

LIBS += -L$$[QT_INSTALL_PREFIX]/lib

#PRECOMPILED_HEADER = stable.h

gcc:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-function
msvc:QMAKE_LFLAGS += /LTCG:INCREMENTAL

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_DEBUG

#LIBS += -larchive -lz -lbz2 -llzma -liconv -lbcrypt -lexpat -lb2 -llz4
LIBS += -ladvapi32 -lole32 -loleaut32 -luser32 -luuid
#LIBS += $$PWD/efsw/libefsw.a

SPIDER_VERSION = $$system(busybox date +%Y.%m.%d.%H.%M.%S)
message( $$SPIDER_VERSION )

DEFINES += SPIDER_VERSION=\\\"$$SPIDER_VERSION\\\"

SOURCES += \
    3rdparty/EasyOLE.cpp \
    binarysettings.cpp \
    chooseqtprojectdialog.cpp \
    cmdprocess.cpp \
    datetimemanager.cpp \
    entry.cpp \
    envvardialog.cpp \
    explorerform.cpp \
    favmanager.cpp \
    flowlayout.cpp \
    jsonsettings.cpp \
    mainwindow.cpp \
    msys2dialog.cpp \
    plainiconfsmodel.cpp \
    programdb.cpp \
    projectchecker.cpp \
    qsettings_binary.cpp \
    qsettings_json.cpp \
    recursivefilelister.cpp \
    repodialog.cpp \
    repositorydirdialog.cpp \
    repositorydirview.cpp \
    scoopsearchdialog.cpp \
    scoopsoftwaredialog.cpp \
    scoopsoftwarelist.cpp \
    searchthread.cpp \
    spidercore.cpp \
    spiderform.cpp \
    spidermain.cpp \
    spiderprocess.cpp \
    test01dialog.cpp \
    windowsutils.cpp \
    wslcore.cpp \
    wslfilesystemmodel.cpp

HEADERS += \
    3rdparty/EasyOLE.h \
    3rdparty/WinReg.hpp \
    3rdparty/prettyprint.hpp \
    binarysettings.h \
    chooseqtprojectdialog.h \
    cmdprocess.h \
    common.h \
    datetimemanager.h \
    envvardialog.h \
    explorerform.h \
    favmanager.h \
    flowlayout.h \
    jsonsettings.h \
    mainwindow.h \
    msys2dialog.h \
    plainiconfsmodel.h \
    programdb.h \
    projectchecker.h \
    qsettings_binary.h \
    qsettings_json.h \
    recursivefilelister.h \
    repodialog.h \
    repositorydirdialog.h \
    repositorydirview.h \
    scoopsearchdialog.h \
    scoopsoftwaredialog.h \
    scoopsoftwarelist.h \
    searchthread.h \
    spidercore.h \
    spiderform.h \
    spidermain.h \
    spiderprocess.h \
    stable.h \
    test01dialog.h \
    windowsutils.h \
    wslcore.h \
    wslfilesystemmodel.h

FORMS += \
    chooseqtprojectdialog.ui \
    envvardialog.ui \
    explorerform.ui \
    mainwindow.ui \
    msys2dialog.ui \
    repodialog.ui \
    repositorydirdialog.ui \
    scoopsearchdialog.ui \
    scoopsoftwaredialog.ui \
    spiderform.ui \
    spidermain.ui \
    test01dialog.ui

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

gcc:QMAKE_POST_LINK=strip $$DESTDIR/$(TARGET)

# https://www.flaticon.com/free-icon/tarantula_2959301?term=spider&page=1&position=7&page=1&position=7&related_id=2959301
# https://www.hipdf.com/jp/png-to-ico
RC_ICONS = tarantula.ico

RESOURCES += \
    spider2.qrc \
    vscode.qrc

include($$(HOME)/include/include.pri)
include($$(HOME)/qt/common/common.pri)

#include($$PWD/../lib/lib.pri)
#include($$PWD/../archive-api/archive-api.pri)

