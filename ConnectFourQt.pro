QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConnectFourQt
TEMPLATE = app

SOURCES +=     main.cpp     mainwindow.cpp \
    chessrobot.cpp \
    gameboard.cpp \
    startwindow.cpp

HEADERS +=     mainwindow.h \
    chessrobot.h \
    gameboard.h \
    startwindow.h

FORMS +=     mainwindow.ui \
    startwindow.ui
