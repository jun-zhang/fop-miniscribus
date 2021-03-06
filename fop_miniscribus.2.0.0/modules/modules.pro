######################################################################
# Automatically generated by qmake (2.01a) ven 4. lug 14:27:15 2008
######################################################################

TEMPLATE = lib
TARGET = txtscribe

lessThan(QT_VER_MAJ, 4) | lessThan(QT_VER_MIN, 5) {
DEPENDPATH += . tidy zip
INCLUDEPATH += . tidy zip
} else {
DEPENDPATH += . tidy
INCLUDEPATH += . tidy
}



DESTDIR	+= ../lib
QT += xml
QT += network
QT += svg 

win32-msvc*{
TARGET = libtxtscribe
}

CONFIG   +=  qt release warn_off staticlib

!include( ../config.pri ) {
error( "../config.pri not found" )
}


           
lessThan(QT_VER_MAJ, 4) | lessThan(QT_VER_MIN, 5) {

# Input
HEADERS += tidy/access.h \
           tidy/attrdict.h \
           tidy/attrs.h \
           tidy/bestqtidy.h \
           tidy/buffio.h \
           tidy/charsets.h \
           tidy/clean.h \
           tidy/config.h \
           tidy/entities.h \
           tidy/fileio.h \
           tidy/forward.h \
           tidy/iconvtc.h \
           tidy/lexer.h \
           tidy/message.h \
           tidy/parser.h \
           tidy/platform.h \
           tidy/pprint.h \
           tidy/streamio.h \
           tidy/tags.h \
           tidy/tidy-int.h \
           tidy/tidy.h \
           tidy/tidyenum.h \
           tidy/tmbstr.h \
           tidy/utf8.h \
           tidy/version.h \
           zip/crc32.h \
           zip/crypt.h \
           zip/deflate.h \
           zip/gojobzip.h \
           zip/inffast.h \
           zip/inffixed.h \
           zip/inflate.h \
           zip/inftrees.h \
           zip/ioapi.h \
           zip/quazip.h \
           zip/quazipfile.h \
           zip/quazipfileinfo.h \
           zip/quazipnewinfo.h \
           zip/trees.h \
           zip/unzip.h \
           zip/zconf.h \
           zip/zconf.in.h \
           zip/zip.h \
           zip/ziphandle.h \
           zip/zlib.h \
           zip/zutil.h
           
SOURCES += tidy/access.c \
           tidy/alloc.c \
           tidy/attrask.c \
           tidy/attrdict.c \
           tidy/attrget.c \
           tidy/attrs.c \
           tidy/buffio.c \
           tidy/charsets.c \
           tidy/clean.c \
           tidy/config.c \
           tidy/entities.c \
           tidy/fileio.c \
           tidy/iconvtc.c \
           tidy/istack.c \
           tidy/lexer.c \
           tidy/localize.c \
           tidy/parser.c \
           tidy/pprint.c \
           tidy/streamio.c \
           tidy/tagask.c \
           tidy/tags.c \
           tidy/tidylib.c \
           tidy/tmbstr.c \
           tidy/utf8.c \
           zip/adler32.c \
           zip/compress.c \
           zip/crc32.c \
           zip/deflate.c \
           zip/example.c \
           zip/gzio.c \
           zip/infback.c \
           zip/inffast.c \
           zip/inflate.c \
           zip/inftrees.c \
           zip/ioapi.c \
           zip/minigzip.c \
           zip/quazip.cpp \
           zip/quazipfile.cpp \
           zip/quazipnewinfo.cpp \
           zip/trees.c \
           zip/uncompr.c \
           zip/unzip.c \
           zip/zip.c \
           zip/zutil.c
               
           
} else {

SOURCES += tidy/access.c \
           tidy/alloc.c \
           tidy/attrask.c \
           tidy/attrdict.c \
           tidy/attrget.c \
           tidy/attrs.c \
           tidy/buffio.c \
           tidy/charsets.c \
           tidy/clean.c \
           tidy/config.c \
           tidy/entities.c \
           tidy/fileio.c \
           tidy/iconvtc.c \
           tidy/istack.c \
           tidy/lexer.c \
           tidy/localize.c \
           tidy/parser.c \
           tidy/pprint.c \
           tidy/streamio.c \
           tidy/tagask.c \
           tidy/tags.c \
           tidy/tidylib.c \
           tidy/tmbstr.c \
           tidy/utf8.c
           
           
HEADERS += tidy/access.h \
           tidy/attrdict.h \
           tidy/attrs.h \
           tidy/bestqtidy.h \
           tidy/buffio.h \
           tidy/charsets.h \
           tidy/clean.h \
           tidy/config.h \
           tidy/entities.h \
           tidy/fileio.h \
           tidy/forward.h \
           tidy/iconvtc.h \
           tidy/lexer.h \
           tidy/message.h \
           tidy/parser.h \
           tidy/platform.h \
           tidy/pprint.h \
           tidy/streamio.h \
           tidy/tags.h \
           tidy/tidy-int.h \
           tidy/tidy.h \
           tidy/tidyenum.h \
           tidy/tmbstr.h \
           tidy/utf8.h \
           tidy/version.h
           
}
           
           
           
           
