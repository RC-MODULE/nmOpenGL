######################################################################
# Automatically generated by qmake (3.1) Thu Feb 24 15:45:05 2022
######################################################################

QT       += core gui
win32:CONFIG   += console static_runtime c++14 release
linux-g++:CONFIG   += console static_runtime c++14 debug
#CONFIG   += console static_runtime c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app

LIBS = -L$(MC12101)/lib -lmc12101load -lio_host
#LIBS = -L$(MC12101)/lib -L$(HAL)/lib -lmc12101load
win32:DEFINES += _ITERATOR_DEBUG_LEVEL=0
linux-g++:QMAKE_CXXFLAGS += -fpermissive
#QMAKE_CXXFLAGS += /MT

INCLUDEPATH  += $$PWD/include $(MC12101)/include $$PWD/../include
HEADERS += $$PWD/include/*.h $$PWD/../include/framebuffer.h
SOURCES += $$PWD/src/*.cpp $$PWD/src/*.c


# Directories

FORMS += \
    forms/mainwindow.ui