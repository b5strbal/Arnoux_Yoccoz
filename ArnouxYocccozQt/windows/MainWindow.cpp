#include "MainWindow.h"
#include "CreateFoliationWizard.h"
#include "DocumentationWindow.h"
#include "FoliationWindow.h"
#include "../drawing/drawings.h"

#include <QAction>
#include <QMenuBar>
#include <QLabel>
#include <QPainter>
#include <QListWidget>
#include <QHBoxLayout>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createActions();    
    createMenus();

    QWidget* centralWidget = new QWidget;

    foliationListWidget = new QListWidget;
    foliationListWidget->setMaximumWidth(100);

    foliationPagesWidget = new QStackedWidget;


    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(foliationListWidget);
    mainLayout->addWidget(foliationPagesWidget);


    setCentralWidget(centralWidget);

    connect(foliationListWidget, SIGNAL(currentRowChanged(int)),
            foliationPagesWidget, SLOT(setCurrentIndex(int)));

    for(int i = 3; i < 10; i++){
        newFoliationWindow(new balazs::Foliation(balazs::arnouxYoccozFoliation(i)));
    }
}


void MainWindow::newFoliationWizard()
{
    if(!createFoliationWizard){
        createFoliationWizard = new CreateFoliationWizard(this);
        connect(createFoliationWizard, SIGNAL(foliation(balazs::Foliation*)),
                this, SLOT(newFoliationWindow(balazs::Foliation*)));
    }
    createFoliationWizard->restart();
    createFoliationWizard->show();
}


void MainWindow::openDocumentation()
{
    if(!documentationWindow){
        documentationWindow = new DocumentationWindow;
    }
    documentationWindow->show();
}

void MainWindow::newFoliationWindow(balazs::Foliation* pFoliation)
{
    static int counter = 1;

    int index = foliationPagesWidget->addWidget(new FoliationWindow(pFoliation));
    foliationPagesWidget->setCurrentIndex(index);

    foliationListWidget->addItem(tr("Foliation %1").arg(QString::number(counter++)));
}



void MainWindow::createActions()
{
    newFoliationAct = new QAction(tr("New Foliation..."), this);
    connect(newFoliationAct, SIGNAL(triggered()), this, SLOT(newFoliationWizard()));
    
    openDocumentationAct = new QAction(tr("Documentation"), this);
    connect(openDocumentationAct, SIGNAL(triggered()), this, SLOT(openDocumentation()));
}



void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(newFoliationAct);
    
    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(openDocumentationAct);
}


