######################################################################
# Automatically generated by qmake (2.01a) gio 8. mag 00:13:01 2008
######################################################################

TEMPLATE = lib
TARGET	= tidy
DEPENDPATH += .
INCLUDEPATH += .


include( ../../mac_univ.pri )

CONFIG   += qt warn_off release staticlib
LANGUAGE	= C++

MOC_DIR	= build/.moc
RCC_DIR	= build/.rcc
OBJECTS_DIR = build/.obj

CONFIG += create_prl 

DESTDIR	= ../../lib/





# Input
HEADERS += access.h \
           attrdict.h \
           attrs.h \
           buffio.h \
           charsets.h \
           clean.h \
           config.h \
           entities.h \
           fileio.h \
           forward.h \
           iconvtc.h \
           lexer.h \
           message.h \
           parser.h \
           platform.h \
           pprint.h \
           streamio.h \
           tags.h \
           tidy-int.h \
           tidy.h \
           tidyenum.h \
           tmbstr.h \
           utf8.h \
           version.h \
           win32tc.h
SOURCES += access.c \
           alloc.c \
           attrask.c \
           attrdict.c \
           attrget.c \
           attrs.c \
           buffio.c \
           charsets.c \
           clean.c \
           config.c \
           entities.c \
           fileio.c \
           iconvtc.c \
           istack.c \
           lexer.c \
           localize.c \
           parser.c \
           pprint.c \
           streamio.c \
           tagask.c \
           tags.c \
           tidylib.c \
           tmbstr.c \
           utf8.c \
           win32tc.c
