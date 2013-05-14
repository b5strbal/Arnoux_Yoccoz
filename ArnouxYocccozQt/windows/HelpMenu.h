#ifndef HELPMENU_H
#define HELPMENU_H

#include <QMenu>


class WindowManager;

class HelpMenu : public QMenu
{
    Q_OBJECT
public:
    explicit HelpMenu(const WindowManager& manager, QWidget *parent = 0);
    
signals:
    
public slots:
    
private:
    QAction* openDocumentationAct;
};

#endif // HELPMENU_H
