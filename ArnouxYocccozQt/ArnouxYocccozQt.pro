#-------------------------------------------------
#
# Project created by QtCreator 2013-04-14T10:22:37
#
#-------------------------------------------------

#QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ArnouxYocccozQt
TEMPLATE = app
#CFLAGS += -Weverything
#QMAKE_CFLAGS += -Weverything
#CONFIG = warn_on
QMAKE_CXXFLAGS += -Wextra

CONFIG += console
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
    intex/TwistedIntervalExchangeMap.cpp \
    intex/Mod1NumberIntExchange.cpp \
    intex/Mod1Number.cpp \
    intex/LengthsAndTwist.cpp \
    intex/IntervalPairing.cpp \
    math/WeighedTree.cpp \
    math/PerronFrobenius.cpp \
    math/Permutation.cpp \
    math/Matrix.cpp \
    math/Choose.cpp \
    fol/FoliationSphere.cpp \
    fol/FoliationRP2.cpp \
    fol/FoliationFromSphere.cpp \
    fol/FoliationFromRP2.cpp \
    fol/FoliationDisk.cpp \
    fol/Foliation.cpp \
    windows/DocumentationWindow.cpp \
    windows/CreateFoliationWizard.cpp \
    drawing/FoliationDrawingArea.cpp \
    windows/foliationWidgets/SepSegmentSearchWidget.cpp \
    windows/FoliationMainWindow.cpp \
    windows/WindowManager.cpp \
    windows/FileMenu.cpp \
    windows/HelpMenu.cpp \
    windows/FoliationManager.cpp \
    io.cpp \
    windows/foliationWidgets/FoliationListWidget.cpp \
    windows/foliationWidgets/TransverseCurveSearchWidget.cpp \
    windows/foliationWidgets/FoliationDataTreeWidget.cpp \
    windows/foliationWidgets/TransverseCurveTreeWidgetItem.cpp

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
    intex/TwistedIntervalExchangeMap.h \
    intex/Mod1NumberIntExchange.h \
    intex/Mod1Number.h \
    intex/LengthsAndTwist.h \
    intex/IntervalPairing.h \
    math/WeighedTree.h \
    math/PerronFrobenius.h \
    math/Permutation.h \
    math/Matrix.hpp \
    math/Matrix.h \
    math/Choose.h \
    fol/FoliationSphere.h \
    fol/FoliationRP2.h \
    fol/FoliationFromSphere.h \
    fol/FoliationFromRP2.h \
    fol/FoliationDisk.h \
    fol/Foliation.h \
    windows/DocumentationWindow.h \
    windows/CreateFoliationWizard.h \
    drawing/FoliationDrawingArea.h \
    qt.h \
    windows/foliationWidgets/SepSegmentSearchWidget.h \
    windows/FoliationMainWindow.h \
    windows/WindowManager.h \
    windows/FileMenu.h \
    windows/HelpMenu.h \
    windows/FoliationManager.h \
    intex/ContainingInterval.h \
    fol/VDirection.h \
    intex/HDirection.h \
    io.h \
    windows/foliationWidgets/FoliationListWidget.h \
    windows/foliationWidgets/TransverseCurveSearchWidget.h \
    windows/foliationWidgets/FoliationDataTreeWidget.h \
    windows/foliationWidgets/TransverseCurveTreeWidgetItem.h

FORMS    += \
    mainwindow.ui

OTHER_FILES += FoliationRP2old.cpp \
    FoliationRP2old.h

