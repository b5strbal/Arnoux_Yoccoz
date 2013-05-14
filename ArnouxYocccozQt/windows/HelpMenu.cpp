#include "HelpMenu.h"
#include "WindowManager.h"


HelpMenu::HelpMenu(const WindowManager &manager, QWidget *parent) :
    QMenu(parent)
{
    setTitle(tr("Help"));
    openDocumentationAct = addAction(tr("Documentation"));
    connect(openDocumentationAct, SIGNAL(triggered()), &manager, SLOT(openDocumentation()));
}
