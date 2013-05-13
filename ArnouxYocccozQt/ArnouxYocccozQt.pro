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
    scc/SeparatrixSegment.cpp \
    scc/Mod1NumberIntExWithInfo.cpp \
    scc/IntervalNeighborhoods.cpp \
    scc/DisjointIntervals.cpp \
    scc/TransverseCurve.cpp \
    scc/TransverseCurveDatabase.cpp \
    scc/SepSegmentDatabase.cpp \
    scc/SepSegmentCollection.cpp \
    scc/SSCMode.cpp \
    scc/SepSegmentCollectionList.cpp \
    global.cpp \
    intex/TwistedIntervalExchangeMap.cpp \
    intex/Mod1NumberIntExchange.cpp \
    intex/Mod1Number.cpp \
    intex/LengthsAndTwist.cpp \
    intex/IntervalPairing.cpp \
    math/WeighedTree.cpp \
    math/PerronFrobenius.cpp \
    math/Permutation.cpp \
    math/Modint.cpp \
    math/Matrix.cpp \
    math/Choose.cpp \
    fol/FoliationSphere.cpp \
    fol/FoliationRP2.cpp \
    fol/FoliationFromSphere.cpp \
    fol/FoliationFromRP2.cpp \
    fol/FoliationDisk.cpp \
    fol/Foliation.cpp \
    windows/MainWindow.cpp \
    windows/FoliationWindow.cpp \
    windows/FoliationSphereWindow.cpp \
    windows/FoliationRP2Window.cpp \
    windows/DocumentationWindow.cpp \
    windows/CreateFoliationWizard.cpp \
    drawing/drawings.cpp \
    drawing/FoliationDrawingArea.cpp

HEADERS  += scc/IntervalNeighborhoods.h \
    scc/DisjointIntervals.h \
    scc/Mod1NumberIntExWithInfo.h \
    scc/SeparatrixSegment.h \
    scc/TransverseCurve.h \
    scc/TransverseCurveDatabase.h \
    scc/SepSegmentDatabase.h \
    scc/SSCMode.h \
    scc/SepSegmentCollectionList.h \
    scc/SepSegmentCollection.h \
    global.h \
    intex/TwistedIntervalExchangeMap.h \
    intex/Mod1NumberIntExchange.h \
    intex/Mod1Number.h \
    intex/LengthsAndTwist.h \
    intex/IntervalPairing.h \
    math/WeighedTree.h \
    math/PerronFrobenius.h \
    math/Permutation.h \
    math/Modint.h \
    math/Matrix.hpp \
    math/Matrix.h \
    math/Choose.h \
    fol/FoliationSphere.h \
    fol/FoliationRP2.h \
    fol/FoliationFromSphere.h \
    fol/FoliationFromRP2.h \
    fol/FoliationDisk.h \
    fol/Foliation.h \
    windows/MainWindow.h.autosave \
    windows/MainWindow.h \
    windows/FoliationWindow.h \
    windows/FoliationSphereWindow.h \
    windows/FoliationRP2Window.h \
    windows/DocumentationWindow.h \
    windows/CreateFoliationWizard.h \
    drawing/drawings.h \
    drawing/FoliationDrawingArea.h \
    qt.h

FORMS    += \
    mainwindow.ui

OTHER_FILES += FoliationRP2old.cpp \
    FoliationRP2old.h \
    io.h

