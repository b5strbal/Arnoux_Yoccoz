#include "WindowManager.h"
#include "DocumentationWindow.h"
#include "foliationWindow/FoliationMainWindow.h"
#include "newFoliation/NewFoliationWindow.h"



WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
    foliationMainWindow = new FoliationMainWindow(*this);
    foliationMainWindow->show();

    newFoliationWindow = new NewFoliationWindow;
    connect(newFoliationWindow, SIGNAL(foliation(balazs::Foliation*)),
            foliationMainWindow, SLOT(createNewFoliation(balazs::Foliation*)));

    documentationWindow = new DocumentationWindow;

}




void WindowManager::newFoliation()
{
    newFoliationWindow->show();
}


void WindowManager::openDocumentation()
{
    documentationWindow->show();
}
