#include "FoliationMainWindow.h"
#include "FileMenu.h"
#include "HelpMenu.h"
#include "../fol/Foliation.h"
#include "FoliationManager.h"
#include "../drawing/FoliationDrawingArea.h"
#include "foliationWidgets/FoliationDataTableWidget.h"
#include "foliationWidgets/SepSegmentSearchWidget.h"
//#include "WindowManager.h"

#include <QAction>
#include <QMenuBar>
#include <QLabel>
#include <QListWidget>
#include <QDockWidget>
#include <QToolBar>
//#include <QSpinBox>


FoliationMainWindow::FoliationMainWindow(const WindowManager &manager, QWidget *parent) :
    QMainWindow(parent),
    activeFoliationIndex(-1)
{

    foliationListDockWidget = new QDockWidget(tr("Open foliations"), this);

    foliationListWidget = new QListWidget(foliationListDockWidget);
    foliationListWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    foliationListDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    foliationListDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    foliationListDockWidget->setWidget(foliationListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, foliationListDockWidget);

    connect(foliationListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(setActiveFoliation(int)), Qt::UniqueConnection);


//    QLabel* permFontSizeLabel = new QLabel(tr("Font size:"));
//    QSpinBox* permFontSizeSpinBox = new QSpinBox;
//    permFontSizeSpinBox->setValue(14);

//    toolBar = addToolBar(tr("Font size"));
//    toolBar->addWidget(permFontSizeLabel);
//    toolBar->addWidget(permFontSizeSpinBox);

    createActions();
    createMenus(manager);
    setActiveFoliation(-1);
   // updateConnections();


    for(int i = 3; i < 10; i++){
        createNewFoliation(new balazs::Foliation(i));
    }

}



void FoliationMainWindow::createNewFoliation(balazs::Foliation *pFoliation)
{
    static int counter = 1;

   // foliations.push_back(std::unique_ptr<FoliationManager>(new FoliationManager(std::unique_ptr<balazs::Foliation>(pFoliation))));
  //  foliations <<std::unique_ptr<FoliationManager>(new FoliationManager(std::unique_ptr<balazs::Foliation>(pFoliation)));

    foliations << new FoliationManager(std::unique_ptr<balazs::Foliation>(pFoliation));

    foliationListWidget->addItem(tr("Foliation %1").arg(QString::number(counter++)));
    foliationListWidget->setCurrentRow(foliationListWidget->count() - 1);
}



void FoliationMainWindow::setActiveFoliation(int index)
{
    // removing old dockwidgets and central widget
    if(activeFoliationIndex == -1){
        // nothing to remove
    } else {
        removeDockWidgets();
        centralWidget()->setParent(nullptr);
    }


    activeFoliationIndex = index;

    // setting central widget
    if(activeFoliationIndex == -1){
        // nothing to set
    } else {
        setCentralWidget(activeFolationManager().drawingArea());
    }

    // updating menubar
    menuBar()->clear();
    if(activeFoliationIndex == -1){
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(helpMenu);
    } else {
        menuBar()->addMenu(fileMenu);
        viewMenu = activeFolationManager().viewMenu();
        menuBar()->addMenu(viewMenu);
        menuBar()->addMenu(searchMenu);
        menuBar()->addMenu(toolsMenu);
        menuBar()->addMenu(helpMenu);

    }

    // adding new dockwidgets
    if(activeFoliationIndex > -1){
        addDockWidgets();
    }
  //  updateConnections();
}


void FoliationMainWindow::addDockWidgets()
{
    foliationDataTableDockWidget = new QDockWidget(tr("Properties"));
    foliationDataTableDockWidget->setWidget(activeFolationManager().foliationDataTableWidget());
    sepSegmentSearchDockWidget = new QDockWidget(tr("Find good separatrix segments"));
    sepSegmentSearchDockWidget->setWidget(activeFolationManager().sepSegmentSearchWidget());
    addDockWidget(Qt::RightDockWidgetArea, foliationDataTableDockWidget);
  //  if(activeFolationManager().foliationDataTableShown()){
  //      addDockWidget(Qt::RightDockWidgetArea, foliationDataTableDockWidget);
  //  }
  //  if(activeFolationManager().sepSegmentSearchShown()){
  //      addDockWidget(Qt::BottomDockWidgetArea, sepSegmentSearchDockWidget);
   // }
}


