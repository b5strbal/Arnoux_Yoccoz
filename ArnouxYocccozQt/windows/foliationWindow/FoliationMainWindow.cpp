#include "FoliationMainWindow.h"
#include "windows/FileMenu.h"
#include "windows/HelpMenu.h"
#include "fol/Foliation.h"
#include "windows/FoliationManager.h"
#include "windows/drawing/FoliationDrawing.h"
#include "FoliationDataTreeWidget.h"
#include "SepSegmentSearchWidget.h"
#include "FoliationListWidget.h"
#include "TransverseCurveSearchWidget.h"
//#include "WindowManager.h"

#include <QAction>
#include <QMenuBar>
#include <QLabel>
#include <QListWidget>
#include <QDockWidget>
#include <QStackedWidget>
#include <cassert>


FoliationMainWindow::FoliationMainWindow(const WindowManager &manager, QWidget *parent) :
    QMainWindow(parent)
{
    drawingAreaStackWidget = new QStackedWidget;
    setCentralWidget(drawingAreaStackWidget);

    foliationListWidget = new FoliationListWidget;

    foliationListDockWidget = new QDockWidget(tr("Open foliations"), this);
    foliationListDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    foliationListDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    foliationListDockWidget->setWidget(foliationListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, foliationListDockWidget);

    foliationDataTableStackedWidget = new QStackedWidget;
    foliationDataTableDockWidget = new QDockWidget(tr("Properties"));
    foliationDataTableDockWidget->setWidget(foliationDataTableStackedWidget);
    addDockWidget(Qt::RightDockWidgetArea, foliationDataTableDockWidget);

    sepSegmentSearchStackedWidget = new QStackedWidget;
    sepSegmentSearchDockWidget = new QDockWidget(tr("Find good separatrix segments"));
    sepSegmentSearchDockWidget->setWidget(sepSegmentSearchStackedWidget);
    addDockWidget(Qt::BottomDockWidgetArea, sepSegmentSearchDockWidget);
    sepSegmentSearchDockWidget->hide();

    transverseCurveSearchStackedWidget = new QStackedWidget;
    transverseCurveSearchDockWidget = new QDockWidget(tr("Find transverse curves"));
    transverseCurveSearchDockWidget->setWidget(transverseCurveSearchStackedWidget);
    addDockWidget(Qt::BottomDockWidgetArea, transverseCurveSearchDockWidget);
    transverseCurveSearchDockWidget->hide();

    createActions();
    createMenus(manager);
    updateMenus(-1);

    connect(foliationListWidget, SIGNAL(currentRowChanged(int)),
            foliationDataTableStackedWidget, SLOT(setCurrentIndex(int)));
    connect(foliationListWidget, SIGNAL(currentRowChanged(int)),
            sepSegmentSearchStackedWidget, SLOT(setCurrentIndex(int)));
    connect(foliationListWidget, SIGNAL(currentRowChanged(int)),
            transverseCurveSearchStackedWidget, SLOT(setCurrentIndex(int)));
    connect(foliationListWidget, SIGNAL(currentRowChanged(int)),
            drawingAreaStackWidget, SLOT(setCurrentIndex(int)));
    connect(foliationListWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(updateMenus(int)));

    for(int i = 3; i < 10; i++){
        createNewFoliation(new balazs::Foliation(i, balazs::arnoux_yoccoz_tag()));
    }
    createNewFoliation(new balazs::Foliation({0.866258, 0.494918, 0.0543639, 0.00142665, 0.0372015, 0.000463471},
                                             balazs::Permutation({1, 0, 5, 3, 2, 4}), 0.0176439));
}



void FoliationMainWindow::createNewFoliation(balazs::Foliation *pFoliation)
{
    static int counter = 1;

    foliations << new FoliationManager(std::unique_ptr<balazs::Foliation>(pFoliation));

    foliationListWidget->addItem(tr("Foliation %1").arg(QString::number(counter++)));
    drawingAreaStackWidget->addWidget(foliations.back()->drawingArea());
    foliationDataTableStackedWidget->addWidget(foliations.back()->foliationDataTableWidget());
    sepSegmentSearchStackedWidget->addWidget(foliations.back()->sepSegmentSearchWidget());
    transverseCurveSearchStackedWidget->addWidget(foliations.back()->transverseCurveSearchWidget());

    foliationListWidget->setCurrentRow(foliationListWidget->count() - 1);
}



