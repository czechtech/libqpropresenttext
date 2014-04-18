QT -= gui
QT += network testlib

TARGET = qpropresenttext
TEMPLATE = lib

DEFINES += LIBQPROPRESENTTEXT_LIBRARY

SOURCES += qpropresenttext.cpp

HEADERS += qpropresenttext.h

unix {
    target.path = /usr/lib
    INSTALLS += target

    header_files.files = $$HEADERS
    header_files.path = /usr/include
    INSTALLS += header_files
}
