#include "WindowManager.h"
#include "CreateFoliationWizard.h"
#include "DocumentationWindow.h"
#include "FoliationMainWindow.h"



WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
    foliationMainWindow = new FoliationMainWindow(*this);
    foliationMainWindow->show();

    createFoliationWizard = new CreateFoliationWizard;
    connect(createFoliationWizard, SIGNAL(foliation(balazs::Foliation*)),
            foliationMainWindow, SLOT(createNewFoliation(balazs::Foliation*)));

    documentationWindow = new DocumentationWindow;

}




void WindowManager::newFoliationWizard()
{
    createFoliationWizard->restart();
    createFoliationWizard->show();
}


void WindowManager::openDocumentation()
{
    documentationWindow->show();
}