void FoliationMainWindow::updateMenus(int newActiveIndex)
{
    menuBar()->clear();
    if(activeIndex() == -1){
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(helpMenu);
    } else {
        menuBar()->addMenu(fileMenu);
        viewMenu = foliations[newActiveIndex]->viewMenu();
        menuBar()->addMenu(viewMenu);
        menuBar()->addMenu(searchMenu);
        menuBar()->addMenu(toolsMenu);
        menuBar()->addMenu(helpMenu);
    }
}






const balazs::Foliation &FoliationMainWindow::activeFoliation() const
{
    assert(activeIndex() >= 0);
    return activeFolationManager().foliation();
}

FoliationManager &FoliationMainWindow::activeFolationManager() const
{
    assert(activeIndex() >= 0);
    return *foliations[activeIndex()];
}

int FoliationMainWindow::activeIndex() const
{
    return foliationListWidget->currentRow();
}



void FoliationMainWindow::createActions()
{
    findSepSegmentsAct = new QAction(tr("Find good separatrix segments..."), this);
    connect(findSepSegmentsAct, SIGNAL(triggered()), this, SLOT(openSepSegmentSearch()), Qt::UniqueConnection);

    findTransverseCurvesAct = new QAction(tr("Find transverse curves..."), this);
    connect(findTransverseCurvesAct, SIGNAL(triggered()), this, SLOT(openTransverseCurveSearch()));

    rotateLeftAct = new QAction(tr("Rotate to Left"), this);
    rotateRightAct = new QAction(tr("Rotate to Right"), this);
    reverseAct = new QAction(tr("Reverse"), this);
    flipOverAct = new QAction(tr("Flip Over"), this);

    connect(rotateLeftAct, SIGNAL(triggered()), this, SLOT(rotateFoliationLeft()), Qt::UniqueConnection);
    connect(rotateRightAct, SIGNAL(triggered()), this, SLOT(rotateFoliationRight()), Qt::UniqueConnection);
    connect(reverseAct, SIGNAL(triggered()), this, SLOT(reverseFoliation()), Qt::UniqueConnection);
    connect(flipOverAct, SIGNAL(triggered()), this, SLOT(flipOverFoliation()), Qt::UniqueConnection);
}







void FoliationMainWindow::rotateFoliationLeft()
{
    createNewFoliation(new balazs::Foliation(activeFoliation(), -1, balazs::rotate_tag()));
}

void FoliationMainWindow::rotateFoliationRight()
{
    createNewFoliation(new balazs::Foliation(activeFoliation(), 1, balazs::rotate_tag()));
}

void FoliationMainWindow::reverseFoliation()
{
    createNewFoliation(new balazs::Foliation(activeFoliation(), balazs::reverse_tag()));
}

void FoliationMainWindow::flipOverFoliation()
{
    createNewFoliation(new balazs::Foliation(activeFoliation(), balazs::flip_over_tag()));
}



void FoliationMainWindow::openSepSegmentSearch()
{
    sepSegmentSearchDockWidget->show();
}

void FoliationMainWindow::openTransverseCurveSearch()
{
    transverseCurveSearchDockWidget->show();
}





void FoliationMainWindow::createMenus(const WindowManager& manager)
{
    fileMenu = new FileMenu(manager);

    searchMenu = new QMenu(tr("Search"));
    searchMenu->addAction(findSepSegmentsAct);
    searchMenu->addAction(findTransverseCurvesAct);

    toolsMenu = new QMenu(tr("Tools"));
    reparametrizeMenu = toolsMenu->addMenu(tr("Reparametrize Foliation"));
    reparametrizeMenu->addAction(rotateLeftAct);
    reparametrizeMenu->addAction(rotateRightAct);
    reparametrizeMenu->addAction(reverseAct);
    reparametrizeMenu->addAction(flipOverAct);
    
    helpMenu = new HelpMenu(manager);
}



