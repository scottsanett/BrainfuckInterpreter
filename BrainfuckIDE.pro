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
LIBS += -L/usr/local/Cellar/boost/1.64.0_1/lib -lboost_system -lboost_serialization

SOURCES += \
        main.cpp \
        ./gui/brainfuckide.cpp \
    ./intermediary/intermediary.cpp \
    ./server/server_delegate.cpp \
    ./client/client_delegate.cpp \
    ./interpreter/interpreter.cpp \
    ./client/client.cpp \
    ./server/server.cpp \
    ./gui/loginwindow.cpp \
    ./gui/createaccount.cpp \
    ./gui/menuaction.cpp \
    ./database/database.cpp \
    ./gui/filechooser.cpp

HEADERS += \
    ./server/server_delegate.hpp \
    ./server/server.hpp \
    namespace.hpp \
    ./interpreter/interpreter.hpp \
    ./intermediary/intermediary.hpp \
    ./client/client_delegate.hpp \
    ./client/client.hpp \
    ./gui/brainfuckide.hpp \
    ./gui/loginwindow.hpp \
    ./gui/createaccount.hpp \
    ./gui/menuaction.hpp \
    ./database/database.hpp \
    ./gui/filechooser.hpp

FORMS += \
        gui/brainfuckide.ui \
    gui/loginwindow.ui \
    gui/createaccount.ui \
    gui/filechooser.ui
