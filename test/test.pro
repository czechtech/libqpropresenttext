CONFIG  += debug
CONFIG  -= app_bundle
QT      -= gui
QT      += network

TEMPLATE = app
CONFIG  += console

SOURCES   += test.cpp
   LIBS += ../libqspropresenttext.so.1.0.0

TARGET   = test 

QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/..\',-z,origin'
