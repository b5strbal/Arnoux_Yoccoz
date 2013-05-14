#ifndef FILEMENU_H
#define FILEMENU_H

#include <QMenu>

class WindowManager;

class FileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit FileMenu(const WindowManager& manager, QWidget *parent = 0);
    
signals:
    
public slots:
    
private:
    QAction* newFoliationAct;

};

#endif // FILEMENU_H
