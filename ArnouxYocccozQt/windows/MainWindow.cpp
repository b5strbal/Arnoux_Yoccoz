#include "MainWindow.h"
#include "CreateFoliationWizard.h"
#include "DocumentationWindow.h"

#include <QAction>
#include <QMenuBar>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createActions();    
    createMenus();

    openFoliationsTabWidget = new QTabWidget;
    openFoliationsTabWidget->addTab(new QLabel("sadfasd"), "asdfs");
    setCentralWidget(openFoliationsTabWidget);
}


void MainWindow::newFoliation()
{
    if(!createFoliationWizard){
        createFoliationWizard = new CreateFoliationWizard(this);
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



void MainWindow::createActions()
{
    newFoliationAct = new QAction(tr("New Foliation..."), this);
    connect(newFoliationAct, SIGNAL(triggered()), this, SLOT(newFoliation()));
    
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


