#include "FileMenu.h"
#include "WindowManager.h"

FileMenu::FileMenu(const WindowManager &manager, QWidget *parent) :
    QMenu(parent)
{
    setTitle(tr("File"));
    newFoliationAct = addAction(tr("New Foliation..."));
    connect(newFoliationAct, SIGNAL(triggered()), &manager, SLOT(newFoliationWizard()));
}
