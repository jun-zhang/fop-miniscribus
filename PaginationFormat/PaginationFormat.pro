######################################################################
# Automatically generated by qmake (2.01a) mar 19. ago 17:28:27 2008
######################################################################

TEMPLATE = app
TARGET = xx
DEPENDPATH += . ui
INCLUDEPATH += .


MOC_DIR = build/.moc
RCC_DIR = build/.rcc
OBJECTS_DIR = build/.obj

CONFIG +=  qt release warn_on console
DESTDIR	+= ./

QT += xml
QT += network
QT += svg 
contains(QT_CONFIG, opengl):QT += opengl




# Input
HEADERS += BasicFoConfig.h \
           config.h \
           XML_Editor.h \
           SessionManager.h \
           FoColorName.h \
           PageFormatDlg.h \
           PageFormatGroup.h
FORMS += ui/pageformatdlg.ui
SOURCES += BasicFoConfig.cpp \
           FoColorName.cpp \
           main.cpp \
           XML_Editor.cpp \
           SessionManager.cpp \
           PageFormatDlg.cpp \
           PageFormatGroup.cpp
