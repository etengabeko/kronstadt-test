TEMPLATE = lib
PROJECT = bignumber
TARGET = $$PROJECT

QT += core
QT -= gui

CONFIG += warn_on
QMAKE_CXXFLAGS += -Werror -Wextra -pedantic-errors
QMAKE_CXXFLAGS += -std=c++11

DESTDIR = $$PWD/build/lib
OBJECTS_DIR = $$PWD/build/.o

SOURCES = \
    src/bignumber.cpp

PUB_HEADERS = \
    src/bignumber.h

HEADERS = \
    $$PUB_HEADERS

isEmpty($$PREFIX) {
    PREFIX = $$PWD
}

# installs
target.path = $$PREFIX/lib

# includes
includes.path = $$PREFIX/include
includes.files += $$PUB_HEADERS

INSTALLS += \
    target \
    includes
