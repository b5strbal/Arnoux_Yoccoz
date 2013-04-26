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
    ../Arnoux_Yoccoz/PerronFrobenius.cpp \
    ../Arnoux_Yoccoz/Matrix.cpp \
    ../Arnoux_Yoccoz/global.cpp \
    ../Arnoux_Yoccoz/FoliationRP2.cpp \
    ../Arnoux_Yoccoz/Foliation.cpp \
    ../Arnoux_Yoccoz/SeparatrixSegment.cpp \
    ../Arnoux_Yoccoz/Choose.cpp \
    ../Arnoux_Yoccoz/DisjointIntervals.cpp \
    ../Arnoux_Yoccoz/SepSegmentCollection.cpp \
    ../Arnoux_Yoccoz/TransverseCurve.cpp \
    ../Arnoux_Yoccoz/FoliationDisk.cpp \
    ../Arnoux_Yoccoz/FoliationSphere.cpp \
    ../Arnoux_Yoccoz/FoliationFromSphere.cpp \
    ../Arnoux_Yoccoz/FoliationFromRP2.cpp \
    ../Arnoux_Yoccoz/Permutation.cpp \
    ../Arnoux_Yoccoz/Modint.cpp \
    ../Arnoux_Yoccoz/SepSegmentDatabase.cpp \
    ../Arnoux_Yoccoz/TransverseCurveDatabase.cpp \
    ../Arnoux_Yoccoz/IntervalNeighborhoods.cpp \
    ../Arnoux_Yoccoz/Mod1Number.cpp \
    ../Arnoux_Yoccoz/Mod1NumberIntExchange.cpp \
    ../Arnoux_Yoccoz/TwistedIntervalExchangeMap.cpp \
    ../Arnoux_Yoccoz/IntervalExchangeFoliationDisk.cpp \
    ../Arnoux_Yoccoz/IntervalExchangeBase.cpp \
    ../Arnoux_Yoccoz/SepSegmentCollectionsFromRP2.cpp \
    ../Arnoux_Yoccoz/TransverseCurveDatabaseFromRP2.cpp \
    ../Arnoux_Yoccoz/SepSegmentCollections.cpp

HEADERS  += mainwindow.h \
    ../Arnoux_Yoccoz/WeighedTree.h \
    ../Arnoux_Yoccoz/PerronFrobenius.h \
    ../Arnoux_Yoccoz/Matrix.hpp \
    ../Arnoux_Yoccoz/Matrix.h \
    ../Arnoux_Yoccoz/global.h \
    ../Arnoux_Yoccoz/FoliationRP2.h \
    ../Arnoux_Yoccoz/Foliation.h \
    ../Arnoux_Yoccoz/SeparatrixSegment.h \
    ../Arnoux_Yoccoz/Choose.h \
    ../Arnoux_Yoccoz/SepSegmentCollection.h \
    ../Arnoux_Yoccoz/TransverseCurve.h \
    ../Arnoux_Yoccoz/FoliationDisk.h \
    ../Arnoux_Yoccoz/FoliationSphere.h \
    ../Arnoux_Yoccoz/FoliationFromSphere.h \
    ../Arnoux_Yoccoz/FoliationFromRP2.h \
    ../Arnoux_Yoccoz/Permutation.h \
    ../Arnoux_Yoccoz/DisjointIntervals.h \
    ../Arnoux_Yoccoz/Modint.h \
    ../Arnoux_Yoccoz/SepSegmentDatabase.h \
    ../Arnoux_Yoccoz/TransverseCurveDatabase.h \
    ../Arnoux_Yoccoz/IntervalNeighborhoods.h \
    ../Arnoux_Yoccoz/Mod1Number.h \
    ../Arnoux_Yoccoz/Mod1NumberIntExchange.h \
    ../Arnoux_Yoccoz/TwistedIntervalExchangeMap.h \
    ../Arnoux_Yoccoz/IntervalExchangeFoliationDisk.h \
    ../Arnoux_Yoccoz/IntervalExchangeBase.h \
    ../Arnoux_Yoccoz/SepSegmentCollectionsFromRP2.h \
    ../Arnoux_Yoccoz/TransverseCurveDatabaseFromRP2.h \
    ../Arnoux_Yoccoz/SepSegmentCollections.h

FORMS    += mainwindow.ui

OTHER_FILES += ../Arnoux_Yoccoz/FoliationRP2old.cpp \
    ../Arnoux_Yoccoz/FoliationRP2old.h \
