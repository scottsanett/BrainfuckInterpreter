#-------------------------------------------------
#
# Project created by QtCreator 2017-06-28T18:05:19
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++14
QMAKE_CXXFLAGS += -std=c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BrainfuckIDE
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/Cellar/boost/1.64.0_1/include
LIBS += -L/usr/local/Cellar/boost/1.64.0_1/lib -lboost_system

SOURCES += \
        main.cpp \
        brainfuckide.cpp \
    intermediary.cpp \
    server_delegate.cpp \
    client_delegate.cpp \
    interpreter.cpp \
    client.cpp \
    server.cpp \
    loginwindow.cpp \
    createaccount.cpp

HEADERS += \
    server_delegate.hpp \
    server.hpp \
    namespace.hpp \
    interpreter.hpp \
    intermediary.hpp \
    client_delegate.hpp \
    client.hpp \
    brainfuckide.hpp \
    loginwindow.hpp \
    createaccount.hpp

FORMS += \
        brainfuckide.ui \
    loginwindow.ui \
    createaccount.ui
