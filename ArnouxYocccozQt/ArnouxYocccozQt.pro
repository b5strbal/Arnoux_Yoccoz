#-------------------------------------------------
#
# Project created by QtCreator 2013-04-14T10:22:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ArnouxYocccozQt
TEMPLATE = app

CONFIG += C++11
SOURCES += main.cpp\
        mainwindow.cpp \
    ../Arnoux_Yoccoz/WeighedTree.cpp \
    ../Arnoux_Yoccoz/UnitIntervalPoint.cpp \
    ../Arnoux_Yoccoz/PerronFrobenius.cpp \
    ../Arnoux_Yoccoz/Matrix.cpp \
    ../Arnoux_Yoccoz/IntervalExchangeMap.cpp \
    ../Arnoux_Yoccoz/global.cpp \
    ../Arnoux_Yoccoz/FoliationRP2.cpp \
    ../Arnoux_Yoccoz/Foliation.cpp \
    ../Arnoux_Yoccoz/ArcsAroundDivPoints.cpp

HEADERS  += mainwindow.h \
    ../Arnoux_Yoccoz/WeighedTree.h \
    ../Arnoux_Yoccoz/UnitIntervalPoint.h \
    ../Arnoux_Yoccoz/PerronFrobenius.h \
    ../Arnoux_Yoccoz/Matrix.hpp \
    ../Arnoux_Yoccoz/Matrix.h \
    ../Arnoux_Yoccoz/io.h \
    ../Arnoux_Yoccoz/IntervalExchangeMap.h \
    ../Arnoux_Yoccoz/global.h \
    ../Arnoux_Yoccoz/FoliationRP2.h \
    ../Arnoux_Yoccoz/Foliation.h \
    ../Arnoux_Yoccoz/ArcsAroundDivPoints.h

FORMS    += mainwindow.ui

OTHER_FILES += ../Arnoux_Yoccoz/FoliationRP2old.cpp \
    ../Arnoux_Yoccoz/FoliationRP2old.h \
