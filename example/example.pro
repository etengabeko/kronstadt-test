TEMPLATE = app
PROJECT = bignumber-example
TARGET = $$PROJECT

QT += core
QT -= gui

CONFIG += warn_on
QMAKE_CXXFLAGS += -Werror -Wextra -pedantic-errors
QMAKE_CXXFLAGS += -std=c++11

DESTDIR = $$PWD/build/bin
OBJECTS_DIR = $$PWD/build/.o

SOURCES = \
    src/main.cpp

INCLUDEPATH += \
    $$PREFIX/include

LIBS += \
    -L$$PREFIX/lib -lbignumber

# installs
target.path = $$PREFIX/bin

INSTALLS += \
    target
