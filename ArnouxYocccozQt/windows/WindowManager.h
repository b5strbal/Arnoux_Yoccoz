#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

class DocumentationWindow;
class FoliationMainWindow;
class NewFoliationWindow;


class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = 0);
    
signals:
    
public slots:
    void newFoliation();
    void openDocumentation();
private:
    NewFoliationWindow* newFoliationWindow;
    DocumentationWindow* documentationWindow;
    FoliationMainWindow* foliationMainWindow;
};

#endif // WINDOWMANAGER_H
