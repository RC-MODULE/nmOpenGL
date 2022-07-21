######################################################################
# Automatically generated by qmake (3.1) Thu Feb 24 15:45:05 2022
######################################################################

QT       += core gui remoteobjects
win32:CONFIG   += console static_runtime c++14 release
linux-g++:CONFIG   += console static_runtime c++14 debug
#CONFIG   += console static_runtime c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app

LIBS = -L$(MC12101)/lib -L$(MC12101_RPC)/lib -lmc12101load -lmc12101load_rpc -lio_host
win32:DEFINES += _ITERATOR_DEBUG_LEVEL=0
win32:QMAKE_CXXFLAGS += /MT
linux-g++:QMAKE_CXXFLAGS += -fpermissive

INCLUDEPATH  += $$PWD/include $(MC12101)/include $$PWD/../include $(HAL)/include $(MC12101_RPC)/include
HEADERS += $$PWD/include/*.h $$PWD/../include/framebuffer.h $(HAL)/include/*.h \
    include/iobserver.h
SOURCES += $$PWD/src/*.cpp


# Directories

FORMS += \
    forms/mainwindow.ui
