CONFIG  += debug
QT      -= gui
QT      += network

TEMPLATE = app
CONFIG  += console

SOURCES   += test.cpp
unix:LIBS += -lqpropresenttext

TARGET   = test 

mac {
  CONFIG -= app_bundle
}