void FoliationMainWindow::removeDockWidgets()
{
 //   if(activeFolationManager().foliationDataTableShown()){
        removeDockWidget(foliationDataTableDockWidget);
  //  }
  //  if(activeFolationManager().sepSegmentSearchShown()){
        removeDockWidget(sepSegmentSearchDockWidget);
  //  }

    foliationDataTableDockWidget->widget()->setParent(nullptr);
    sepSegmentSearchDockWidget->widget()->setParent(nullptr);
    delete foliationDataTableDockWidget;
    delete sepSegmentSearchDockWidget;
}



const balazs::Foliation &FoliationMainWindow::activeFoliation() const
{
    assert(activeFoliationIndex >= 0);
    return activeFolationManager().foliation();
}

FoliationManager &FoliationMainWindow::activeFolationManager() const
{
    assert(activeFoliationIndex >= 0);
    return *foliations[activeFoliationIndex];
}



void FoliationMainWindow::createActions()
{
    findSepSegmentsAct = new QAction(tr("Find good separatrix segments..."), this);
    connect(findSepSegmentsAct, SIGNAL(triggered()), this, SLOT(openSepSegmentSearch()), Qt::UniqueConnection);

    rotateLeftAct = new QAction(tr("Rotate to Left"), this);
    rotateRightAct = new QAction(tr("Rotate to Right"), this);
    reverseAct = new QAction(tr("Reverse"), this);
    flipOverAct = new QAction(tr("Flip Over"), this);

    connect(rotateLeftAct, SIGNAL(triggered()), this, SLOT(rotateFoliationLeft()), Qt::UniqueConnection);
    connect(rotateRightAct, SIGNAL(triggered()), this, SLOT(rotateFoliationRight()), Qt::UniqueConnection);
    connect(reverseAct, SIGNAL(triggered()), this, SLOT(reverseFoliation()), Qt::UniqueConnection);
    connect(flipOverAct, SIGNAL(triggered()), this, SLOT(flipOverFoliation()), Qt::UniqueConnection);
}




//void FoliationMainWindow::updateConnections()
//{
//    if(foliations.empty()){
//        viewMenu->setEnabled(false);
//        searchMenu->setEnabled(false);
//        toolsMenu->setEnabled(false);

//    } else {
//        viewMenu->setEnabled(true);
//        searchMenu->setEnabled(true);
//        toolsMenu->setEnabled(true);


//    }


//}



//void FoliationMainWindow::setPermutationLabels(bool visible){
//    activeFolationManager().drawingArea()->setPermutationLabels(visible);
//}

//void FoliationMainWindow::setLengthsLabels(bool visible){
//    activeFolationManager().drawingArea()->setLengthsLabels(visible);
//}

//void FoliationMainWindow::setColoredFilling(bool visible){
//    activeFolationManager().drawingArea()->setColoredFilling(visible);
//}

//void FoliationMainWindow::setPermFontSize(int fontSize)
//{
//    activeFolationManager().drawingArea()->setPermFontSize(fontSize);
//}



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
    addDockWidget(Qt::BottomDockWidgetArea, sepSegmentSearchDockWidget);
}










void FoliationMainWindow::createMenus(const WindowManager& manager)
{
    fileMenu = new FileMenu(manager);
    menuBar()->addMenu(fileMenu);

    searchMenu = new QMenu(tr("Search"));
    searchMenu->addAction(findSepSegmentsAct);

    toolsMenu = new QMenu(tr("Tools"));
    reparametrizeMenu = toolsMenu->addMenu(tr("Reparametrize Foliation"));
    reparametrizeMenu->addAction(rotateLeftAct);
    reparametrizeMenu->addAction(rotateRightAct);
    reparametrizeMenu->addAction(reverseAct);
    reparametrizeMenu->addAction(flipOverAct);
    
    helpMenu = new HelpMenu(manager);
    menuBar()->addMenu(helpMenu);
}


//{



//lengthsFontSizeLabel = new QLabel(tr("Font size:"));
//lengthsFontSizeSpinBox = new QSpinBox;
//connect(lengthsFontSizeSpinBox, SIGNAL(valueChanged(int)), drawingArea, SLOT(setLengthsFontSize(int)));
//lengthsFontSizeSpinBox->setValue(10);

//}

