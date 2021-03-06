######################################################################
# Automatically generated by qmake (2.01a) dom 7. ott 16:51:11 2007
######################################################################

TEMPLATE = lib
TARGET = 
DEPENDPATH += . zip
INCLUDEPATH += . zip

CONFIG   += qt warn_off release staticlib
LANGUAGE	= C++


DESTDIR	= ../all_os_libs/
TARGET	= fop



QT += xml
QT += network
QT += svg 

MOC_DIR	= build/.moc
RCC_DIR	= build/.rcc
OBJECTS_DIR = build/.obj
UI_DIR	= ui


macx {
QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
CONFIG+=x86 ppc
}

# Input
HEADERS += BasicConfig.h \
           fop_handler.h \
           prefophandler.h \
           QmyDom.h \
           network_get.h \
           ziphandle.h \
           zip/crypt.h \
           zip/gojobzip.h \
           zip/ioapi.h \
           zip/quazip.h \
           zip/quazipfile.h \
           zip/quazipfileinfo.h \
           zip/quazipnewinfo.h \
           zip/unzip.h \
           zip/zip.h \
           zip/zlib.h
SOURCES += fop_handler.cpp \
           prefophandler.cpp \
           QmyDom.cpp \
           zip/ioapi.c \
           zip/quazip.cpp \
           zip/quazipfile.cpp \
           zip/quazipnewinfo.cpp \
           zip/unzip.c \
           zip/zip.c
