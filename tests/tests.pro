TEMPLATE = app
PROJECT = bignumber-tests
TARGET = $$PROJECT

QT += core \
      testlib
QT -= gui

CONFIG += warn_on
QMAKE_CXXFLAGS += -Wall -Werror -Wextra -pedantic-errors
QMAKE_CXXFLAGS += -std=c++11

DESTDIR = $$PWD/build/sbin
OBJECTS_DIR = $$PWD/build/.o
MOC_DIR = $$PWD/build/.moc

SOURCES = \
    src/main.cpp

isEmpty($$PREFIX) {
    PREFIX = $$PWD/../
}

INCLUDEPATH += \
    $$PREFIX/include

LIBS += \
    -L$$PREFIX/lib -lbignumber

#installs
target.path = $$PREFIX/sbin

INSTALLS += \
    target
