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
    ../Arnoux_Yoccoz/fol/FoliationSphere.cpp \
    ../Arnoux_Yoccoz/fol/FoliationRP2.cpp \
    ../Arnoux_Yoccoz/fol/FoliationFromSphere.cpp \
    ../Arnoux_Yoccoz/fol/FoliationFromRP2.cpp \
    ../Arnoux_Yoccoz/fol/FoliationDisk.cpp \
    ../Arnoux_Yoccoz/fol/Foliation.cpp \
    ../Arnoux_Yoccoz/intex/TwistedIntervalExchangeMap.cpp \
    ../Arnoux_Yoccoz/intex/Mod1NumberIntExchange.cpp \
    ../Arnoux_Yoccoz/intex/Mod1Number.cpp \
    ../Arnoux_Yoccoz/math/WeighedTree.cpp \
    ../Arnoux_Yoccoz/math/PerronFrobenius.cpp \
    ../Arnoux_Yoccoz/math/Permutation.cpp \
    ../Arnoux_Yoccoz/math/Matrix.cpp \
    ../Arnoux_Yoccoz/math/Choose.cpp \
    ../Arnoux_Yoccoz/scc/TransverseCurveDatabaseFromRP2.cpp \
    ../Arnoux_Yoccoz/scc/TransverseCurveDatabase.cpp \
    ../Arnoux_Yoccoz/scc/TransverseCurve.cpp \
    ../Arnoux_Yoccoz/scc/SepSegmentDatabase.cpp \
    ../Arnoux_Yoccoz/scc/SepSegmentCollections.cpp \
    ../Arnoux_Yoccoz/scc/SeparatrixSegment.cpp \
    ../Arnoux_Yoccoz/scc/IntervalNeighborhoods.cpp \
    ../Arnoux_Yoccoz/scc/DisjointIntervals.cpp \
    ../Arnoux_Yoccoz/intex/LengthsAndTwist.cpp \
    ../Arnoux_Yoccoz/global.cpp \
    ../Arnoux_Yoccoz/intex/IntervalPairing.cpp \
    ../Arnoux_Yoccoz/scc/Mod1NumberIntExWithInfo.cpp

HEADERS  += mainwindow.h \
    ../Arnoux_Yoccoz/fol/FoliationSphere.h \
    ../Arnoux_Yoccoz/fol/FoliationRP2.h \
    ../Arnoux_Yoccoz/fol/FoliationFromSphere.h \
    ../Arnoux_Yoccoz/fol/FoliationFromRP2.h \
    ../Arnoux_Yoccoz/fol/FoliationDisk.h \
    ../Arnoux_Yoccoz/fol/Foliation.h \
    ../Arnoux_Yoccoz/intex/TwistedIntervalExchangeMap.h \
    ../Arnoux_Yoccoz/intex/Mod1NumberIntExchange.h \
    ../Arnoux_Yoccoz/intex/Mod1Number.h \
    ../Arnoux_Yoccoz/math/WeighedTree.h \
    ../Arnoux_Yoccoz/math/PerronFrobenius.h \
    ../Arnoux_Yoccoz/math/Permutation.h \
    ../Arnoux_Yoccoz/math/Matrix.hpp \
    ../Arnoux_Yoccoz/math/Matrix.h \
    ../Arnoux_Yoccoz/math/Choose.h \
    ../Arnoux_Yoccoz/scc/TransverseCurveDatabaseFromRP2.h \
    ../Arnoux_Yoccoz/scc/TransverseCurveDatabase.h \
    ../Arnoux_Yoccoz/scc/TransverseCurve.h \
    ../Arnoux_Yoccoz/scc/SepSegmentDatabase.h \
    ../Arnoux_Yoccoz/scc/SepSegmentCollections.h \
    ../Arnoux_Yoccoz/scc/SeparatrixSegment.h \
    ../Arnoux_Yoccoz/scc/IntervalNeighborhoods.h \
    ../Arnoux_Yoccoz/scc/DisjointIntervals.h \
    ../Arnoux_Yoccoz/intex/LengthsAndTwist.h \
    ../Arnoux_Yoccoz/global.h \
    ../Arnoux_Yoccoz/intex/IntervalPairing.h \
    ../Arnoux_Yoccoz/scc/Mod1NumberIntExWithInfo.h

FORMS    += mainwindow.ui

OTHER_FILES += ../Arnoux_Yoccoz/FoliationRP2old.cpp \
    ../Arnoux_Yoccoz/FoliationRP2old.h
